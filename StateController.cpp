// Handles external inputs and what they do to the state of the application. 
// call update to process a round of inputs and state updates
#include "StateController.h"
#include "GlobalObjects.h"

void buttonClicked(void) { Serial.println("Button clicked"); }
void buttonLongClicked(void) { Serial.println("Button long pressed"); }

StateController::StateController(){  
  _encoderPosition = 0;
  _selectedChannel = 0;
}


void StateController::update(void) {
    
    encoder.tick(); // just call tick() to check the state.
    button.update();
    
    int newPos = encoder.getPosition();

    if (this->_encoderPosition != newPos) {
        this->_selectChannel();
        this->_encoderPosition = newPos;
    }

//    this->drawChannelList();
}

int8_t StateController::getSelectedChannel(void) {
  return this->_selectedChannel;
}

void StateController::_incrementSelectedChannel(void) {
  this->_selectedChannel++;

  // Wrap around if we exceed the number of channels 
  if(this->_selectedChannel >= NUM_CHANNELS) {
    this->_selectedChannel = 0;
  }
}

void StateController::_decrementSelectedChannel(void) {
  this->_selectedChannel--;

  // Wrap around if we exceed the number of channels 
  if(this->_selectedChannel < 0) {
    this->_selectedChannel = (NUM_CHANNELS-1);
  }
}

void StateController::_selectChannel(void) {
  static int direction;
  direction = (int)encoder.getDirection();
  if(direction > 0) { 
    this->_incrementSelectedChannel(); 
  } else if (direction < 0) { 
    this->_decrementSelectedChannel(); 
  }
  // Serial.println(this->_selectedChannel);
}
    
