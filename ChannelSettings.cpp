#include "ChannelSettings.h"
#include "Utils.h"

ChannelSettings::ChannelSettings(uint8_t peak, uint8_t threshold, uint8_t attackScan, uint8_t retriggerDelay, uint8_t note) {
    settings[PEAK] = peak; // 1-100, 100 means all ADC values over 1000 register at 127 velocity 
    settings[THRESHOLD] = threshold; //1-100 cant be higher than max, is the min value needed before a hit is registered,  
    settings[ATTACK_SCAN] = attackScan; //1- 10 how long the controller waits to see how hard the pad was hit,  
    settings[RETRIGGER_DELAY] = retriggerDelay; //1-100 ms before another hit can be registered,  
    settings[NOTE] = note;    
}

void ChannelSettings::_modifyParameter(int8_t amount, int8_t parameter) {
    switch(parameter) {
        case NOTE:
            this->settings[NOTE] = clipValue(this->settings[NOTE] + amount, NOTE_MIN, NOTE_MAX, CLIP_CLAMP_VALUE);
            break;
        
        // Clip threshold to 1, up to the current peak value
        case THRESHOLD:
            this->settings[THRESHOLD] = clipValue(this->settings[THRESHOLD] + amount, THRESHOLD_MIN, this->settings[PEAK], CLIP_CLAMP_VALUE);
            break;
        
        // Clip peak from current threshold value up to 100
        case PEAK:
            this->settings[PEAK] = clipValue(this->settings[PEAK] + amount, this->settings[THRESHOLD], PEAK_MAX, CLIP_CLAMP_VALUE);
            break;
        
        case ATTACK_SCAN:
            this->settings[ATTACK_SCAN] = clipValue(this->settings[ATTACK_SCAN] + amount, ATTACK_SCAN_MIN, ATTACK_SCAN_MAX, CLIP_CLAMP_VALUE);
            break;
        
        case RETRIGGER_DELAY:
            this->settings[RETRIGGER_DELAY] = clipValue(this->settings[RETRIGGER_DELAY] + amount, RETRIGGER_DELAY_MIN, RETRIGGER_DELAY_MAX, CLIP_CLAMP_VALUE);
            break;    
    }
}

void ChannelSettings::incrementParameter(int8_t parameter) {
    this->_modifyParameter(1, parameter);
}

void ChannelSettings::decrementParameter(int8_t parameter) {
   this->_modifyParameter(-1, parameter);
}