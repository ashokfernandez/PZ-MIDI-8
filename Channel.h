// #include "PZMIDI8.h"
// #include "GlobalObjects.h"

#include "ChannelSettings.h"

#ifndef CHANNEL_h
#define CHANNEL_h

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

// Bitmap labels for the numbers on the channel list
#include "BitmapLabels.h"

class Channel {
public:
    Channel(const unsigned char* labelBitmap, ChannelSettings* settings);

    // Methods to render the channel on the screen, either as a column in a list of channels
    // or an edit screen of a single channel
    void drawListView(Adafruit_SSD1306 &display, uint8_t channelIndex, bool isSelected);
    void drawEdittView(Adafruit_SSD1306 &display, AppState appState);

    // Manage a channels settings
    ChannelSettings* getSettings(void);
    void loadSettings(void); // todo, load settings to EEPROM
    void saveSettings(void); // todo, save settings to EEPROM

    // Takes an enum of a parameter and changes it
    void incrementParameter(int8_t parameter);
    void decrementParameter(int8_t parameter);

    // Manual testing, should be replaced by a some kind of IO scan
    int8_t getLevel(void);
    void setLevel(int8_t);



private:
    ChannelSettings* _settings;
    const unsigned char* _labelBitmap;
    int8_t _level;

    int8_t _getMeterHeight(void);
};
#endif
