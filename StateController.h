#ifndef StateController_h
#define StateController_h

#include "Arduino.h" 
#include "Channel.h"
#include <EasyButton.h>
#include <RotaryEncoder.h>
#include <Adafruit_SSD1306.h>

// Rotary encoder pins
#define ROTARY_DATA_PIN 3
#define ROTARY_CLOCK_PIN A3

// Push button switch
#define ROTARY_PUSH_BUTTON_PIN 2
#define LONG_PRESS_DURATION_MS 1500

// Declare the functions used in the setup() function to hookup
// interrupts and click callbacks
//void _encoderISR(void);
//void _buttonISR(void);
void buttonClicked(void);
void buttonLongClicked(void);


class StateController {
public:
    StateController(Channel channel[]);
    void update(void);
    void drawChannelList(void);


private:
    Channel* _channel;
    Adafruit_SSD1306* _display;
    int8_t _selectedChannel;
    int8_t _encoderPosition;
    void _selectChannel(void);
    void _decrementSelectedChannel(void);
    void _incrementSelectedChannel(void);

};
#endif
