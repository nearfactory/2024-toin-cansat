# Terms

## TWELITE NET

A library of procedures for wireless communication included in this TWELITE SDK.

## Simple net

TWELITE NET does not perform any special relay processing, and any relaying is controlled by the application.

## Relay net

This method is used to perform relay processing within TWELITE NET.

## MAC ACK, ACK, with ACK

A procedure to check whether a packet has been received by a destination as defined in IEEE802.15.4. A very short ACK packet is sent back immediately after reception is complete.

In exceptional cases, ACK transmission or ACK reception may fail, so that ACK does not guarantee that an acknowledgement has been passed on to the destination, and conversely, failure to obtain an ACK does not guarantee that it has not been passed on to the destination.

## Nodes, stations

A general term for a single radio, or a group of radios with a common function. Described as a radio station, parent station, child station or relay station.

## [Addresses](paketto/adoresuno.md) (long addresses, short addresses)

An identification number to identify a radio station.

## broadcast transmission

A method of communicating simultaneously without identifying the radio station. The signal is transmitted to the radio stations that can receive it. There is no guarantee that all radio stations will receive the message.

## Application ID

Network identifier. Communication is only possible between radio stations with the same application ID.

## LQI

Link Quality Indicator, a value used to evaluate the quality of communication as specified in IEEE802.15.4. RSSI is also used for the same purpose, although the exact definition is different.

LQI is not strictly defined in the standard for physical quantities or phenomena in wireless packet communications, but TWELITE NET uses a value equivalent to the field strength at reception.

## [DIO](../hw-api-ref/perifuraru/dio.md)

General Purpose IO (GPIO). It can be used for digital input and output.
