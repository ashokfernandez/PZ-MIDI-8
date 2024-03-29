#include "PZMIDI8.h"
#include "ChannelSettings.h"
#include "Drum.h"

#ifndef CHANNEL_h
#define CHANNEL_h

// The velocity level can be anything between 0-127
// There are 24 rows for the meter on the display (32 - 8 header rows = MAIN_DISPLAY_HEIGHT)
// 127 levels / 24 rows = 5.2 rows per level, round it down to 5
#define SCALE_LEVEL_TO_METER(level) (level/5)  
#define METER_DECAY 10 // How many steps the meter should decay in each display frame

// List of parameters on screen when editing
#define MAX_LINES_OF_PARAMETERS_ON_DISPLAY 3
#define LINE_HEIGHT 8

// The edit markers show little triangles indicating roughly where the threshold 
// and peak are relative to the input signal
#define EDIT_INPUT_METER_MARKER_HEIGHT 3
#define EDIT_INPUT_METER_MARKER_HALF_WIDTH 3
#define EDIT_INPUT_METER_HEIGHT (HEADER_HEIGHT - EDIT_INPUT_METER_MARKER_HEIGHT)
#define EDIT_INPUT_METER_MARKER_TOP (HEADER_HEIGHT - EDIT_INPUT_METER_MARKER_HEIGHT)
#define EDIT_INPUT_METER_MARKER_BOTTOM (HEADER_HEIGHT - 1)

class Channel {
  public:
      // Channel(int8_t channelNumber, const unsigned char* labelBitmap, HelloDrum* drum);
      Channel(int8_t channelNumber, int16_t* inputAddress, const unsigned char* labelBitmap);

      // Methods to render the channel on the screen, either as a column in a list of channels
      // or an edit screen of a single channel
      void drawListView(Adafruit_SSD1306* display, StateController* state);
      void drawEditView(Adafruit_SSD1306* display, StateController* state);

      // Manage a channels settings
      ChannelSettings* getSettings(void);
      void saveSettings(void); // todo, save settings to EEPROM

      // Takes an enum of a parameter and changes it
      void incrementParameter(int8_t parameter);
      void decrementParameter(int8_t parameter);

      
      void update(void);
      
      // Manual testing, should be replaced by a some kind of IO scan (e.g update method)
      // scrap these methods
      void scanForDrumHit(void);
      void sendDrumHitOverMIDI(void);
      int8_t getLevel(void);
      void setLevel(int8_t);



  private:
      int8_t _channelNumber;
      const unsigned char* _labelBitmap;
      int16_t* _inputAddress;
      int8_t _level;

      ChannelSettings* _settings;
      Drum* _drum;

      int8_t _getMeterHeight(void);
};
#endif
