---
title: "Accel real-time graph"
author: "Mono Wireless Inc."
description: "Acceleration real-time graph"
---
# Accel real-time graph

`Windows` `macOS` `Linux`  `RasPi`&#x20;

Used for the purpose of referencing sample data from [TWELITE CUE](https://mono-wireless.com/cue) and [TWELITE MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/motion-pal.html).

![Showing demo data](../../../../../.gitbook/assets/img_graph_mot_demodata.png)

{% hint style="info" %}
Note: The [Sensor Graph Function](graph_sns.md) records one packet as one sample, even when continuous measurement is performed such that multiple samples are included in one packet.
{% endhint %}

CUE mode, MOT mode, and 2525 FIFO mode.

When the number of consecutive samples exceeds a certain number (analysis window), the XYZ axis is displayed with frequency analysis. However, in 2525 FIFO mode, it is always assumed to be continuous.

When a packet is explicitly delimited (when more than 3 seconds have elapsed since the previous packet, every packet in CUE mode, or when the sequence number of packets becomes discontinuous in MOT mode), a dummy of 4 samples is (When more than 3 seconds have elapsed since the last packet.

Data is stored for up to 4 nodes in order of arrival first.

{% hint style="warning" %}
During multi-node operation, communication failures often occur due to mutual packet collisions. In particular, in FIFO mode, the transmission cycle is almost constant, and packets are attempted to be sent at almost the same time to each other, resulting in transmission failures due to interference, which may continue for a long time. In principle, one node per CHANNEL should be used.
{% endhint %}

### Typical CUE Interactive settings mode settings

* measure continuously at about 25 Hz. (Transmit Interval: `0`, Sensor Parameter: `03000000`) 
* measure continuously at about 50 Hz. (Transmit Interval: `0`, Sensor Parameter: `03000100`)
* measure continuously at about 100 Hz. (Transmit Interval: `0`, Sensor Parameter: `03000200`)
* measure 128 samples at about 100Hz about every 10 seconds. (Transmit Interval: `10`, Sensor Parameter: `03000208`)
* measure 256 samples at about 100Hz about every 10 seconds. (Transmit Interval: `10`, Sensor Parameter: `03000210`)
* measure 64 samples at about 50Hz every 10 seconds. (Transmit Interval: `10`, Sensor Parameter: `03000104`)
* measure 128 samples at about 200Hz about every 10 seconds. (Transmit Interval: `10`, Sensor Parameter: `03000308`)



### Operation

| Operation                            |  Desc.                                                        |
| ------------------------------------ | ------------------------------------------------------------ |
| Right part of the `[(I)ID#]` button | Switch IDs with each click. (Note: For continuous sample data in FIFO mode, operation with multiple IDs is not practical.
| Right part `[(f)SMP#]` button | Change the frequency analysis size to 64, 128, or 256 with each click.
| right part `[[>PAUSE( )]` button | aborts display update (sample acquisition is performed until the internal temporary sample buffer is full) |
| right part `[(c)SAVE DISPLAYED DATA]` button | output data in CSV format (log folder). Outputs data from the sample displayed on the rightmost side of the screen to the oldest sample in the buffer. The number of output is always 5120, and the sample displayed on the rightmost side of the screen is the last data. | mouse drag (graph part)
| Mouse drag (graph section) | Move the position of the displayed sample |
| Mouse drag (bottom scroll bar) | Move the position of the displayed sample (in larger steps)
| Cursor→, ← | Move sample display |
| Cursor↑, ↓ | Zoom in/out on the sample horizontal axis (Equal, 2x, 3x, 4x; for 256 analysis samples, 2x). (Equal, 2x, 3x, 4x, up to 2x if the number of samples is 256) |


### Sample Rate Estimation

The sampling rate is calculated from the packet reception time. Since the reception time of multiple past samples is averaged to obtain one sample, the error will be larger if there are packet skips, etc.

After the sample rate is estimated, the graph display scrolls smoothly.

The timestamp of the related log record (T_SMPL) is also estimated in the same way, and the timestamp is after the time of packet acquisition.



### Open at startup of CUE graph mode.

Specify `31` in [STAGE Common Settings→Designate Startup Application].



### Log Output (Data Save)

By clicking the `[Data Save]` button, data for up to 512 samples will be output starting from the display position on the screen (rightmost sample). Note that the most recent sample comes last, so there may be no data at the beginning.

* The most recent sample on the right end is the 512th data (at the end of the file).
* When frequency analysis is performed, the number of frequency analysis samples is the number of frequency analysis samples from the last sample.
* The frequency analysis results are added to the line where the sample subject to frequency analysis is recorded (in the case of 64 samples, the 32nd line from the 449th sample is the result, from DC component to high frequency component).

| Label      | Item                             | Desc.                                                        |
| ---------- | -------------------------------- | ------------------------------------------------------------ |
| # | Sample number | |
| T_PKT[ms] | Packet reception time | Since one packet contains multiple samples, samples with the same timestamp are lined up. | SEQ
| SEQ | Packet Sequence Number | Packets are assigned and if they are consecutive, there is no missing packet. |
| T_SMPL[ms] | Sample time (virtual or estimated) | Timestamp of each sample generated from the packet reception time. It does not match the actual time the sample was taken. (The error is large because the sample rate is estimated from the received packet interval, and the timestamp is one packet period later than the actual sample time because the sample period is calculated by adding the sample period to the received sample.
| X[G] | Sample value on the X-axis | Based on the value from the sensor, the unit is [G].                  | Y[G] | Y-axis sample value
| Y[G] | sample value of Y axis | based on the value from the sensor, unit is [G].                  | Z[G] | Z-axis sample value
| Z[G] | sample value of Z-axis | based on the value from the sensor, unit is [G].                  | FD# | Frequency Analysis Calculation | Based on value from sensor, unit is [G].
| FD# | Number of the frequency analysis calculated value | If the number of frequency analysis samples is 64, they are ordered in the order of `DC,1,2,...,31`. ,31` if the number of frequency analysis samples is 64. | Hz
| Hz | Frequency axis value of the frequency analysis calculation result | Approximate frequency. The frequency is calculated by `(FD# / FD_Len) * FD_Freq`. | X
| X | Frequency analysis calculated value of X-axis | |
| Y | Frequency analysis calculated value of Y-axis | |
| X | X-axis frequency analysis calculated value | | Y | Y-axis frequency analysis calculated value | | Y-axis frequency analysis calculated value
| Label | additional information name | see table below | Info | additional information
| Info | additional information | see table below
| Label | Additional Information | Refer to the table below

#### Additional Information

| Label      | Item                             | Desc.                            |
| -------------- | ------------------------------------------------------------ | ---------------------------- |
| ModuleSID | Serial number of the sending side | |
| Tick[ms] | System on TWELITE STAGE app side at log file open [ms] | |
| Date | Date when log file was opened | Time | Time when log file was opened
| Time | Time_Msec_part | Time when log file was opened
| Time_Msec_part | Less-than-seconds portion of log file open [ms] | | | Samples | Valid sample data
| Samples | Valid sample data | | | FD_Len
| FD_Len | Number of frequency analysis samples | |
| FD_Start# | Frequency analysis start sample number | | | FD_Freq | Frequency analysis start sample number
| FD_Freq | Frequency estimate of frequency analysis range [Hz] | Estimated value from sample reception interval |
| | | FD_Freq

### Log output (auto save)

| Label      | Item                             | Desc.                                                         |
| ---------- | -------------------------- | ------------------------------------------------------------ |
| # | Sample number | |
| T_PKT[ms] | Packet reception time | Since one packet contains multiple samples, samples with the same timestamp are lined up. | SEQ
| SEQ | Packet Sequence Number | Packets are assigned and if they are consecutive, there is no missing packet. |
| T_SMPL[ms] | Sample time (virtual or estimated) | Timestamp of each sample generated from the packet reception time. It does not match the actual time the sample was taken. (The error is large because the sample rate is estimated from the received packet interval, and the timestamp is one packet period later than the actual sample time because the sample period is calculated by adding the sample period to the received sample.
| X[G] | Sample value on the X-axis | Based on the value from the sensor, the unit is [G].                  | 
| Y[G] | sample value of Y axis | based on the value from the sensor, unit is [G].                  | 
| Z[G] | sample value of Z-axis | based on the value from the sensor, unit is [G].                  | 
| Label | Additional information name | Refer to the following table
| Info | Additional Information | Refer to the table below |

#### Additional Information

| Label      | Item                         | Desc.          |
| ---------- | ---------------------------- | -------------- |
| ModuleSID  | Serial number of the sending side | |
| Tick[ms]   | System on TWELITE STAGE app side at log file open [ms] | |
| Date       | Date when log file was opened | |
| Time       | Time_Msec_part | Time when log file was opened |
| Time_Msec_part | Less-than-second part of log file open [ms] | |
