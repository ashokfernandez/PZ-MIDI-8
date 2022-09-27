#include "PZMIDI8.h"


#define NUM_SELECT_PINS 3
#define NUM_ANALOG_INPUTS 8

Class MUX_4051{
    public:
        MUX_4015(int8_t selectPins[NUM_SELECT_PINS], int8_t analogInPin);  //s0,s1,s2, analogPin{
        int8_t selectPin[NUM_SELECT_PINS];
        int8_t analogInPin;
        int16_t inputBuffer[NUM_ANALOG_INPUTS];
        void scanAnalogInputs(void);
        int16_t getInput(int8_t inputIndex);
    
}