#include "PZMIDI8.h"
#include "Channel.h"

Channel::Channel(const unsigned char* bitmapLabel) {
  _label = bitmapLabel; // Label to display at the top of the list view
  _level = 0; // Meter display level
}

void Channel::drawListView(Adafruit_SSD1306 &display, uint8_t channelIndex, bool isSelected){   
    // Parameters for channel label
    uint8_t boxStart = channelIndex * CHANNEL_WIDTH;
    uint8_t boxWidth = CHANNEL_WIDTH;
    uint8_t boxTop = 0;
    uint8_t boxHeight = HEADER_HEIGHT;

    // Parameters for the meter
    uint8_t meterHeight = this->_getMeterHeight();
    uint8_t meterOffset = HEADER_HEIGHT + (MAIN_DISPLAY_HEIGHT - meterHeight);

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


int8_t Channel::getLevel(void){
  return _level;
}

void Channel::setLevel(int8_t level){
  // Floor the level to zero incase it goes negative
  if(level < 0) {
    level = 0;
  }
  
  _level = level;
}


int8_t Channel::_getMeterHeight(void){
  int8_t meterHeight = SCALE_LEVEL_TO_METER(getLevel());
  
  // Clip the meter height in case it overshoots
  if(meterHeight > MAIN_DISPLAY_HEIGHT) {
    meterHeight = MAIN_DISPLAY_HEIGHT;
  }
  
  return meterHeight;
}
