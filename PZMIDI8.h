// Hardware drivers
#ifndef DRIVERS_H
#define DRIVERS_H

  #include <Arduino.h>
  #include <math.h>
  #include <hellodrum.h>
  #include <SPI.h>
  #include <Wire.h>
  #include <EasyButton.h>
  #include <RotaryEncoder.h>
  #include <Adafruit_SSD1306.h>

#endif

#ifndef CLASSES_H
#define CLASSES_H
  // Forward delcare classes to avoid #include interdependanies
  class Channel;
  class ChannelSettings;
  class StateController;
  class ViewController;
#endif

#ifndef CONFIG_H
#define CONFIG_H

  #define NUM_CHANNELS 8 

  // Rotary encoder and switch
  #define ROTARY_DATA_PIN A2
  #define ROTARY_CLOCK_PIN A3
  #define ROTARY_PUSH_BUTTON_PIN 2
  #define LONG_PRESS_DURATION_MS 1500

  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  // The pins for I2C are defined by the Wire-library. 
  // On an arduino UNO:       A4(SDA), A5(SCL)
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#endif 





