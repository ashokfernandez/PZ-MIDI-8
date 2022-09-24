// Handles external inputs and what they do to the state of the application. 
// call update to process a round of inputs and state updates
#include "StateController.h"
#include "GlobalObjects.h"
#include "Utils.h"

StateController::StateController(void){  
  page = CHANNEL_LIST;
  selectedChannel = 0;
  editingParameter = false;
  selectedParameter = 0;
  switchHoldTime_ms = 0;
}

// Public Methods

void StateController::buttonClicked(void) {
  
  switch(this->page){
  
    // If we're on the channel list and a channel is selected, clicking will open the settings
    case CHANNEL_LIST:
      if (this->selectedChannel != NO_CHANNEL_SELECTED) {
        this->page = CHANNEL_EDIT;
      }
      break;
    
    // If we're on the edit screen, clicking will toggle between editing a parameter and 
    // scrolling through them
    case CHANNEL_EDIT:
      this->editingParameter = !this->editingParameter;
      break;
    
    // If we're currently saving, clicking does nothing
    case SAVE_IN_PROGRESS:
      break;
  }
}

void StateController::buttonHeldFor_ms(uint16_t time_ms) {
  switch(this->page){
    case CHANNEL_LIST:
      break;
    case CHANNEL_EDIT:
      break;
    case SAVE_IN_PROGRESS:
      break;
  }
}

void StateController::buttonLongClicked(void) {
  switch(this->page){
    
    // If we're on the channel list and we somehow see a long press (this shouldn't happen) 
    // we should edit the selected channel if one is selected
    case CHANNEL_LIST:
      if (this->selectedChannel != NO_CHANNEL_SELECTED) {
        this->page = CHANNEL_EDIT;
      }
      break;

    // If we're editing a parameter value and a long press is detected, go back to selecting parameters
    // If we're no editing a parameter value, we should save the channel settings to EEPORM
    case CHANNEL_EDIT:
      if(this->editingParameter) {
        this->editingParameter = false;
      } else {
        // TODO
        // IMPLEMENT SAVE LOGIC HERE
        this->page = CHANNEL_LIST;
      }
      break;

    case SAVE_IN_PROGRESS:
      break;
  }
}

void StateController::encoderIncremented(void) {
  switch(this->page){
    // If we're on the channel list and the encoder is incremented, select the next channel
    case CHANNEL_LIST:
      this->_incrementSelectedChannel();
      break;
    
    // If we're on the edit page and the encoder is incremented, check if we're editing a 
    // parameter or selecting which parameter to edit then increment accordingly
    case CHANNEL_EDIT:
      if (this->editingParameter) {
        this->_incrementSelectedParameterValue();
      } else {
        this->_incrementSelectedParameter();
      }
      break;
    
    // If we're saving, do nothing
    case SAVE_IN_PROGRESS:
      break;
  }
}

void StateController::encoderDecremented(void) {
  switch(this->page){
    // If we're on the channel list and the encoder is decremented, select the previous channel
    case CHANNEL_LIST:
      this->_decrementSelectedChannel();
      break;

    // If we're on the edit page and the encoder is incremented, check if we're editing a 
    // parameter or selecting which parameter to edit then increment accordingly
    case CHANNEL_EDIT:
      if (this->editingParameter) {
        this->_decrementSelectedParameterValue();
      } else {
        this->_decrementSelectedParameter();
      }
      break;
    
    // If we're saving, do nothing
    case SAVE_IN_PROGRESS:
      break;
  }
}


// ------// ------// ------// ------// ------// ------// ------


// Private methods

// Channel selection
void StateController::_deselectAllChannels(void) {
  this->selectedChannel = NO_CHANNEL_SELECTED;
}

void StateController::_incrementSelectedChannel(void) {
  this->selectedChannel = clipValue(this->selectedChannel+1, 0, NUM_CHANNELS - 1, CLIP_WRAP_AROUND);
}

void StateController::_decrementSelectedChannel(void) {
  this->selectedChannel = clipValue(this->selectedChannel-1, 0, NUM_CHANNELS - 1, CLIP_WRAP_AROUND);
}


// Parameter selection
void StateController::_incrementSelectedParameter(void) {
  this->selectedParameter = clipValue(this->selectedParameter+1, 0, NUM_PARAMETERS - 1, CLIP_WRAP_AROUND);
}

void StateController::_decrementSelectedParameter(void) {
  this->selectedParameter = clipValue(this->selectedParameter-1, 0, NUM_PARAMETERS - 1, CLIP_WRAP_AROUND);
}

void StateController::_deselectAllParameters(void) {
  this->selectedParameter = NO_PARAMETERS_SELECTED;
}


// Parameter value change
void StateController::_incrementSelectedParameterValue(void) {
  channels[this->selectedChannel].incrementParameter(this->selectedParameter);
}

void StateController::_decrementSelectedParameterValue(void) {
  channels[this->selectedChannel].decrementParameter(this->selectedParameter);
}






