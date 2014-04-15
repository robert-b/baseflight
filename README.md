baseflight
==========

32 bit fork of the MultiWii RC flight controller firmware

Added in this fork :

 * ["ACRO RATE"](https://github.com/multiwii/baseflight/pull/29) is added to enable a second expo/rate while flying. This allow a mild version of expo/rate for precision then another set for acro.
 * [filtering sensor data with a kalman filter](http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter). Extraced and rework from https://github.com/multiwii/baseflight/pull/54

       set gyro_lpf = 188
 