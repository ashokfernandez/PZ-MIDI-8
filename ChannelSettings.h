#include <Arduino.h>

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

#define NUM_PARAMETERS 5
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
        ChannelSettings(int8_t peak, int8_t threshold, int8_t attackScan, int8_t retriggerDelay, int8_t note);

        // Given an index to a parameter, increment or decrement the value. 
        // Handles clipping to the approrate range
        void incrementParameter(int8_t parameter);
        void decrementParameter(int8_t parameter);

        int8_t* settings;
    private:
        void _modifyParameter(int8_t amount, int8_t parameter);
};

#endif