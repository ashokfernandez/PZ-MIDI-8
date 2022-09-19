 #include "PZMIDI8.h"

#ifndef StateController_h
#define StateController_h

// Rotary encoder pins
#define ROTARY_DATA_PIN 3
#define ROTARY_CLOCK_PIN A3

// Push button switch
#define ROTARY_PUSH_BUTTON_PIN 2
#define LONG_PRESS_DURATION_MS 1500

// TODO Setup state changes for callbacks
void buttonClicked(void);
void buttonLongClicked(void);


class StateController {
    public:
        StateController();
        void update(void);
        int8_t getSelectedChannel(void);


    private:
//        Channel* _channel;
//        Adafruit_SSD1306* _display;
        int8_t _selectedChannel;
        int8_t _encoderPosition;
        void _selectChannel(void);
        void _decrementSelectedChannel(void);
        void _incrementSelectedChannel(void);

};
#endif
