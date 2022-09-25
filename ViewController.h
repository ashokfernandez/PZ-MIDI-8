// Handles drawing the display, based on the current state
#include "PZMIDI8.h"

#ifndef ViewController_h
#define ViewController_h


class ViewController {
    public:
        ViewController(Adafruit_SSD1306* display, StateController* state, Channel* channels);
        void drawDisplay(void);

    private:
        Adafruit_SSD1306* _display;
        StateController* _state;
        Channel* _channels;
        void _drawChannelList(void);
        void _drawChannelEdit(void);
};
#endif
