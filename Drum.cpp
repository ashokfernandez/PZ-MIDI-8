#include "Drum.h"

Drum::Drum(int16_t* inputAddress, ChannelSettings* channelSettings) {
    this->_inputAddress = inputAddress;
    this->_channelSettings = channelSettings;
}

// perhaps rename this method
void Drum::update(void) {
    int16_t inputValue = *this->_inputAddress;
    
    // todo: Port algortihm from hello drum here
    // detect if we've gone over the threshold
        // if so for the first time, send a midi note on message
        // if so for the second time, send a midi note off message
    // detect if we've waited long enough to retrigger
        // if so, reset the counter
    


}

// void HelloDrum::singlePiezoSensing(byte sens, byte thre, byte scanTime, byte maskTime)
// {

//   int Threshold = thre * 10;
//   int Sensitivity = sens * 10;

//   hit = false;

//   //when the value > threshold
//   if (piezoValue > Threshold && loopTimes == 0)
//   {
//     //Start the scan time
//     time_hit = millis(); //check the time pad hitted

//     //compare time to cancel retrigger
//     if (time_hit - time_end < maskTime)
//     {
//       return; //Ignore the scan
//     }
//     else
//     {
//       velocity = piezoValue; //first peak
//       loopTimes = 1;         //start scan trigger
//     }
//   }

//   //peak scan start
//   if (loopTimes > 0)
//   {
//     if (piezoValue > velocity)
//     {
//       velocity = piezoValue;
//     }
//     loopTimes++;

//     //scan end
//     //if (loopTimes == scanTime)
//     if (millis() - time_hit >= scanTime)
//     {


//       velocity = curve(velocity, Threshold, Sensitivity, curvetype); //apply the curve at the velocity
//       hit = true;                                                    //mark as hit
//       time_end = millis();


//       loopTimes = 0; //reset loopTimes (ready for next sensing)
//     }
//   }
// }
