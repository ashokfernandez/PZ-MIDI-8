#ifndef Channel_h
#define Channel_h
#include "Arduino.h" 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define NUM_CHANNELS 8 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// The SSD1306 OLED Display used has a top header row that displays in yellow, and 
// a main section that displays in blue
#define HEADER_HEIGHT 8 
#define MAIN_DISPLAY_HEIGHT (SCREEN_HEIGHT-HEADER_HEIGHT) 
#define CHANNEL_WIDTH (SCREEN_WIDTH/NUM_CHANNELS) // Display width of each channel

// The velocity level can be anything between 0-127
// There are 24 rows for the meter on the display (32 - 8 header rows = MAIN_DISPLAY_HEIGHT)
// 127 levels / 24 rows = 5.2 rows per level, round it down to 5
#define SCALE_LEVEL_TO_METER(level) (level/5)  
#define METER_DECAY 10 // How many steps the meter should decay in each display frame


// Bitmaps for the channel label (e.g 1 2 3 4 5 6 7 8)

// 'Channel 1', 16x8px
const unsigned char bitmapLabel_channel1 [] PROGMEM = {
  0xff, 0xff, 0xff, 0x7f, 0xfe, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xfe, 0x3f, 0xff, 0xff
};
// 'Channel 2', 16x8px
const unsigned char bitmapLabel_channel2 [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x7f, 0xfd, 0xbf, 0xff, 0x7f, 0xfe, 0xff, 0xfd, 0xff, 0xfc, 0x3f, 0xff, 0xff
};
// 'Channel 3', 16x8px
const unsigned char bitmapLabel_channel3 [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x7f, 0xfd, 0xbf, 0xff, 0x7f, 0xfd, 0xbf, 0xfd, 0xbf, 0xfe, 0x7f, 0xff, 0xff
};
// 'Channel 4', 16x8px
const unsigned char bitmapLabel_channel4 [] PROGMEM = {
  0xff, 0xff, 0xff, 0x7f, 0xfe, 0x7f, 0xfd, 0x7f, 0xfc, 0x3f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff
};
// 'Channel 5', 16x8px
const unsigned char bitmapLabel_channel5 [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x3f, 0xfd, 0xff, 0xfc, 0x7f, 0xff, 0xbf, 0xff, 0xbf, 0xfc, 0x7f, 0xff, 0xff
};
// 'Channel 6', 16x8px
const unsigned char bitmapLabel_channel6 [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x7f, 0xfd, 0xff, 0xfc, 0x7f, 0xfd, 0xbf, 0xfd, 0xbf, 0xfe, 0x7f, 0xff, 0xff
};
// 'Channel 7', 16x8px
const unsigned char bitmapLabel_channel7 [] PROGMEM = {
  0xff, 0xff, 0xfc, 0x7f, 0xff, 0xbf, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff
};
// 'Channel 8', 16x8px
const unsigned char bitmapLabel_channel8 [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x7f, 0xfd, 0xbf, 0xfe, 0x7f, 0xfd, 0xbf, 0xfd, 0xbf, 0xfe, 0x7f, 0xff, 0xff
};

class Channel {
public:
    Channel(const unsigned char* bitmapLabel);

    // Draws the channel as a label and a meter on the display
    void drawListView(Adafruit_SSD1306 &display, uint8_t channelIndex, bool isSelected);
    
    int8_t getLevel(void);
    void setLevel(int8_t);
private:
    const unsigned char* _label;
    int8_t _level;

    int8_t _getMeterHeight(void);
};
#endif
