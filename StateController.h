 #include "PZMIDI8.h"

#ifndef StateController_h
#define StateController_h

// Rotary encoder pins
#define ROTARY_DATA_PIN 3
#define ROTARY_CLOCK_PIN A3

// Push button switch
#define ROTARY_PUSH_BUTTON_PIN 2
#define LONG_PRESS_DURATION_MS 1500

// Timeout if no interactions happen
#define SELECT_CHANNEL_TIMEOUT_MS 5000

// Different states the application can be in 
enum STATES { CHANNEL_LIST, CHANNEL_EDIT, SAVE_IN_PROGRESS };

// TODO Setup state changes for callbacks
void buttonClicked(void);
void buttonLongClicked(void);


class StateController {
    public:
        StateController();
        void update(void);
        int8_t getCurrentState(void);
        int8_t getSelectedChannel(void);
        
        // Don't call this directly, exposed as a public method so the button callbacks can access it
        void setCurrentState(uint8_t state);

    private:
        // Current state
        uint8_t _currentState;
        

        // Tracks encoder changes
        int8_t _encoderPosition;
        unsigned long _encoderLastChanged_ms;

        // Manages scrolling through channels on the channel list
        int8_t _selectedChannel;
        void _selectChannel(void);
        void _deselectChannel(void);
        void _decrementSelectedChannel(void);
        void _incrementSelectedChannel(void);

};
#endif
