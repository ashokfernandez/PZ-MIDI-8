// Global objects on the heap
// Include this header in any file that will reference these 

#include "Channel.h"
#include "StateController.h"

#ifndef GLOBAL_OBJECTS_H
#define GLOBAL_OBJECTS_H

extern RotaryEncoder encoder;
extern EasyButton button;
extern Adafruit_SSD1306 display;
extern StateController* state;
extern Channel channels[NUM_CHANNELS];

#endif
