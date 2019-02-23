#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#define WAV_PIN 9
#define TRIGGER_PIN 2
#define SAMPLE_SELECTOR_PIN A7
#define PITCH_PIN A1

#include <Metro.h>
#include <AltSoftSerial.h>  // Wav Trigger uses this 
#include <wavTrigger.h>

wavTrigger wTrig;

//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET 6  // Connect RST to pin 9
#define PIN_DC    8  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
#define DC_JUMPER 0


  

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

void setup()
{
  pinMode(TRIGGER_PIN, INPUT);
  
  Serial.begin(9600);

  // If the Arduino is powering the WAV Trigger, we should wait for the WAV
  //  Trigger to finish reset before trying to send commands.
  delay(1000);

  
  wTrig.start();
  delay(10);
  wTrig.stopAllTracks();
  wTrig.samplerateOffset(0);

  initOled();
  delay(100);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), onTriggerRise, RISING);

}
int sampleNumber = 1;
int previousSampleNumber = 1;
int pitch = 0;
int newPitch = 0;
int lastTriggerTimeAgo = 100;

void loop() {
  sampleNumber = map(analogRead(SAMPLE_SELECTOR_PIN), 0, 1024, 1, 119); // 0-1024 [529 - 1014]
  previousSampleNumber = sampleNumber;
  
  newPitch = analogRead(PITCH_PIN); // 0-1024 [1021 - 523] -> -32767 - 32767
  newPitch = map(newPitch, 0, 1024, 32767, -32767);
  if(newPitch != pitch){
    wTrig.samplerateOffset(newPitch); 
  }
  pitch = newPitch;
  oled.clear(PAGE);
  if(lastTriggerTimeAgo < 2){
    oled.drawBitmap(groupGraphic(sampleNumber));
    
  }
  
  
//  printTitle("S: " + String(sampleNumber) + "\nP:" + String(pitch/2730), 0); 
  oled.setFontType(0); // 5x7
//  oled.setCursor(0, 10);
//  oled.print(String(sampleNumber));
  oled.setCursor(0,32);
  oled.print(String(sampleNumber) + " " + sampleName(sampleNumber));
  
  oled.display();
  lastTriggerTimeAgo += 1;
  delay(40);
}
void onTriggerRise(){
  triggerSample();
}
void triggerSample(){
  wTrig.trackStop(sampleNumber);
  wTrig.trackPlayPoly(sampleNumber);
  lastTriggerTimeAgo = 0;
}



