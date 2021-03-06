#include <Encoder.h>
#include <HID-Project.h>
#include <HID-Settings.h>

//This sketch programs a Pro Micro (5V, 16 MHz) to read a rotary encoder (vol up/down)
//and its key press (media play/pause) and sends it to the host computer.
//Encoder library: https://github.com/John-Lluch/Encoder
//USB HID library: https://github.com/NicoHood/HID

#define pinA 11
#define pinB 10
#define pinP 9
Encoder encoder(pinA, pinB, pinP);

const byte ledPin = 2;
int delta;
int accumulator;

//Scale factor, because encoder sends around 4 impulses per haptic click when turning. Tune by feel, higher factor means slower response.
int encoderScaleFactor = 4;

void setup() {  
  EncoderInterrupt.begin(&encoder);
  pinMode(ledPin, OUTPUT);
  
  //HID type must be Consumer (not keyboard) to send media keys.
  Consumer.begin();
}

void loop() {
  
  delta = encoder.delta();

   if (delta > 0)  {
      accumulator++;
    }
    
   if (delta < 0)  {
     accumulator--;
    }

  if (accumulator > encoderScaleFactor) {
    Consumer.write(MEDIA_VOLUME_UP);
    accumulator = 0;
  }

  if (accumulator < -encoderScaleFactor) {
    Consumer.write(MEDIA_VOLUME_DOWN); 
    accumulator = 0;
  }

  //encoder.button() returns a boolean
  if (encoder.button()) {
    Consumer.write(MEDIA_PLAY_PAUSE); 
    delay(500);
  }
