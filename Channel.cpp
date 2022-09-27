#include "Channel.h"

#include "StateController.h"
#include "Utils.h"

Channel::Channel(const unsigned char* labelBitmap, int8_t channelNumber) {
  this->_labelBitmap = labelBitmap; // Label to display at the top of the list view
  this->_channelNumber = channelNumber;
  this->_level = 0; // Meter display level
  this->_settings = new ChannelSettings(channelNumber);
  this->_drum = new HelloDrum(channelNumber);
}

void Channel::scanForDrumHit(void){
  this->_drum->singlePiezoMUX(this->_settings->setting[PEAK], this->_settings->setting[THRESHOLD], this->_settings->setting[SCAN], this->_settings->setting[RETRIGGER]);
}

void Channel::sendDrumHitOverMIDI(void){
  if(this->_drum->hit) {
    // TODO replace with sending actual midi messages
    Serial.print(this->_channelNumber);
    Serial.println(" hit detected!");
    Serial.println(this->_drum->velocity);
  }
}

void Channel::drawListView(Adafruit_SSD1306* display, StateController* state){   

    // Parameters for channel label
    uint8_t xOffset = this->_channelNumber * CHANNEL_WIDTH;
    uint8_t width = CHANNEL_WIDTH;
    uint8_t yOffset = 0;

    // Parameters for the meter
    int8_t meterHeight = this->_getMeterHeight();
    int8_t meterOffset = HEADER_HEIGHT + (MAIN_DISPLAY_HEIGHT - meterHeight);

    // Colours
    uint8_t background = SSD1306_BLACK;
    uint8_t foreground = SSD1306_WHITE;
    bool isSelected = (this->_channelNumber == state->selectedChannel);

    // Invert the colours if the channel is currently selected
    if(isSelected){
      background = SSD1306_WHITE;
      foreground = SSD1306_BLACK;
    }
    
    // Draw the label at the top of the channel      
    display->drawBitmap(xOffset, yOffset, this->_labelBitmap, CHANNEL_WIDTH, HEADER_HEIGHT, background, foreground);
    
    // If the channel is selected, fill the meter background and draw a smaller meter in the negative space
    if(isSelected){
        display->fillRect(xOffset, HEADER_HEIGHT, width, MAIN_DISPLAY_HEIGHT, background);  
        display->fillRect(xOffset+1, meterOffset+1, width-2, meterHeight-1, foreground);
    }
    // Otherwise just fill the enter channel width with the meter
    else {
        display->fillRect(xOffset, meterOffset, width, meterHeight, foreground);
    }

    // Decay the meter value
    // If the meter has already peaked, start decaying the value so it slowly drops
    if(this->getLevel() > 0) {
      this->setLevel(this->getLevel() - METER_DECAY);
    }

}

void Channel::drawEditView(Adafruit_SSD1306* display, StateController* state){
  
  /* ------------------------------------------------------------------------
  // Draw the input level meter, and markers for threshold and peak
  ---------------------------------------------------------------------------*/

  // Take the threshold and peak values (1-100) and remap them to width of the screen (0-127), accounting for the width of the meter market so we
  // don't try draw half the meter marker off the screen at low and high values
  int8_t thresholdMarkerCenterX = map(this->_settings->setting[THRESHOLD], THRESHOLD_MIN, THRESHOLD_MAX, EDIT_INPUT_METER_MARKER_HALF_WIDTH - 1, SCREEN_WIDTH - 1 - EDIT_INPUT_METER_MARKER_HALF_WIDTH);
  int8_t peakMarkerCenterX = map(this->_settings->setting[PEAK], PEAK_MIN, PEAK_MAX, EDIT_INPUT_METER_MARKER_HALF_WIDTH - 1, SCREEN_WIDTH - 1 - EDIT_INPUT_METER_MARKER_HALF_WIDTH);
   
  int8_t thresholdMarkerX0 = thresholdMarkerCenterX - EDIT_INPUT_METER_MARKER_HALF_WIDTH;
  int8_t thresholdMarkerX1 = thresholdMarkerCenterX;
  int8_t thresholdMarkerX2 = thresholdMarkerCenterX + EDIT_INPUT_METER_MARKER_HALF_WIDTH;

  int8_t peakMarkerX0 = peakMarkerCenterX - EDIT_INPUT_METER_MARKER_HALF_WIDTH;
  int8_t peakMarkerX1 = peakMarkerCenterX;
  int8_t peakMarkerX2 = peakMarkerCenterX + EDIT_INPUT_METER_MARKER_HALF_WIDTH;
  
  // Draw the markers
  display->fillTriangle(thresholdMarkerX0, EDIT_INPUT_METER_MARKER_BOTTOM, thresholdMarkerX1, EDIT_INPUT_METER_MARKER_TOP, thresholdMarkerX2, EDIT_INPUT_METER_MARKER_BOTTOM, SSD1306_WHITE);
  display->fillTriangle(peakMarkerX0, EDIT_INPUT_METER_MARKER_BOTTOM, peakMarkerX1, EDIT_INPUT_METER_MARKER_TOP, peakMarkerX2, EDIT_INPUT_METER_MARKER_BOTTOM, SSD1306_WHITE);
  
  // Draw the meter
  // TODO hook this up to the ADC value for the channel
  display->fillRect(EDIT_INPUT_METER_MARKER_HALF_WIDTH, 0, SCREEN_WIDTH, EDIT_INPUT_METER_HEIGHT, SSD1306_WHITE);
  
  /* ------------------------------------------------------------------------
  // Draw the list of channel parameters, highlighting which one is selected
  ---------------------------------------------------------------------------*/

  // Draw the list of labels showing what can be edited
  char parameterLabelBuffer[PARAMETER_LABEL_MAX_CHARACTERS];
  display->setTextSize(1);

  // Choose which three labels to display based on the currently selected parameter.
  // The selected parameter should always be in the middle, unless it's the first or last one
  uint8_t topLabelIndex = state->selectedParameter;
  if (topLabelIndex > 0 && topLabelIndex < (NUM_PARAMETERS - 1)) {
    topLabelIndex -= 1;
  } else if (topLabelIndex == (NUM_PARAMETERS - 1)) {
    // Clamps the last item to the bottom instead of trying to center it
    topLabelIndex -= 2;
  }
  
  for (uint8_t i = 0; i < MAX_LINES_OF_PARAMETERS_ON_DISPLAY; i++) {
    // Parameter labels need to be copied from program memory into a buffer before 
    // printing to the display, otherwise the memory corrupts and you get nothing but spaghetti on screen
    strcpy_P(parameterLabelBuffer, (char*)PARAMETER_LABELS[i + topLabelIndex]);
  
    // If the parameter is selected, invert the colours
    if (i + topLabelIndex == state->selectedParameter) {
      display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
    
    // Draw the label 
    display->setCursor(0, HEADER_HEIGHT + (i * LINE_HEIGHT));  
    display->print(parameterLabelBuffer);
  }
  
  /* ------------------------------------------------------------------------
  // Draw the value of the currently selected parameter, highlighting the display
  // area if we're editing that parameter
  ---------------------------------------------------------------------------*/
  this->_settings->drawParameter(display, state->selectedParameter, state->editingParameter);
}


int8_t Channel::getLevel(void){
  return _level;
}

void Channel::setLevel(int8_t level){
  // Floor the level to zero incase it goes negative
  _level = level < 0 ? 0 : level;
}

ChannelSettings* Channel::getSettings(void){
  return this->_settings;
}

void Channel::saveSettings(void) {
  this->_settings->save();
}


int8_t Channel::_getMeterHeight(void){
  return clipValue(SCALE_LEVEL_TO_METER(getLevel()), 0, MAIN_DISPLAY_HEIGHT);
}

void Channel::incrementParameter(int8_t parameter) {
  this->_settings->incrementParameter(parameter);
}
void Channel::decrementParameter(int8_t parameter) {
  this->_settings->decrementParameter(parameter);
}