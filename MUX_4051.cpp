#include "MUX_4051.h"

//s0,s1,s2, analogPin
MUX_4051::MUX_4051(int8_t selectPin0, int8_t selectPin1, int8_t selectPin2, int8_t analogInPin) {
  this->analogInPin = analogInPin;
  this->selectPin[0] = selectPin0;
  this->selectPin[1] = selectPin1;
  this->selectPin[2] = selectPin2;
  
  // Setup select pins as digital outputs, default to high
  for (uint8_t i = 0; i < NUM_SELECT_PINS; i++){
      pinMode(selectPin[i], OUTPUT);
      digitalWrite(selectPin[i], HIGH);
  }

  // Clear the buffer
  for (uint8_t i = 0; i < NUM_ANALOG_INPUTS; i++){
      this->inputBuffer[i] = 0;
  }
}

// Scan all analog inputs and store the values in the buffer
void MUX_4051::scanAnalogInputs(void){ 
  for (int8_t i = 0; i < NUM_ANALOG_INPUTS; i++){
    digitalWrite(this->selectPin[0], HIGH && (i & B00000001));
    digitalWrite(this->selectPin[1], HIGH && (i & B00000010));
    digitalWrite(this->selectPin[2], HIGH && (i & B00000100));
    this->inputBuffer[i] = analogRead(this->analogInPin);
  }
}

// Returns the current value of the input at the given index
int16_t MUX_4051::getInputValue(int8_t inputIndex) {
  return this->inputBuffer[inputIndex];
}