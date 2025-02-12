/* Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */
#pragma once

#include <stdarg.h>
#include "twecommon.h"
#include "tweserial.h"
#include "tweserial_jen.h"
#include "tweprintf.h"

#include "mwx_stream.hpp"

#include <AppHardwareApi.h>
#include "serial.h"

namespace mwx { inline namespace L1 {
	// calculate baud/100 avoiding division.
	constexpr uint16_t _serial_get_hect_baud(uint32_t baud) {
		// note: constexpr does work with very limited condition.
		//   static_assert(_serial_get_hect_baud(115200) == 1152, "no constexpr computation at compile time.");
		return baud == 115200 ? 1152 :
				(baud == 38400 ? 384 :
			    	(baud == 9600 ? 96 : 
						(baud / 100) // last one
				))
		;
	}

	class serial_jen : public mwx::stream<serial_jen> {
		uint8_t* _au8SerialTxBuffer; // internal buffer FIFO queue
		uint8_t* _au8SerialRxBuffer; // internal buffer FIFO queue
		struct {
			uint8_t _u8Port;
			uint8_t _u8Conf;
			uint16_t _u16HectBaud;
		} _serdef;

		uint16_t BUF_TX;
		uint16_t BUF_RX;

		TWE_tsFILE* _psSer;

		// constructor, does nothing
		serial_jen() {}

		// special value of configuration byte, which is not initialized yet.
		static const uint8_t CONF_VOID = 0xFF;

	public:
		// obj ptr for surrogating available(), read().
		//  - for the_twelite.settings which intercepts read().
		struct SURR_OBJ {
			void *pobj;
			bool (*pf_avail)(void*);
			int (*pf_read)(void*);
		} *_surr_obj;

		// SURR_OBJ is use for Interactive mode. (this should be stored _psSer->uData.au8[4].)
		static const uint8_t SURR_OBJ_PURPOSE_INTERACTIVE_MODE = 1;

	private: 
		void set_surrobj_purpose(uint8 u8purpose) {
			_psSer->uData.au8[4] = u8purpose;
		}
		bool is_surrobj_for(uint8 u8purpose) {
			return _psSer->uData.au8[4] == u8purpose;
		}
		static bool is_surrobj_for(TWE_tsFILE* psSer, uint8 u8purpose) {
			return psSer->uData.au8[4] == u8purpose;
		}
		
	public:
		// parameters for begin()
		struct E_CONF {
			static const uint8_t PORT_ALT = 0x01;
		};

		
	private:
		void _begin() {
			if (_setup_finished()) {
				// configure with alt port
				//   NOTE: The default UART1 ports set is different from AHI defaults from mwx.
				//         default: DIO11(TxD),9(RxD) / alternative: DIO14,15.
				if(_serdef._u8Port == E_AHI_UART_1) {
					vAHI_UartSetLocation(E_AHI_UART_1, 
							(_serdef._u8Conf & E_CONF::PORT_ALT) ? TRUE : FALSE);
				}
			
				TWETERM_tsSerDefs sDef;
				sDef.au8RxBuf = _au8SerialRxBuffer;
				sDef.au8TxBuf = _au8SerialTxBuffer;
				sDef.u16RxBufLen = BUF_RX;
				sDef.u16TxBufLen = BUF_TX;

				sDef.u32Baud = 115200;
				TWETERM_vInitJen(_psSer, _serdef._u8Port, &sDef);
				_set_baudrate(_serdef._u16HectBaud);

				SUPER::pvOutputContext = (void*)_psSer;
				SUPER::_bReady = 1;
			}
		}

		void _set_baudrate(uint16 u16hbaud) {
			uint16 u16Divisor;
			uint8 u8cbp;

			// find ideal div/cbp for JN516x
			switch(u16hbaud) {
			case 96:
				u16Divisor = 104;
				u8cbp = 15;
				break;
			case 192:
				u16Divisor = 52;
				u8cbp = 15;
				break;
			case 384:
				u16Divisor = 26;
				u8cbp = 15;
				break;
			case 576:
				u16Divisor = 23;
				u8cbp = 11;
				break;
			case 768:
				u16Divisor = 13;
				u8cbp = 15;
				break;
			case 1152:
				u16Divisor = 10;
				u8cbp = 13;
				break;
			case 2304:
				u16Divisor = 5;
				u8cbp = 13;
				break;
			default:
				/* Calculate divisor: baud=1Mbps/Div */
				u16Divisor   = (uint16)((10000UL + u16hbaud/2) / u16hbaud);
				u8cbp = 15; // default value
				break;
			}

			// set baudrate
			vAHI_UartSetBaudDivisor(_serdef._u8Port, u16Divisor);
			vAHI_UartSetClocksPerBit(_serdef._u8Port, u8cbp);
		}

	public:
		using SUPER = mwx::stream<serial_jen>;

		// constructor, does nothing
		serial_jen(uint8_t u8Port) {
			_psSer = nullptr;
			_serdef._u8Port = u8Port;
			_serdef._u16HectBaud = 1152;
			_serdef._u8Conf = CONF_VOID; // set other value when begins.

			_surr_obj = nullptr;

			BUF_TX = 0;
			BUF_RX = 0;
		}

		// setup() : initilize class and prepare internal buffers.
		//   - shall call once.
		void setup(uint16_t buf_tx, uint16_t buf_rx) {
			if (!_setup_finished()) {
				BUF_TX = buf_tx;
				BUF_RX = buf_rx;

				_au8SerialTxBuffer = new uint8_t[BUF_TX];
				_au8SerialRxBuffer = new uint8_t[BUF_RX];

				_psSer = new TWE_tsFILE();
				SUPER::_bSetup = (uint8_t)true;
			}
		}

		// begin() : start UART port
		//   - shall call setup() in advance.
		//   so far, config is NOT handled.
		void begin(uint32_t speed = 115200, uint8_t config = CONF_VOID) {
			if (_setup_finished()) {
				uint16 hbaud = _serial_get_hect_baud(speed);

				if (_serdef._u8Conf == CONF_VOID) {
					// initial, firstly begins.
					_serdef._u8Conf = (config == CONF_VOID) ? 0 : config;
					_serdef._u16HectBaud = hbaud;
					_begin();
				} else {
					// change baud, already begun.
					if (hbaud != _serdef._u16HectBaud) {
						_serdef._u16HectBaud = hbaud;
						_set_baudrate(hbaud);
					}
				}
			}
		}

		// end() : de-init the port
		//   NOTE: not implemented.
		void end(void) {
			SUPER::_bReady = 0;
		}

		inline int available() {
			return _surr_obj ? _surr_obj->pf_avail(_surr_obj->pobj) : (!SERIAL_bRxQueueEmpty(_serdef._u8Port));
		}

		inline void flush(void) {
			_psSer->fp_flush(_psSer);
		}

		inline int read() {
			if (_surr_obj && is_surrobj_for(SURR_OBJ_PURPOSE_INTERACTIVE_MODE)) {
				return _surr_obj->pf_read(_surr_obj->pobj);
			} else {
				int iChar = -1;
				if (!SERIAL_bRxQueueEmpty(_serdef._u8Port)) {
					iChar = SERIAL_i16RxChar(_serdef._u8Port);
				}
				return iChar;
			}
		}

		inline size_t write(int n) {
			if (_surr_obj) {
				if (TWEINTRCT_bIsVerbose()) return 0;
			}

			return (int)SERIAL_bTxChar(_serdef._u8Port, n);
		}

		// for upper class use
		static void vOutput(char out, void* vp) {			
			TWE_tsFILE* fp = (TWE_tsFILE*)vp;
			if (is_surrobj_for(fp, SURR_OBJ_PURPOSE_INTERACTIVE_MODE)
					 && TWEINTRCT_bIsVerbose()) return;

			fp->fp_putc(out, fp);
		}

		// called about to sleep.
		void _on_sleep() {
			if (_setup_finished()) {
				flush();
			}
		}

		// called when waking up.
		void _on_wakeup() {
			if (_setup_finished()) {
				begin(_serdef._u16HectBaud * 100, _serdef._u8Conf);
				_begin(); // re-begin
			}
		}

public:
		// class specific
		TWE_tsFILE* get_tsFile() { return _psSer; }
		uint8 get_Port() { return _serdef._u8Port; }

		void register_surrogate(SURR_OBJ *obj, uint8_t u8purpose = SURR_OBJ_PURPOSE_INTERACTIVE_MODE) {
			_surr_obj = obj;
			set_surrobj_purpose(u8purpose);
		}
	};
}}
