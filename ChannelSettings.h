#include "PZMIDI8.h"

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

enum EDIT_PARAMETER { NOTE, THRESHOLD, PEAK, ATTACK_SCAN, RETRIGGER_DELAY };

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
        ChannelSettings(uint8_t peak, uint8_t threshold, uint8_t attackScan, uint8_t retriggerDelay, uint8_t note);
        
        uint8_t settings[5] = {
            note, // 0 - 127, midi notes
            threshold, //1-100 cant be higher than max, is the min value needed before a hit is registered,  
            peak, // 1-100, 100 means all ADC values over 1000 register at 127 velocity 
            attackScan, //1- 10 how long the controller waits to see how hard the pad was hit,  
            retriggerDelay //1-100 ms before another hit can be registered,  
        };

        // Given an index to a parameter, increment or decrement the value. 
        // Handles clipping to the approrate range
        void incrementParameter(int8_t parameter);
        void decrementParameter(int8_t parameter);
    private:
        void _modifyParameter(int8_t amount, int8_t parameter);
}

#endif