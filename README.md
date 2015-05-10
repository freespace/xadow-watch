# Run time

Run time is currently approximately 8 hours. This depends however on
using my modified versions of the Seeedstudio libraries:

* [`Xadow_mainboard`](https://github.com/freespace/Xadow_MainBoard)
* [`RGB_OLED_SSD1331`](https://github.com/freespace/RGB_OLED_SSD1331)
* [`SGL`](https://github.com/freespace/SGL)

Additionally to compile this project in Arduino 1.6.x you will need to
install the `seeeduino` vendor files in the `v1.6.3` directory in
[Files_For_Seeed_Main_Board](https://github.com/freespace/Files_For_Seeed_Main_Board).

Battery logging will reduce run time by around 20 minutes due to use of
ADC.

# Module API

There is an informal module C API which is as follows:

* `<module>_init`: called once during `setup`. May take arguments as
                   needed

* `<module>_show`: called during wake periods. Will be given a `uint8_t`
                   indicating the number of changes to the current time.

## Changes to current time

The value passed to `<module>_show` has values from 0..5, where:

*  0 - no change to time
*  1 - second changed
*  2 - minute changed
*  3 - hour changed
*  4 - day changed
*  5 - month changed

Because change starts with seconds, a change in a high order counter
means changes in lower order counters. For example of change is 3,
indicating the hour has changed, the minutes and seconds has also
changed.

### Redraw bit

The MSB of the change field is expected to never be used. If it is set,
it signals to the module that it should complete redraw its interface.
