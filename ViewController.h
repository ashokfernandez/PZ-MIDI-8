// Handles drawing the display, based on the current state
#include "PZMIDI8.h"
#include "StateController.h"

#ifndef ViewController_h
#define ViewController_h


class ViewController {
    public:
        ViewController(Adafruit_SSD1306* display, StateController* state);
        void drawDisplay(void);

    private:
        Adafruit_SSD1306* _display;
        StateController* _state;
        void _drawChannelList(void);
        void _drawChannelEdit(void);
};
#endif
