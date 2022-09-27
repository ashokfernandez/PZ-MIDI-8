#include "MUX_4051.h"

//s0,s1,s2, analogPin
MUX_4051::MUX_4051(int8_t selectPins[NUM_SELECT_PINS], int8_t analogInPin) {
    this->selectPin = selectPin;
    this->analogInPin = analongInPin;

    // Setup select pins as digital outputs, default to high
    for (uint8_t i = 0; i < NUM_SELECT_PINS; i++){
        pinMode(selectPin[i], OUTPUT);
        digitalWrite(selectPin[i], HIGH);
    }

    // Clear the buffer
    for (i = 0; i < NUM_ANALOG_INPUTS; i++){
        this->inputBuffer[i] = 0;
    }
}

void MUX_4051::scanAnalogInputs(void){
    for (byte pin = muxNum * 8; pin < (muxNum + 1) * 8; pin++)
  {

    
    for (byte i = 0; i < 3; i++)
    {
      if (pin & (1 << i))
        digitalWrite(selectPins[i], HIGH);
      else
        digitalWrite(selectPins[i], LOW);
    }

    rawValue[pin] = analogRead(this->);
  }

}

Class MUX_4051{
    public:
        MUX_4015(int8_t selectPins[NUM_SELECT_PINS], int8_t analogInPin);  //s0,s1,s2, analogPin{
        int8_t selectPin[NUM_SELECT_PINS];
        int8_t analogInPin;
        void scanAnalogInputs(void);
        int16_t getInput(int8_t inputIndex);
        int16_t inputBuffer[NUM_ANALOG_INPUTS];
    
}