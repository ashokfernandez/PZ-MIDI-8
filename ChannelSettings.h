#include "PZMIDI8.h"

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

  // Make sure all lists of the parameters are in the same order as they're defined here
  // don't forget to include the null byte in the max char count!
  #define NUM_PARAMETERS 5
  #define PARAMETER_LABEL_MAX_CHARACTERS 10
  enum EDIT_PARAMETER { NOTE, THRESHOLD, PEAK, ATTACK_SCAN, RETRIGGER_DELAY };
  const char PARAMETER_LABELS[][PARAMETER_LABEL_MAX_CHARACTERS] PROGMEM = { "NOTE     ", "THRESHOLD", "PEAK     ", "SCAN     ", "RETRIGGER"};  // Spaces so each label is 9 chars long, makes it easy draw the background for the selected label

  // Max min allowed range for each parameter
  #define NOTE_MIN 0
  #define NOTE_MAX 127

  #define THRESHOLD_MIN 1
  #define THRESHOLD_MAX 100

  #define PEAK_MIN 1
  #define PEAK_MAX 100

  #define ATTACK_SCAN_MIN 1
  #define ATTACK_SCAN_MAX 10

  #define RETRIGGER_DELAY_MIN 1
  #define RETRIGGER_DELAY_MAX 99

  // The area where the value of each parameter is displayed for editing
  #define EDIT_PARAMETER_BOX_LEFT 63
  #define EDIT_PARAMETER_BOX_TOP HEADER_HEIGHT
  #define EDIT_PARAMETER_BOX_WIDTH (SCREEN_WIDTH - EDIT_PARAMETER_BOX_LEFT)
  #define EDIT_PARAMETER_BOX_HEIGHT (SCREEN_HEIGHT - HEADER_HEIGHT)



  // Constant strings used to display the current midi note, the octave is drawn dynamically
  // don't forget to include the null byte in the max char count!
  #define NOTE_LABEL_MAX_CHARATERS 3
  #define NOTES_PER_OCTAVE 12
  const char NOTE_LABELS[][NOTE_LABEL_MAX_CHARATERS] PROGMEM = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" };  // Spaces so each label is 9 chars long, makes it easy draw the background for the selected label

  class ChannelSettings {
    public: 
      // ChannelSettings(int8_t channelNumber);
      ChannelSettings(int8_t* settings);
      int8_t* setting;
      
      // Given an index to a parameter, increment or decrement the value. 
      // Handles clipping to the approrate range
      void incrementParameter(int8_t parameter);
      void decrementParameter(int8_t parameter);

      // Load and save settings to EEPROM
      // void loadSettingsFromEEPROM(void);
      // void saveSettingsToEEPROM(void);

      // Handles drawing a single parameter to display when we're in edit mode
      void drawParameter(Adafruit_SSD1306* display, int8_t parameter, bool editingParameter);
        
    private:
      void _drawNumber(Adafruit_SSD1306* display, int8_t number);
      void _drawNumberWithMs(Adafruit_SSD1306* display, int8_t number);
      void _modifyParameter(int8_t amount, int8_t parameter);
  };

#endif