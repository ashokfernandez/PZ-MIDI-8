// Handles drawing the display, based on the current state

#ifndef ViewController_h
#define ViewController_h


class ViewController {
    public:
        ViewController();
        void drawDisplay(void);
    private:
        void _drawChannelList(void);
        void _drawChannelEdit(void);
};
#endif
