// Handles drawing the display, based on the current state

#include "PZMIDI8.h"
#include "ViewController.h"
#include "StateController.h"
#include "GlobalObjects.h"

ViewController::ViewController() {
    
}

void ViewController::drawDisplay(void) {
  display.clearDisplay();

  switch(state->getCurrentState()) {
    case CHANNEL_LIST:
      this->_drawChannelList();
      break;
    case CHANNEL_EDIT:
      // to implement
      this->_drawChannelEdit();
      break;
    case SAVE_IN_PROGRESS:
      // to implement
      break;
  }

  display.display();
}

void ViewController::_drawChannelList(void) {
  // Tracks which channel is selected 
  bool isSelected = false;

  // Draw all the channels
  for(uint8_t i=0; i<NUM_CHANNELS; i++){
    isSelected = (i == state->getSelectedChannel()) ? true : false;
    channels[i].drawListView(display, i, isSelected);
  }  
}

void ViewController::_drawChannelEdit(void) {
  // To implement
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.println(F("EDIT EDIT EDIT"));
}