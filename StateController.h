 #include "PZMIDI8.h"

#ifndef StateController_h
#define StateController_h



// Timeout if no interactions happen
#define SELECT_CHANNEL_TIMEOUT_MS 5000


// Different states the application can be in 
enum PAGE { CHANNEL_LIST, CHANNEL_EDIT, SAVE_IN_PROGRESS };


// Value to set the selected channel variable to if no channel is selected
#define NO_CHANNEL_SELECTED -10
#define NO_PARAMETERS_SELECTED -10

// };

// PAD[0] = sensitivity = MAX
// PAD[1] = threshold = THRESHOLD
// PAD[2] = scan_time = ATTACK_SCAN
// PAD[3] = mask_time = RETRIGGER_DELAY
// PAD[4] = note = NOTE 
// #define MIDI_CHANNEL 0

// pad1.singlePiezoMUX (settings.max, settings.threshold, settings.attackScan, settings.retriggerDelay);
// MIDI.sendNoteOn(settings.note, pad2.velocity, MIDI_CHANNEL); //(note, velocity, channel)
// MIDI.sendNoteOff(settings.note, 0, MIDI_CHANNEL);

// }



class StateController {
    public:
        // Public interface only contains callbacks for external events, and a getter
        StateController();

        // todo create a function which updates the state of all inuputs, that's what this used to be for
        // void update(void);

        // int8_t getState(void);
        // Current state
        int8_t page;
        int8_t selectedChannel;
        int8_t selectedParameter;
        uint16_t switchHoldTime_ms;
        bool editingParameter;

        void buttonClicked(void);
        void buttonHeldFor_ms(uint16_t time_ms);
        void buttonLongClicked(void);

        void encoderIncremented(void);
        void encoderDecremented(void);
    private: 

        // Tracks encoder changes
        // int8_t _encoderPosition;
        // unsigned long _encoderLastChanged_ms;

        // Manages scrolling through channels on the channel list
        // int8_t _selectedChannel;
        
        // void _selectChannel(uint8 channelIndex);
        void _deselectAllChannels(void);
        void _incrementSelectedChannel(void);
        void _decrementSelectedChannel(void);

        void _incrementSelectedParameter(void);
        void _decrementSelectedParameter(void);
        void _deselectAllParameters(void); 
        
        void _incrementSelectedParameterValue(void);
        void _decrementSelectedParameterValue(void);
};
#endif
