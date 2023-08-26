#include "PZMIDI8.h"

#ifndef MUX_4051_h
#define MUX_4051_h

#define NUM_SELECT_PINS 3
#define NUM_ANALOG_INPUTS 8

class MUX_4051 {
    public:
        MUX_4051(int8_t selectPin0, int8_t selectPin1, int8_t selectPin2, int8_t analogInPin);  //s0,s1,s2, analogPin{
        int8_t analogInPin;
        int8_t selectPin[NUM_SELECT_PINS];
        volatile static inline int16_t inputBuffer[NUM_ANALOG_INPUTS];
        void scanAnalogInputs(void);
        int16_t getInputValue(int8_t inputIndex);
};

#endif