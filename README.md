# L-Tek DDR Pad XInput Mod

This project is another way to replace the stock control board in an L-Tek pad. This time, things are mapped to an XInput controller. Support for Start and Back buttons is built in as well as the ability to set up LEDs under the arrows to light when pressed. Also, a SNES controller can be connected to fill out more of the XInput controls. I'll need to double check everything, but the loop should take far less than 1000 micros to complete for a 1000Hz polling frequency.

The following Digital Pins are used:

Pin | Function
--- | --------
4 | DDR Down Arrow
5 | DDR Right Arrow
6 | DDR Left Arrow
7 | DDR Up Arrow
8 | DDR Start Button
9 | DDR Back Button
21 | DDR Down Arrow LED
20 | DDR Right Arrow LED
19 | DDR Left Arrow LED
18 | DDR Up Arrow LED
15 | SNES Data Pin
14 | SNES Latch Pin
16 | SNES Clock Pin


Pad and Controller inputs map to the following XInput controls by default:

Pad | XInput
--- | ------
DDR Up | Y
DDR Down | A
DDR Left | X
DDR Right | B
DDR Start | Start
DDR Back | Back
SNES D-pad (toggle off) | D-pad
SNES D-pad (toggle on) | Left Stick
SNES Start | Start
SNES Back | Back
SNES L | Left Bumper
SNES R | Right Bumper
SNES Face Buttons | Right Stick

Holding L+R+Select and pressing a direction on the Face Buttons, D-Pad, or DDR Pad will map them to the following XInput controls:

Direction | XInput
--------- | ------
Left/Y | Left Stick
Right/A | Right Stick
Up/X | Face Buttons
Down/B | D-Pad

You can overload mappings, including having all three mapped the same. This configuration is saved to EEPROM on change and loaded upon powerup. In the event that the EEPROM data is not valid, controls will revert to defaults.

This project uses [Dave Madison's ArduinoXInput Library](https://github.com/dmadison/ArduinoXInput). You will need to install his [XInput USB Core](https://github.com/dmadison/ArduinoXInput_AVR) to use it on your board. Please read the documentation of both projects to find out how to set it up on a compatible board.

To Do:
~~Set up toggling of custom D-pad, arrow, and face button mappings to XInput Left Stick, Right Stick, D-pad, and face buttons.~~
Have LEDs respond to player number data on startup and then switch to lighting up when pressed.
