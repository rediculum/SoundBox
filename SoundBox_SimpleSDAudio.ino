/*
=============================================================================================
Sound Box 1.0
by ReDiculum (Aug - Sep 2019)
-------------------------------
Sound box like a board to launch samples stored as 8bit full-rate (62500) mono PCM files named from 1.PCM to 10.PCM
from arcade buttons using a demux

SD Card > Arduino pinout:

SCK  (5) -> Pin 9
CS (1)   -> Pin 10 (Chip select)
MOSI (2) -> Pin 11
MISO (7) -> Pin 12

Speaker (sound) -> Pin 9 and GND

CD74HC4067 > Arduino pinout:
S0 -> Pin 7
S1 -> Pin 6
S2 -> Pin 5
S3 -> Pin 4
SIG -> Pin A0

=============================================================================================

https://www.youtube.com/watch?v=OgaeEiHemU4

SimpleSDAudio library for audio playback from SD card
http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio

*/
#include <SimpleSDAudio.h>

#define SPEAKER_PIN 9
#define SD_CHIPSELECT_PIN 10
#define LED_PIN 13 // onboard LED
#define S0_PIN 7
#define S1_PIN 6
#define S2_PIN 5
#define S3_PIN 4 
#define SIG_PIN 0

void setup() {
  Serial.begin(57600); // Set console to 57600 baud
  Serial.println("=== SoundBox based on SSDA ===");  

  pinMode(LED_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

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
  int countImpulse=1;

  /* define a char array with an index size of 6 and convert
  integer "countImpulse" to char "track" using utoa() function
  http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html 
  */
  char track[6]; 
  utoa(countImpulse,track,10);
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
