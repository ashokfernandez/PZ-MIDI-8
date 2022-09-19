// Main libraries
#include <hellodrum.h>
#include <SPI.h>
#include <Wire.h>
#include <EasyButton.h>
#include <RotaryEncoder.h>
#include <Adafruit_SSD1306.h>

#include "StateController.h"
#include "Channel.h"

#define NUM_CHANNELS 8 


// Global objects on the heap
extern RotaryEncoder encoder;
extern EasyButton button;
extern Adafruit_SSD1306 display;
extern StateController state;
extern Channels channels;
