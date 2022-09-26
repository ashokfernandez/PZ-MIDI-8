// Handles drawing the display, based on the current state

#include "ViewController.h"
#include "StateController.h"
#include "Channel.h"

ViewController::ViewController(Adafruit_SSD1306* display, StateController* state, Channel* channels) {
  this->_display = display;
  this->_state = state;
  this->_channels = channels;
}

void ViewController::drawDisplay(void) {
  this->_display->clearDisplay();

  switch(this->_state->page) {
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

  this->_display->display();
}

void ViewController::_drawChannelList(void) {
  // Tracks which channel is selected 
  bool isSelected = false;

  // Draw all the channels
  for(uint8_t i=0; i<NUM_CHANNELS; i++){
    isSelected = (i == this->_state->selectedChannel) ? true : false;
    this->_channels[i].drawListView(this->_display, this->_state);
  }  
}

void ViewController::_drawChannelEdit(void) {
  // To implement
  this->_channels[this->_state->selectedChannel].drawEditView(this->_display, this->_state);
}