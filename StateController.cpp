// Handles external inputs and what they do to the state of the application. 
// call update to process a round of inputs and state updates
#include "StateController.h"
#include "GlobalObjects.h"

// Value to set the selected channel variable to if no channel is selected
#define NO_CHANNEL_SELECTED -10

void buttonClicked(void) { 
    
}

void buttonLongClicked(void) { 
  switch(state->getCurrentState()) {
    case CHANNEL_LIST:
      state->setCurrentState(CHANNEL_EDIT);
      break;
    case CHANNEL_EDIT:
      state->setCurrentState(CHANNEL_LIST);
      break;
  }
}

StateController::StateController(){  
  _currentState = CHANNEL_LIST;
  _encoderPosition = 0;
  _selectedChannel = -1;
  _encoderLastChanged_ms = 0;
}


void StateController::update(void) {
    // Get latest values from hardware
    encoder.tick();
    button.update();

    // If the encoder moved...
    int newEncoderPosition = encoder.getPosition();
    if (this->_encoderPosition != newEncoderPosition) {
        this->_encoderLastChanged_ms = millis();
        this->_selectChannel();
        this->_encoderPosition = newEncoderPosition;
    }

    // If enough time has elaspsed since the last encoder move, deselect the selected channel
    if (millis() - this->_encoderLastChanged_ms > SELECT_CHANNEL_TIMEOUT_MS) {
      this->_deselectChannel();
    }
}

int8_t StateController::getCurrentState(void) {
  return this->_currentState;
}


void  StateController::setCurrentState(uint8_t state) {
  this->_currentState = state;
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
  this->_selectedChannel = NO_CHANNEL_SELECTED;
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


    
