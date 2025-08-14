This code is a test to reuse Color-LCDs from old printers.

This is for a 320x200 color LCD from a Canon MP640R. The display is a LQ030Q3DW01.

The code is under construction an currently not working.

The current version of the code try to send all the signals with CPU power of the Raspberry Pi Pico. Its much beter as the Atmega experiment, but we don't get an image. The displays shows only a fading effect that looks more like a electical damage with to high voltage.
Maybe the data is streaming to slow to the display.

ToDo:
- use programmable IO to do the job
