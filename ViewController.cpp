// Handles external inputs and what they do to the state of the display. 
#include "ViewController.h"

ViewController::ViewController() {
    
}

void ViewController::drawChannelList(void) {
  display.clearDisplay();
  bool isSelected = false;

  for(uint8_t i=0; i<NUM_CHANNELS; i++){
    isSelected = (i == this->_selectedChannel) ? true : false;
    this->_channel[i].drawListView(display, i, isSelected);
  }

  display.display();
}
