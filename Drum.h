#include "PZMIDI8.h"
#include "ChannelSettings.h"

class Drum {
    public:
        Drum(int16_t* inputAddress, ChannelSettings* channelSettings);
        void update(void);

    private:
        int16_t* _inputAddress;
        ChannelSettings* _channelSettings;
};