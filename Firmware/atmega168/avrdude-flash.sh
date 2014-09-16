#!/bin/bash
avrdude -P /dev/ttyS0 -c siprog -p atmega168 -U flash:w:main.hex:i -U eeprom:w:main.eep -i 10 -v -s
# avrdude -b 2400 -P /dev/ttyS0 -c siprog -p atmega168 -U flash:w:main.hex:i -U eeprom:w:main.eep -v -s
# Read Fuses
# avrdude -c siprog -i 10  -p m168 -P /dev/ttyUSB0 -v -U lfuse:r:-:i
#oder alles:
# avrdude -c siprog -i 10 -p m168 -v