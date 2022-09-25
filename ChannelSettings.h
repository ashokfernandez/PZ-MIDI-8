#include <Arduino.h>

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

  // Make sure all lists of the parameters are in the same order as they're defined here
  #define NUM_PARAMETERS 5
  #define PARAMETER_LABEL_MAX_CHARACTERS 10
  enum EDIT_PARAMETER { NOTE, THRESHOLD, PEAK, ATTACK_SCAN, RETRIGGER_DELAY };
  const char PARAMETER_LABELS[][PARAMETER_LABEL_MAX_CHARACTERS] PROGMEM = { "NOTE     ", "THRESHOLD", "PEAK     ", "SCAN     ", "RETRIGGER"};

  #define NOTE_MIN 0
  #define NOTE_MAX 127

  #define THRESHOLD_MIN 1
  #define THRESHOLD_MAX 100

  #define PEAK_MIN 1
  #define PEAK_MAX 100

  #define ATTACK_SCAN_MIN 1
  #define ATTACK_SCAN_MAX 10

  #define RETRIGGER_DELAY_MIN 1
  #define RETRIGGER_DELAY_MAX 100



  class ChannelSettings {
    public: 
      // ChannelSettings(int8_t channelNumber);
      ChannelSettings(int8_t* settings);

      // Given an index to a parameter, increment or decrement the value. 
      // Handles clipping to the approrate range
      void incrementParameter(int8_t parameter);
      void decrementParameter(int8_t parameter);

      // Load and save settings to EEPROM
      // void loadSettingsFromEEPROM(void);
      // void saveSettingsToEEPROM(void);
      int8_t* setting;
      // int8_t note;
      // int8_t threshold;
      // int8_t peak;
      // int8_t attackScan;
      // int8_t retriggerDelay;
        
    private:
      void _modifyParameter(int8_t amount, int8_t parameter);
  };

#endif