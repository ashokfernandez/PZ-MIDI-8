// Hardware drivers
#ifndef DRIVERS_H
#define DRIVERS_H

  #include <Arduino.h>
  #include <math.h>
  #include <EEPROM.h>
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
  #define MIDI_CHANNEL 1

  // Rotary encoder and switch
  #define ROTARY_DATA_PIN 0
  #define ROTARY_CLOCK_PIN 1
  #define ROTARY_PUSH_BUTTON_PIN 7
  #define LONG_PRESS_DURATION_MS 1500

  #define MUX_SELECT_PIN_1 6
  #define MUX_SELECT_PIN_2 5
  #define MUX_SELECT_PIN_3 4
  #define MUX_ANALOG_PIN_IN A10

  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  // The pins for I2C are defined by the Wire-library. 
  // On an arduino UNO:       A4(SDA), A5(SCL)
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 32 // OLED display height, in pixels

  // The SSD1306 OLED Display used has a top header row that displays in yellow, and 
  // a main section that displays in blue
  #define HEADER_HEIGHT 8 
  #define MAIN_DISPLAY_HEIGHT (SCREEN_HEIGHT-HEADER_HEIGHT) 
  #define CHANNEL_WIDTH (SCREEN_WIDTH/NUM_CHANNELS) // Display width of each channel

#endif 





