/*
===========================================================================
Sound Box 1.0
by ReDiculum (Aug - Sep 2019)
-------------------------------
Sound box like a board to launch samples stored as 
8bit full-rate (62500) mono PCM files named from 1.PCM to 8.PCM (16.PCM)
on a SD card with arcade buttons using a multiplexer

https://github.com/rediculum/SoundBox

SD Card > Arduino pinout:

SCK  (5) -> Pin 9
CS (1)   -> Pin 10 (Chip select)
MOSI (2) -> Pin 11
MISO (7) -> Pin 12

Speaker (sound) -> Pin 6 and GND (use a poti for volume regulation)

Multiplexer > Arduino pinout:
S0 -> Pin 2
S1 -> Pin 3
S2 -> Pin 4
S3 -> Pin 5 if you have a CD74HC4067 mux and provide 16 arcade buttons
SIG -> Pin A0 // Z of the button switch mux
SIG -> Pin A1 // Z of the button led mux if you use 5V luminated buttons

===========================================================================
References:

SimpleSDAudio library for audio playback from SD card
http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio

How to deal with mux and demux
https://learn.sparkfun.com/tutorials/multiplexer-breakout-hookup-guide/all

===========================================================================
=== Begin of CONFIGURATION ===
*/

// Pin assignment
#define SPEAKER_PIN 6
#define SD_CHIPSELECT_PIN 10
#define Z_SWITCH_PIN 14 // A0
#define Z_LED_PIN 15 // A1
#define BUTTONS 8 // Amount of acrade buttons
//#define BUTTONS 16 

// Define your S-pins from your multiplexer here. 
const int selectPins[3] = {2,3,4}; // like any 4051
//const int selectPins[4] = {2,3,4,5}; // CD74HC4067

// Do you use a 2nd multiplexer for the button LEDs?
bool led = true;
bool fade = true; // only works with analog Z on mux

/*
===========================================================================
=== End of CONFIGURATION ===
*/

#include <SimpleSDAudio.h>

byte s_pins = sizeof(selectPins);
byte brightness = 0;
bool increment;
  
void setup() {

  Serial.begin(57600); // Set console to 57600 baud
  Serial.println("=== SoundBox based on SSDA ===");  
  if (led) {
    pinMode(Z_LED_PIN, OUTPUT);

    for (byte button=0; button<=BUTTONS; button++)  {
      digitalWrite(Z_LED_PIN, HIGH);
      delay(100);
      digitalWrite(Z_LED_PIN, LOW);
    }
  }
  
  // Set Chip Select Pin
  SdPlay.setSDCSPin(SD_CHIPSELECT_PIN);
  // Initialize SD Card with library SimpleSDAudio
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) {
    tone(SPEAKER_PIN,100); delay(250); noTone(SPEAKER_PIN); delay(400);
    tone(SPEAKER_PIN,80); delay(600); noTone(SPEAKER_PIN);
    Serial.println("Error initializing SD Card");
  }
}

void loop(void) { 
  // Loop through all eight pins holding an arcade button.
  for (byte button=0; button<=BUTTONS; button++) {
    selectMuxPin(button, s_pins);
    // read Z and reduce value
    byte launch = map(analogRead(Z_SWITCH_PIN),0,1023,0,1);

    if (launch) {
      Serial.println();
      delay(100);

      /* define a char array with an index size of 6 and convert
      integer "countImpulse" to char "track" using utoa() function
      http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html 
      */
      char track[6]; 
      utoa(button,track,10);
      // Append the suffix .PCM to the char array
      strcat(track, ".PCM");
      
      // Set the audio file if it exists and play it
      if (SdPlay.setFile(track)) {
        Serial.print("Playing..."); Serial.println(track);
        SdPlay.play();

        while(!SdPlay.isStopped()) {
          if (led) {
            if (fade) {
              analogWrite(Z_LED_PIN, brightness);
              (brightness == 0) ? increment == true : true;
              (brightness == 255) ? increment == false : true;
              (increment) ? brightness++ : brightness--;
              delay(30);
            } else {
              (digitalRead(Z_LED_PIN) == "LOW") ? digitalWrite(Z_LED_PIN, HIGH) : digitalWrite(Z_LED_PIN, LOW);
              delay(200);
            }
          }
        }
      } else {
        Serial.print("File "); Serial.print(track); Serial.println(" not found");
        tone(SPEAKER_PIN,80); delay(250); noTone(SPEAKER_PIN); delay(400);
        tone(SPEAKER_PIN,120); delay(600); noTone(SPEAKER_PIN);
      }
      if (led) {
        (fade) ? analogWrite(Z_LED_PIN, 0): digitalWrite(Z_LED_PIN, LOW);
      }
    delay(200);
    }
  }
}

// The selectMuxPin function sets the S-pins
// accordingly, given a pin from 0-7.
void selectMuxPin(byte pin, byte pins) {
  for (int i=0; i<pins; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
