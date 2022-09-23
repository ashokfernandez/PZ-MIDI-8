#include "PZMIDI8.h"
#include "Channel.h"
#include "ChannelSettings.h"
#include "Utils.h"

Channel::Channel(const unsigned char* labelBitmap, ChannelSettings settings) {
  _labelBitmap = labelBitmap; // Label to display at the top of the list view
  _level = 0; // Meter display level
  _settings = settings;
}



void Channel::drawListView(Adafruit_SSD1306 &display, uint8_t channelNumber, AppState appState){   

    // Parameters for channel label
    uint8_t boxStart = channelNumber * CHANNEL_WIDTH;
    uint8_t boxWidth = CHANNEL_WIDTH;
    uint8_t boxTop = 0;
    uint8_t boxHeight = HEADER_HEIGHT;

    // Parameters for the meter
    int8_t meterHeight = this->_getMeterHeight();
    int8_t meterOffset = HEADER_HEIGHT + (MAIN_DISPLAY_HEIGHT - meterHeight);

    // Colours
    uint8_t background = SSD1306_BLACK;
    uint8_t foreground = SSD1306_WHITE;

    // Invert the colours if the channel is currently selected
    if(isSelected){
      background = SSD1306_WHITE;
      foreground = SSD1306_BLACK;
    }
    
    // Draw the label at the top of the channel      
    display.drawBitmap(boxStart, boxTop, _label, CHANNEL_WIDTH, HEADER_HEIGHT, background, foreground);
    
    // If the channel is selected, fill the meter background and draw a smaller meter in the negative space
    if(isSelected){
        display.fillRect(boxStart, HEADER_HEIGHT, boxWidth, MAIN_DISPLAY_HEIGHT, background);  
        display.fillRect(boxStart+1, meterOffset+1, boxWidth-2, meterHeight-1, foreground);
    }
    // Otherwise just fill the enter channel width with the meter
    else {
        display.fillRect(boxStart, meterOffset, boxWidth, meterHeight, foreground);
    }

    // Decay the meter value
    // If the meter has already peaked, start decaying the value so it slowly drops
    if(this->getLevel() > 0) {
      this->setLevel(this->getLevel() - METER_DECAY);
    }

}

void Channel::drawEditView(Adafruit_SSD1306 &display, StateController appState){   
}


int8_t Channel::getLevel(void){
  return _level;
}

void Channel::setLevel(int8_t level){
  // Floor the level to zero incase it goes negative
  _level = level < 0 ? 0 : level;
}

ChannelSettings Channel::getSettings(void){
  return this->settings;
}


int8_t Channel::_getMeterHeight(void){
  // int8_t meterHeight = ;

  // Clip the meter height in case it overshoots
  // if(meterHeight > MAIN_DISPLAY_HEIGHT) {
    // meterHeight = MAIN_DISPLAY_HEIGHT;
  // }
  
  return clipValue(SCALE_LEVEL_TO_METER(getLevel()), 0, MAIN_DISPLAY_HEIGHT, CLIP_CLAMP_VALUE) ;
}

void Channel::incrementParameter(int8_t parameter) {
  this->settings.incrementParameter(parmeter);
}
void Channel::decrementParameter(int8_t parameter) {
    this->settings.decrementParameter(parmeter);
}