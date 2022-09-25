#include "ChannelSettings.h"
#include "Utils.h"

ChannelSettings::ChannelSettings(int8_t* setting) {
  // int8_t setting[] = { note, threshold, peak, attackScan, retriggerDelay };
  this->setting = setting;
  // this->note = note;
  // this->threshold = threshold; //1-100 cant be higher than max, is the min value needed before a hit is registered,  
  // this->setting[PEAK] = peak; // 1-100, 100 means all ADC values over 1000 register at 127 velocity 
  // this->attackScan = attackScan; //1- 10 how long the controller waits to see how hard the pad was hit,  
  // this->retriggerDelay = retriggerDelay; //1-100 ms before another hit can be registered,  
}

void ChannelSettings::incrementParameter(int8_t parameter) {
  this->_modifyParameter(1, parameter);
}

void ChannelSettings::decrementParameter(int8_t parameter) {
  this->_modifyParameter(-1, parameter);
}

void ChannelSettings::_modifyParameter(int8_t amount, int8_t parameter) {
  switch(parameter) {
      case NOTE:
          this->setting[NOTE] = clipValue(this->setting[NOTE] + amount, NOTE_MIN, NOTE_MAX, CLIP_CLAMP_VALUE);
          break;
      
      // Clip threshold to 1, up to the current peak value
      case THRESHOLD:
          this->setting[THRESHOLD] = clipValue(this->setting[THRESHOLD] + amount, THRESHOLD_MIN, this->setting[PEAK], CLIP_CLAMP_VALUE);
          break;
      
      // Clip peak from current threshold value up to 100
      case PEAK:
          this->setting[PEAK] = clipValue(this->setting[PEAK] + amount, this->setting[THRESHOLD], PEAK_MAX, CLIP_CLAMP_VALUE);
          break;
      
      case ATTACK_SCAN:
          this->setting[ATTACK_SCAN] = clipValue(this->setting[ATTACK_SCAN] + amount, ATTACK_SCAN_MIN, ATTACK_SCAN_MAX, CLIP_CLAMP_VALUE);
          break;
      
      case RETRIGGER_DELAY:
          this->setting[RETRIGGER_DELAY] = clipValue(this->setting[RETRIGGER_DELAY] + amount, RETRIGGER_DELAY_MIN, RETRIGGER_DELAY_MAX, CLIP_CLAMP_VALUE);
          break;    
  };
}


