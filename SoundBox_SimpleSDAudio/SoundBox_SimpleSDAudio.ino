/*
==========================================================================
Sound Box 1.0
by ReDiculum (Aug - Sep 2019)
-------------------------------
Sound box like a board to launch samples stored as 
8bit full-rate (62500) mono PCM files named from 1.PCM to 10.PCM
from arcade buttons using a multiplexer

SD Card > Arduino pinout:

SCK  (5) -> Pin 9
CS (1)   -> Pin 10 (Chip select)
MOSI (2) -> Pin 11
MISO (7) -> Pin 12

Speaker (sound) -> Pin 6 and GND

Multiplexer > Arduino pinout:
S0 -> Pin 2
S1 -> Pin 3
S2 -> Pin 4
S3 -> Pin 5 if you have a CD74HC4067 and provide 16 arcade buttons
SIG -> Pin A0

============================================================================

SimpleSDAudio library for audio playback from SD card
http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio

How to deal with mux and demux
https://learn.sparkfun.com/tutorials/multiplexer-breakout-hookup-guide/all

*/
#include <SimpleSDAudio.h>

// Pin assignment
#define SPEAKER_PIN 6
#define SD_CHIPSELECT_PIN 10
#define LED_PIN 13 // onboard LED
#define SIG_PIN 14 // A0
#define BUTTONS 8 // How many acrade buttons
//#define BUTTONS 16 

// Define your S-pins from your multiplexer here. 
const int selectPins[3] = {2,3,4}; // like any 4051
//const int selectPins[4] = {2,3,4,5}; // CD74HC4067

byte s_pins = sizeof(selectPins);

void setup() {

  Serial.begin(57600); // Set console to 57600 baud
  Serial.println("=== SoundBox based on SSDA ===");  

  pinMode(LED_PIN, OUTPUT);
  
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
  for (byte button=0; button<=BUTTONS; button++)
  {
    selectMuxPin(button, s_pins);
    // read Z and reduce value
    byte launch = map(analogRead(SIG_PIN),0,1023,0,1); 

    if (launch == 1) {
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
          ;
        }
        SdPlay.deInit();
      } else {
        SdPlay.deInit();
        Serial.print("File "); Serial.print(track); Serial.println(" not found");
        tone(SPEAKER_PIN,80); delay(250); noTone(SPEAKER_PIN); delay(400);
        tone(SPEAKER_PIN,120); delay(600); noTone(SPEAKER_PIN);
      }
    }
  }
}

// The selectMuxPin function sets the S-pins
// accordingly, given a pin from 0-7.
void selectMuxPin(byte pin, byte pins)
{
  for (int i=0; i<pins; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
