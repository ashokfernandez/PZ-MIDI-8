// Handles drawing the display, based on the current state

#include "ViewController.h"
#include "StateController.h"
#include "GlobalObjects.h"

ViewController::ViewController(Adafruit_SSD1306* display, StateController* state) {
    _display = display;
    _state = state;
}

void ViewController::drawDisplay(void) {
  this->_display.clearDisplay();

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

  this->_display.display();
}

void ViewController::_drawChannelList(void) {
  // Tracks which channel is selected 
  bool isSelected = false;

  // Draw all the channels
  for(uint8_t i=0; i<NUM_CHANNELS; i++){
    isSelected = (i == this->_state->getSelectedChannel()) ? true : false;
    channels[i].drawListView(this->_display, i, isSelected);
  }  
}

void ViewController::_drawChannelEdit(void) {
  // To implement
  this->_display.setTextSize(1);
  this->_display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  this->_display.setCursor(0, 0);
  this->_display.println(F("EDIT EDIT EDIT"));
}