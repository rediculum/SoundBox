# SoundBox
Sound box like a board to launch samples stored as 8bit full-rate (32000) mono WAV files named from 1.WAV to 8.WAV (16.WAV) on a SD card with arcade buttons using a multiplexer
## Pinout
### SD Card > Arduino pinout:
```
SCK  (5) -> Pin 13 must be!
CS (1)   -> Pin 10 (Chip select)
MOSI (2) -> Pin 11 must be!
MISO (7) -> Pin 12 must be!
```
### Speaker -> Arduino pinout:
```
Speaker (sound) -> Pin 9 and GND
```
### Multiplexer > Arduino pinout:
```
S0 -> Pin 2
S1 -> Pin 3
S2 -> Pin 4
S3 -> Pin 5 if you have a CD74HC4067 mux and provide 16 arcade buttons
SIG -> Pin A0 // Z of the button switch mux
SIG -> Pin A1 // Z of the button led mux if you use 5V luminated buttons
```
![](https://github.com/rediculum/SoundBox/raw/master/SoundBox_fritzing.png)
## References and used libraries
TMRpcm library for audio playback from SD card
https://github.com/TMRh20/TMRpcm or install it from Arduino IDE Lib Manager

How to deal with mux and demux
https://learn.sparkfun.com/tutorials/multiplexer-breakout-hookup-guide/all

## Convert audio to WAV with sox
This sox command will convert an MP3 file into 8bit 32k rate mono wave file.
```
sox audiofile.mp3 --norm -b8 -r32000 -c1 1.WAV
```
You may install the *libsox-fmt-mp3* package to support MP3 convertion.

## Enclosure printing
Refer to Thingiverse.com for STL download or use the Customizer to make your own case
https://www.thingiverse.com/thing:3830885
![](https://github.com/rediculum/SoundBox/raw/master/images/SoundBox_lid_printing.jpg)
