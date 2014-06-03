#Baseflight - STM32 F3 DEV version for the NAZE32Pro#


##Polish me##
these things work and are tested

* motor/servo pwm 10 out
* standard rx pwm 8 chan
* serial rx : spketrum sat
* ppm rx
* usb cli/msp
* bluetooth/433mhz msp on telemport 
* gps nmea (rx) on telemport , rc pin
* baro5611 spi 
* mag5983 spi
* mpu6050 spi


##Test Me##
these things need to be tested

* buzzer
* sbus 
* sumd
* gps ublox (rx/tx)


##Fix Me##

* detect acc sensors range
* headfree
* led1 is not present
* i2c on rc 3/4 pin, need configuration for offest rcpin
* vbat monitor
* softserial
* non msp telemetry
* startup.s will reboot to dfu with gcc , keil startup.s  will reset/reboot
* build make target=NAZE  .. worth fixing f103 build or remove thing for rev5 i2c ?
* all the things