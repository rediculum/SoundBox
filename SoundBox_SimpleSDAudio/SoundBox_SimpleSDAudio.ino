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

SCK  (5) -> Pin 13 must be!
CS (1)   -> Pin 10 (Chip select)
MOSI (2) -> Pin 11 must be!
MISO (7) -> Pin 12 must be!

Speaker (sound) -> Pin 9 and GND

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
#define SPEAKER_PIN 9
#define SD_CHIPSELECT_PIN 10
#define Z_SWITCH_PIN A0
#define Z_LED_PIN A1
//#define BUTTONS 8 // Amount of acrade buttons
#define BUTTONS 16 

// Define your S-pins from your multiplexer here. 
//const byte selectPins[] = {2, 3, 4}; // like any 4051
const byte selectPins[] = {2, 3, 4, 5}; // CD74HC4067

// Do you use a 2nd multiplexer for the button LEDs?
bool led = false;
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
  Serial.begin(9600); // Set console to 57600 baud
  Serial.println("=== SoundBox based on SSDA ===");  
    
  if (led) {
    pinMode(Z_LED_PIN, OUTPUT);

    for (byte button=0; button<BUTTONS; button++)  {
      digitalWrite(Z_LED_PIN, HIGH);
      delay(100);
      digitalWrite(Z_LED_PIN, LOW);
    }
  }

  for (int i=0; i<s_pins; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }

  // Set Chip Select Pin
  SdPlay.setSDCSPin(SD_CHIPSELECT_PIN);
  // Initialize SD Card with library SimpleSDAudio
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) {
    tone(SPEAKER_PIN,100); delay(250); noTone(SPEAKER_PIN); delay(400);
    tone(SPEAKER_PIN,80); delay(600); noTone(SPEAKER_PIN);
    Serial.println("Error initializing SD Card");
  } else {
    Serial.println("SD Card initialized");
  }
}

void loop(void) { 
  // Loop through all eight pins holding an arcade button and set S pins accordingly.
  for (byte button=0; button<BUTTONS; button++) {
    for (byte i=0; i<s_pins; i++) {
      digitalWrite(selectPins[i], button & (1<<i)?HIGH:LOW);
    }
    // read Z and reduce value
    byte launch = map(analogRead(Z_SWITCH_PIN),0,1010,0,1);
    if (launch) {
      /* define a char array with an index size of 6 and convert
      integer "countImpulse" to char "track" using utoa() function
      http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html 
      */
      char track[6]; 
      utoa(button+1,track,10);
      // Append the suffix .PCM to the char array
      strcat(track, ".PCM");
      
      // Set the audio file if it exists and play it
      if (SdPlay.setFile(track)) {
        Serial.print("Playing..."); Serial.println(track);
        SdPlay.play();

        while(SdPlay.isPlaying()) {
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
        SdPlay.stop();
      } else {
        Serial.print("File "); Serial.print(track); Serial.println(" not found");
        delay(500);
      }
      
      if (led) {
        (fade) ? analogWrite(Z_LED_PIN, 0): digitalWrite(Z_LED_PIN, LOW);
      }
    delay(20);
    }
  }
  Serial.println();
}
