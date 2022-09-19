// Handles external inputs and what they do to the state of the display. 
#include "PZMIDI8.h"
#include "ViewController.h"
#include "GlobalObjects.h"

ViewController::ViewController() {
    
}

void ViewController::drawChannelList(void) {
  display.clearDisplay();
  bool isSelected = false;

  for(uint8_t i=0; i<NUM_CHANNELS; i++){
    isSelected = (i == state->getSelectedChannel()) ? true : false;
    channels[i].drawListView(display, i, isSelected);
  }

  display.display();
}
