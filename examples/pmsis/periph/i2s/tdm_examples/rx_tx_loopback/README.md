# Loopback(Out -> Out)

This is an example to use RX and TX at the same time. Incoming data on RX are not saved in memory but are sent back on TX.

## Configuration

The number of slots/channels, here microphones, used are configured using with `NB_CHANNELS` and `NB_ACTIVE_CHANNELS`.

Bypass mode can be set for each TX channels independently, enabling or disabling each slot.

Optional :
* `PI_I2S_OPT_EXT_CLK` : use external clock for SCK/BCLK.
* `PI_I2S_OPT_EXT_WS` : use external clock for WS/LRCLK. This option is to be used be used with `PI_I2S_OPT_EXT_CLK`.
These options should be set before opening i2s device.

## FPGA connection

Data should be sent on `J4-4`. FPGA settings should be as follows :
  * `JMP7` : on if data from microphones
  * `JMP7` : off otherwise.

#### Pin mapping
* I2S_0 :
  * `J4-5` : SCK/BCLK
  * `J4-4` : RX or TX(when not in full duplex mode)
  * `J4-3` : WS/FS
  * `J2-10` : TX in full duplex mode

* I2S_1 :
  * `J5-5` : SCK/BCLK
  * `J5-4` : RX or TX(when not in full duplex mode)
  * `J5-3` : WS/FS
  * `J2-12` : TX in full duplex mode

* I2S_2 :
   * `J6-5` : SCK/BCLK
   * `J6-4` : RX or TX(when not in full duplex mode)
   * `J6-3` : WS/FS
   * `J2-14` : TX in full duplex mode