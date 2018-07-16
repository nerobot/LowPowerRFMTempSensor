#README

A low power, wireless temperature sensor using the RFM69 433MHz module and the PIC24FJ64GA202 XLP MCU

The PIC has been chosen over the atmega MCU due to far superior low power module.
Initial testing shows that the complete circuit's power consumption can be reduced down to around 700nA during sleep.

Similar testing on the atmega only allowed sleep power consumption to go down to 4uA, so quite a difference.

Unfortunately, it did mean re-writing the RFM69 code, but this is pretty much a complete copy of the Arduino library, but with a few small changes as needed.

The library needs updating as it's quite a mess, but it does work.