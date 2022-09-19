#include "PZMIDI8.h"

#ifndef ViewController_h
#define ViewController_h

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

ViewController::ViewController() {
    
}

class ViewController {
    public:
        ViewController();
        void drawChannelList(void);
    // private:
        

};
#endif
