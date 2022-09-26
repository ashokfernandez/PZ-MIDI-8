#include "PZMIDI8.h"

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

  // Make sure all lists of the parameters are in the same order as they're defined here
  // don't forget to include the null byte in the max char count!
  #define NUM_PARAMETERS 6
  #define PARAMETER_LABEL_MAX_CHARACTERS 12
  enum EDIT_PARAMETER { NOTE, PEAK, THRESHOLD, CURVE, SCAN, RETRIGGER };
  const char PARAMETER_LABELS[][PARAMETER_LABEL_MAX_CHARACTERS] PROGMEM = { " NOTE      ",  " PEAK      ", " THRESHOLD ", " CURVE     ", " SCAN      ", " RETRIGGER "};  // Spaces so each label is 11 chars long, makes it easy draw the background for the selected label

  // Max min allowed range for each parameter
  #define NOTE_MIN 0
  #define NOTE_MAX 127

  #define PEAK_MIN 1
  #define PEAK_MAX 100

  #define THRESHOLD_MIN 1
  #define THRESHOLD_MAX 100  

  #define CURVE_MIN 0
  #define CURVE_MAX 4

  #define SCAN_MIN 1
  #define SCAN_MAX 10

  #define RETRIGGER_MIN 1
  #define RETRIGGER_MAX 99

  // The area where the value of each parameter is displayed for editing
  #define EDIT_PARAMETER_BOX_LEFT 65
  #define EDIT_PARAMETER_BOX_TOP HEADER_HEIGHT
  #define EDIT_PARAMETER_BOX_WIDTH (SCREEN_WIDTH - EDIT_PARAMETER_BOX_LEFT)
  #define EDIT_PARAMETER_BOX_HEIGHT (SCREEN_HEIGHT - HEADER_HEIGHT)
  #define EDIT_PARAMETER_BOX_CURSOR_X (EDIT_PARAMETER_BOX_LEFT + 8)
  #define EDIT_PARAMETER_BOX_CURSOR_Y (EDIT_PARAMETER_BOX_TOP + 5)

  // Number of EEPROM cells that have to be set to 255 for the channels settings to be considered 
  // empty. If the EEPROM is empty for the channel, the defaul settings will be written there
  #define EEPROM_IS_EMPTY_CHECK 3
  #define EEPROM_EMPTY_VALUE 0xFF


  // Constant strings used to display the current midi note, the octave is drawn dynamically
  // don't forget to include the null byte in the max char count!
  #define NOTE_LABEL_MAX_CHARATERS 3
  #define NOTES_PER_OCTAVE 12
  const char NOTE_LABELS[][NOTE_LABEL_MAX_CHARATERS] PROGMEM = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" };  // Spaces so each label is 9 chars long, makes it easy draw the background for the selected label

  // (int8_t note, int8_t peak, int8_t threshold, int8_t attackScan, int8_t retriggerDelay )
  // Default channel settings
  const int8_t defaultChannelSettings[NUM_CHANNELS][NUM_PARAMETERS] = {
    {35, 100, 10, 1, 10, 30},
    {36, 100, 10, 1, 10, 30},
    {37, 100, 10, 1, 10, 30},
    {38, 100, 10, 1, 10, 30},
    {39, 100, 10, 1, 10, 30},
    {40, 100, 10, 1, 10, 30},
    {41, 100, 10, 1, 10, 30},
    {42, 100, 10, 1, 10, 30}
  };

  class ChannelSettings {
    public: 
      // ChannelSettings(int8_t channelNumber);
      ChannelSettings(int8_t channelNumber);
      int8_t setting[NUM_PARAMETERS];
      
      // Given an index to a parameter, increment or decrement the value. 
      // Handles clipping to the approrate range
      void incrementParameter(int8_t parameter);
      void decrementParameter(int8_t parameter);

      // Load and save settings to EEPROM
      void save(void);

      // Handles drawing a single parameter to display when we're in edit mode
      void drawParameter(Adafruit_SSD1306* display, int8_t parameter, bool editingParameter);
        
    private:
      int8_t _channelNumber;
      int8_t _eepromStartAddress;
      void _initSettingsFromEEPROM(void);
      void _drawNumber(Adafruit_SSD1306* display, int8_t number);
      void _drawNumberWithMs(Adafruit_SSD1306* display, int8_t number);
      void _drawCurve(Adafruit_SSD1306* display, int8_t curve);
      void _modifyParameter(int8_t amount, int8_t parameter);
  };

#endif