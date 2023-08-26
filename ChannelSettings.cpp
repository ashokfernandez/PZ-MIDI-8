#include "ChannelSettings.h"
#include "Utils.h"

#include "Bitmaps/Curves.h"

// pass channel number in instantiation 
// if first EEPROM cell is 0 write defaults
// load EEPROM into settings
// on save, write all values to EEPROM (library will detect if there are any differences)

ChannelSettings::ChannelSettings(int8_t channelNumber) {
  // int8_t setting[] = { note, threshold, peak, attackScan, retriggerDelay };
  // int8_t channelNumber
  // this->setting = setting;
  this->_channelNumber = channelNumber;
  this->_eepromStartAddress = channelNumber * NUM_PARAMETERS;
  this->_initSettingsFromEEPROM();
}

// Settings are stored in EEPROM, using the channel number to calculate the address
// Each channel requires NUM_PARAMETER (6) EEPROM cells to store all the parameters
void ChannelSettings::_initSettingsFromEEPROM(void) {
  bool eepromIsEmpty = true;

  // Check if the first 3 bytes of the channels EEPROM space are 0xFF, if they
  // are we'll assume the channel is empty
  for(uint8_t i = 0; i < EEPROM_IS_EMPTY_CHECK; i++) {
    eepromIsEmpty = eepromIsEmpty && (EEPROM.read(this->_eepromStartAddress + i) == EEPROM_EMPTY_VALUE);
  }

  // If a channel has settings in EEPROM, load these - otherwise write the default 
  // settings from ChannelSettings.h to EEPROM. should only happen once when the code is first run
  if (!eepromIsEmpty) {
    // Load the settings from EEPROM
    for (uint8_t i = 0; i < NUM_PARAMETERS; i++) {
      this->setting[i] = EEPROM.read(this->_eepromStartAddress + i);
    }    
  } else {
    // Write the default values to the object and save them to EEPROM for next time
    for (uint8_t i = 0; i < NUM_PARAMETERS; i++) {
      this->setting[i] = defaultChannelSettings[this->_channelNumber][i];
      EEPROM.put(this->_eepromStartAddress + i, this->setting[i]);
    }
  }
}

int8_t ChannelSettings::get(int8_t parameter) {
  return this->setting[parameter];
}

void ChannelSettings::save(void){   
  // The EEPROM library's update will only actually write if something changed, saving 3.3ms of write time
  // and preserving the ~100,000 write limit of the EEPROM on the chip
  for (uint8_t i = 0; i < NUM_PARAMETERS; i++) {
    EEPROM.update(this->_eepromStartAddress + i, this->setting[i]);
  }
}



void ChannelSettings::incrementParameter(int8_t parameter) {
  this->_modifyParameter(1, parameter);
}

void ChannelSettings::decrementParameter(int8_t parameter) {
  this->_modifyParameter(-1, parameter);
}

void ChannelSettings::_modifyParameter(int8_t amount, int8_t parameter) {
  switch(parameter) {
      case NOTE: { 
          this->setting[NOTE] = clipValue(this->setting[NOTE] + amount, NOTE_MIN, NOTE_MAX);
          break;
      }
      
      // Clip peak from current threshold value up to 100
      case PEAK: {
          this->setting[PEAK] = clipValue(this->setting[PEAK] + amount, this->setting[THRESHOLD], PEAK_MAX);
          break;
      }
      
      // Clip threshold to 1, up to the current peak value
      case THRESHOLD: {
          this->setting[THRESHOLD] = clipValue(this->setting[THRESHOLD] + amount, THRESHOLD_MIN, this->setting[PEAK]);
          break;
      }

      // Clip peak from current threshold value up to 100
      case CURVE: {
          this->setting[CURVE] = clipValue(this->setting[CURVE] + amount, CURVE_MIN, CURVE_MAX, CLIP_WRAP_AROUND);
          break;
      }
      
      case SCAN: {
          this->setting[SCAN] = clipValue(this->setting[SCAN] + amount, SCAN_MIN, SCAN_MAX);
          break;
      }
      
      case RETRIGGER: {
          this->setting[RETRIGGER] = clipValue(this->setting[RETRIGGER] + amount, RETRIGGER_MIN, RETRIGGER_MAX);
          break;
      }    
  };
}

// 
void  ChannelSettings::_drawNumberWithMs(Adafruit_SSD1306* display, int8_t number) {
  display->setTextSize(2);
  display->setCursor(EDIT_PARAMETER_BOX_CURSOR_X, EDIT_PARAMETER_BOX_CURSOR_Y);  
  display->print(number);
  display->println(F("ms"));
}

void  ChannelSettings::_drawNumber(Adafruit_SSD1306* display, int8_t number) {
  display->setTextSize(2);
  display->setCursor(EDIT_PARAMETER_BOX_CURSOR_X, EDIT_PARAMETER_BOX_CURSOR_Y);  
  display->println(number);  
}

int8_t ChannelSettings::getHelloDrumCurveNumber(void) {
  static const int8_t curveMap[NUM_CURVES] = {4, 3, 0, 1, 2};
  return curveMap[this->setting[CURVE]];
}

void  ChannelSettings::_drawCurve(Adafruit_SSD1306* display, int8_t curve, int8_t foregroundColour) {
  display->drawBitmap(EDIT_PARAMETER_BOX_LEFT, EDIT_PARAMETER_BOX_TOP, curveBitmaps[this->setting[CURVE]], 63, 24, foregroundColour);
}

// Handles drawing the selected parameter to the display when we're in edit mode
void ChannelSettings::drawParameter(Adafruit_SSD1306* display, int8_t parameter, bool editingParameter) {
  int8_t foregroundColour;
  
  // If we're editing a parameter, show a solid filled area with inverted text
  // otherwise just draw an outline and show solid text
  if (editingParameter) {
    display->fillRect(EDIT_PARAMETER_BOX_LEFT, EDIT_PARAMETER_BOX_TOP, EDIT_PARAMETER_BOX_WIDTH, EDIT_PARAMETER_BOX_HEIGHT, SSD1306_WHITE);      
    foregroundColour = SSD1306_BLACK;
  } else {
    display->drawRect(EDIT_PARAMETER_BOX_LEFT, EDIT_PARAMETER_BOX_TOP, EDIT_PARAMETER_BOX_WIDTH, EDIT_PARAMETER_BOX_HEIGHT, SSD1306_WHITE);  
    foregroundColour = SSD1306_WHITE;
  }

  display->setTextColor(foregroundColour);  

  switch(parameter) {
    // Turn the midi note number (0-127) into a note (i.e C#) and an octave number
    case NOTE: {
      uint8_t octave = (this->setting[NOTE] / NOTES_PER_OCTAVE);
      uint8_t noteLabelIndex = this->setting[NOTE] % NOTES_PER_OCTAVE;          
      char noteLabelBuffer[NOTE_LABEL_MAX_CHARATERS];          
      
      // Copy the note label from program memory
      strcpy_P(noteLabelBuffer, (char*)NOTE_LABELS[noteLabelIndex]);

      // Draw the note label and octave on the display
      display->setTextSize(2);
      display->setCursor(EDIT_PARAMETER_BOX_LEFT + 8, EDIT_PARAMETER_BOX_TOP + 5);  
      display->print(noteLabelBuffer);
      display->print(F("-"));
      display->print(octave);
      break;
    }

    // Draw number from 1-100
    case PEAK: {
      this->_drawNumber(display, this->setting[PEAK]);
      break;
    }

    
    // Draw number from 1-100
    case THRESHOLD: {
      this->_drawNumber(display, this->setting[THRESHOLD]);
      break;
    }

    // TODO: Draw bitmap of each curve profile to display
    case CURVE: {
      this->_drawCurve(display, this->setting[CURVE], foregroundColour);
      break;
    }
    
    
    // Draw number of milliseconds
    case SCAN: {
      this->_drawNumberWithMs(display, this->setting[SCAN]);
      break;
    }

    // Draw number of milliseconds
    case RETRIGGER: {
      this->_drawNumberWithMs(display, this->setting[RETRIGGER]);  
      break;
    }
          
  };

}

