// Handles external inputs and what they do to the state of the application. 
// call update to process a round of inputs and state updates
#include "StateController.h"
#include "GlobalObjects.h"

void buttonClicked(void) { Serial.println("Button clicked"); }
void buttonLongClicked(void) { Serial.println("Button long pressed"); }

StateController::StateController(){  
  _encoderPosition = 0;
  _selectedChannel = -1;
  _encoderLastChanged_ms = 0;
}


void StateController::update(void) {
    // Get latest values from hardware
    encoder.tick();
    button.update();

    // If the encoder moved...
    int newPos = encoder.getPosition();
    if (this->_encoderPosition != newPos) {
        this->_encoderLastChanged_ms = millis();
        this->_selectChannel();
        this->_encoderPosition = newPos;
    }

    if (millis() - this->_encoderLastChanged_ms > SELECT_CHANNEL_TIMEOUT_MS) {
      this->_deselectChannel();
    }
}

int8_t StateController::getSelectedChannel(void) {
  return this->_selectedChannel;
}

void StateController::_selectChannel(void) {
  int direction;
  direction = (int)encoder.getDirection();
  
  if(direction > 0) { 
    this->_incrementSelectedChannel(); 
  } else if (direction < 0) { 
    this->_decrementSelectedChannel(); 
  }
}

void StateController::_deselectChannel(void) {
  this->_selectedChannel = -1;
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


    
