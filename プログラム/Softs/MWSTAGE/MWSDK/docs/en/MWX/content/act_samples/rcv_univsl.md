---
title: "Rcv_Unvsl (Universal Reciever)"
author: "Mono Wireless Inc."
description: "Rcv_Unvsl (Universal Reciever)"
---

# Rcv_Unvsl (Universal Reciever)

By running `NWK_LAYERED` on `twe_twelite.network` and `NWK_SIMPLE` on `twe_twelite.network2` in the MWX library, packets of different types, including packets of layered tree net (TWELITE PAL, ARIA, etc) You can receive and interpret various types of packets.

However, wireless packets must be on the same CHANNEL and have the same Application ID.



## main.cpp

`setup()`, `loop()`, and the callback function `on_rx_packet()` for incoming packets.



### setup()

These objects are declared in `pkt_handler.cpp` and initialized by `pnew()` in `setup()`. It mainly interprets the payload (data) of the packet.

```cpp
	mwx::pnew(g_pkt_pal);
	mwx::pnew(g_pkt_apptwelite);
	mwx::pnew(g_pkt_actsamples);
	mwx::pnew(g_pkt_unknown);
```



Two network objects are created. Be sure to `NWK_LAYERED` to `the_twelite.network`.

```cpp
	auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
	auto&& nwk_sm = the_twelite.network2.use<NWK_SIMPLE>();
```



### loop()

In this sample, the important part of the `loop()` process is the `.refresh()` process, which is performed about every second. Only `g_pkt_apptwelite().refresh()` does the duplicate checker timeout. Other objects do nothing.

```cpp
	if (TickTimer.available()) {
		static unsigned t;
		if (!(++t & 0x3FF)) {
			g_pkt_pal.refresh();
			g_pkt_apptwelite.refresh();
			g_pkt_actsamples.refresh();
			g_pkt_unknown.refresh();
		}
	}
```



### on_rx_packet()

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	auto type = rx.get_network_type();
	bool b_handled = false;

	// PAL
	if (!b_handled
		&& type == mwx::NETWORK::LAYERED
		&& g_pkt_pal.analyze(rx, b_handled)
	) {
		g_pkt_pal.display(rx);
	}

	// Act samples
	if (!b_handled
		&& type == mwx::NETWORK::SIMPLE
		&& g_pkt_actsamples.analyze(rx, b_handled)
	) {
		g_pkt_actsamples.display(rx);
	}

	// Standard application (e.g. App_Twelite)
	if (!b_handled
		&& type == mwx::NETWORK::NONE
		&& g_pkt_apptwelite.analyze(rx, b_handled)
	) {
		g_pkt_apptwelite.display(rx);
	}

	// unknown
	if (!b_handled) {
		g_pkt_unknown.analyze(rx, b_handled);
		g_pkt_unknown.display(rx);
	}
}
```

This is the most important part of this sample code. `auto type = rx.get_network_type();` determines the packet type.

* `mwx::NETWORK::LAYERED` : `NWK_LAYERED` layer tree net ket
* `mwx::NETWORK::SIMPLE` : `NWK_SIMPLE` packets
* `mwx::NETWORK::NONE` : no network (e.g. App\_Twelite)
* other : error or unsupported packets



In the case of `mwx::NETWORK::NONE`, the MWX library does not handle duplicate checkers for the same packet that may be sent in multiple retransmissions. It is necessary to describe the handling of these. In this sample, `dup_checker.hpp` and `dup_checker.cpp` are provided.



Interpretation of the packet refers to the `packet_rx&` object that wraps `tsRxDataApp*`. The `packet_rx` class itself has no special functionality, it only defines a means of accessing some information obtained from `tsRxDataApp*` using `get_psRxDataApp()`.



## pkt_common.hpp

It is defined for the purpose of unifying the interface of the packet interpretation part.

```cpp
template <class D>
struct pkt_handler {
	D& self() { return static_cast<D&>(*this); }
	bool analyze(packet_rx& rx, bool &b_handled) {
		return self().pkt.analyze(rx, b_handled);
	}
	void display(packet_rx& rx) {
		Serial
			<< crlf
			<< format("!PKT_%s(%03d-%08x/S=%d/L=%03d/V=%04d)"
					, self().get_label_packet_type()
					, self().pkt.data.u8addr_src
					, self().pkt.data.u32addr_src
					, rx.get_psRxDataApp()->u8Seq
					, rx.get_lqi()
					, self().pkt.data.u16volt
					);

		self().disp_detail(rx);
	}
	void refresh() {
		self()._refresh();
	}
};

// packet analyzer for App_Twelite
class pkt_handler_apptwelite : public pkt_handler<pkt_handler_apptwelite> {
	friend class pkt_handler<pkt_handler_apptwelite>;
	pkt_apptwelite pkt;
	void disp_detail(packet_rx& rx);
	const char* get_label_packet_type() { return "AppTwelite"; }
	void _refresh() { pkt.refresh(); }
public:
	pkt_handler_apptwelite() : pkt() {}
};
```



* `analyze()` : interpret the payload of a packet.
* `display()` : Display packet information.
* `refresh()` : describe the process every second.
* `self()` : cast to the derived class `D`.



In addition, the packet interpretation class (example `pkt_handler_apptwelite` above) contains a member object `pkt`. The actual packet interpretation part is done in `pkt_???.cpp`.



## pkt\_???.hpp, pkt\_???.cpp

A packet interpretation part `analyze()` for each packet type and a data structure `data` are defined. The member `data` is a structure but inherits the common structure of `PktDataCommon`. This common part is used to concisely describe the code for serial output of the packet's data.



### pkt_pal

Corresponds to PAL-related packets; the PAL packet structure has a complex data structure. The implementation here is based on the EASTL container.

* `_vect_pal_sensors` : pool of `_pal_sensor` objects. This object is a dedicated class for use with instusive map.
* `_map_pal_sensors` : intrusive map structure for efficient retrieval of sensor data.

Allocate an entry in `_vect_pal_sensors` for each of the multiple data in a packet as they are added and store the value. Once all the data in a packet has been interpreted, a `_map_pal_sensors` is constructed with the sensor type as the key.



## dup_checker

Implement a duplicate checker. The behavior of the checker can be customized by template arguments.



### template argument

* `MODE` : If `MODE_REJECT_SAME_SEQ` is specified, packets with the same sequence number are excluded. This is used when the packet order is reordering. The `MODE_REJECT_OLDER_SEQ` adopts a more recent number.
* `TIMEOUT_ms` : The interval at which to initialize the duplicate database. If `1000` is specified, data after 1 second will be erased. Packets that were excluded immediately before will be adopted again when the duplicate database is initialized.
* `N_ENTRIES` : The maximum number of elements to be allocated in the data structure.
* `N_BUCKET_HASH` : Maximum number of hash values. Specifies a prime number. It is determined based on the type of wireless node being received.



### container

* `_mmap_entries` : It is an intrusive hash multi-map structure. The search key is the serial number of the wireless node.
* `_vect_pool` : Allocates a fixed number (`N_ENTRIES`) of elements used in the map structure.
* `_ring_vecant_idx` : Keeps track of `_vect_pool` elements not used in `_mmap_entries` by array index number. It is a ring buffer structure, which takes one value from the ring buffer when adding elements and returns a value to the ring buffer when removing elements.



### duplicate check

```cpp
	bool check_dup(uint32_t u32ser, uint16_t u16val, uint32_t u32_timestamp) {
		// find entry by key:u32ser.
		auto r = _mmap_entries.equal_range(u32ser);

        ...
    }
```

To retrieve data from the multimap structure, call `.equal_range()`. The resulting `r` is an iterator that enumerates elements with the same serial number.

Each element (`_dup_checker_entry`) has a timestamp and sequence number. Duplicates are checked according to these values.





