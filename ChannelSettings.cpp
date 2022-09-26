#include "ChannelSettings.h"
#include "Utils.h"

// pass channel number in instantiation 
// if first EEPROM cell is 0 write defaults
// load EEPROM into settings
// on save, write all values to EEPROM (library will detect if there are any differences)

ChannelSettings::ChannelSettings(int8_t channelNumber) {
  // int8_t setting[] = { note, threshold, peak, attackScan, retriggerDelay };
  // int8_t channelNumber
  // this->setting = setting;
  this->_readSettingsFromEEPROM(channelNumber);
  // this->note = note;
  // this->threshold = threshold; //1-100 cant be higher than max, is the min value needed before a hit is registered,  
  // this->setting[PEAK] = peak; // 1-100, 100 means all ADC values over 1000 register at 127 velocity 
  // this->attackScan = attackScan; //1- 10 how long the controller waits to see how hard the pad was hit,  
  // this->retriggerDelay = retriggerDelay; //1-100 ms before another hit can be registered,  
}

void ChannelSettings::_readSettingsFromEEPROM(int8_t channelNumber) {
  
  for (uint8_t i = 0; i < NUM_PARAMETERS; i++) {
    this->setting[i] = defaultChannelSettings[channelNumber][i];
  }
}


void loadSettingsFromEEPROM(void){ 

}

void saveSettingsToEEPROM(void){ 

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
          this->setting[CURVE] = clipValue(this->setting[CURVE] + amount, CURVE_MIN, CURVE_MAX);
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

// Handles drawing the selected parameter to the display when we're in edit mode
void ChannelSettings::drawParameter(Adafruit_SSD1306* display, int8_t parameter, bool editingParameter) {
  
  // If we're editing a parameter, show a solid filled area with inverted text
  // otherwise just draw an outline and show solid text
  if (editingParameter) {
    display->fillRect(EDIT_PARAMETER_BOX_LEFT, EDIT_PARAMETER_BOX_TOP, EDIT_PARAMETER_BOX_WIDTH, EDIT_PARAMETER_BOX_HEIGHT, SSD1306_WHITE);      
    display->setTextColor(SSD1306_BLACK);
  } else {
    display->drawRect(EDIT_PARAMETER_BOX_LEFT, EDIT_PARAMETER_BOX_TOP, EDIT_PARAMETER_BOX_WIDTH, EDIT_PARAMETER_BOX_HEIGHT, SSD1306_WHITE);  
    display->setTextColor(SSD1306_WHITE);
  }

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
      this->_drawNumber(display, this->setting[CURVE]);
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

