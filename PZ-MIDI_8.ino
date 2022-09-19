#include "PZMIDI8.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Channel.h"
#include "StateController.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Setup hardware objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(ROTARY_DATA_PIN, ROTARY_CLOCK_PIN, RotaryEncoder::LatchMode::FOUR3);
EasyButton button(ROTARY_PUSH_BUTTON_PIN);

// ISRs for external interrupts
void _encoderISR(void) { encoder.tick(); }
void _buttonISR(void) { button.read(); }

// Single global instances in heap mem of key objects
StateController state();
Channel channels[] = {
  Channel(bitmapLabel_channel1), 
  Channel(bitmapLabel_channel2), 
  Channel(bitmapLabel_channel3), 
  Channel(bitmapLabel_channel4), 
  Channel(bitmapLabel_channel5), 
  Channel(bitmapLabel_channel6), 
  Channel(bitmapLabel_channel7), 
  Channel(bitmapLabel_channel8)
};



void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Attach interrupt for rotary encoder
  attachInterrupt(digitalPinToInterrupt(ROTARY_DATA_PIN), _encoderISR, CHANGE);

  // Attach callbacks and interrupt to push button switch
  button.begin();
  button.onPressed(buttonClicked);
  button.onPressedFor(LONG_PRESS_DURATION_MS, buttonLongClicked);
  button.enableInterrupt(_buttonISR);

  // Reset display buffer
  display.clearDisplay();
}

void loop() {
    for(int8_t i=0; i<NUM_CHANNELS; i++){
     channels[i].setLevel(i*17);
    }
    
    for(int8_t i=0; i<25; i++){
      state.update();
      delay(10);
    }

}
