#include <TaskScheduler.h>

#include "PZMIDI8.h"
#include "Channel.h"
#include "StateController.h"
#include "ViewController.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Setup hardware objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(ROTARY_DATA_PIN, ROTARY_CLOCK_PIN, RotaryEncoder::LatchMode::FOUR3);
EasyButton button(ROTARY_PUSH_BUTTON_PIN);

// ISRs for external interrupts
void _encoderISR(void) { encoder.tick(); }
void _buttonISR(void) { button.read(); }

// Single global instances in heap mem of key objects
StateController* state;
ViewController* view;

#include "BitmapLabels.h"

Channel channels[] = {
  Channel(bitmapLabel_channel1, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel2, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel3, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel4, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel5, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel6, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel7, new ChannelSettings(100, 5, 10, 30, 63)), 
  Channel(bitmapLabel_channel8, new ChannelSettings(100, 5, 10, 30, 63))
};


void buttonClicked(void) { 
  state->buttonClicked();    
}

void buttonLongClicked(void) { 
  state->buttonLongClicked();
}

// Callbacks to be executed by the task scheduler
void drawDisplay() { view->drawDisplay(); }
void updateState() { 
  


  // Check if the encoder moved
  encoder.tick();
  int direction = (int)encoder.getDirection();
  
  // if so, trigger a state update
  if(direction > 0) { 
    state->encoderIncremented();
  } else if (direction < 0) { 
    state->encoderDecremented();
  }  
}

void simulateInputLevels() {
  for(int8_t i=0; i<NUM_CHANNELS; i++){
    channels[i].setLevel(i*15);
  }
}

// Setup the task scheduler to manage what should run when
Scheduler taskScheduler;
Task readInputs(TASK_IMMEDIATE, TASK_FOREVER, &updateState, &taskScheduler, true);
Task updateDisplay(33 * TASK_MILLISECOND, TASK_FOREVER, &drawDisplay, &taskScheduler, true); // 33ms ~ 30fps
Task mockInputHits(2 * TASK_SECOND, TASK_FOREVER, &simulateInputLevels, &taskScheduler, true);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Attach interrupt for rotary encoder
  attachInterrupt(digitalPinToInterrupt(ROTARY_DATA_PIN), _encoderISR, CHANGE);

  // Attach callbacks and interrupt to push button switch
  button.begin();
  button.onPressed(buttonClicked);
  button.onPressedFor(LONG_PRESS_DURATION_MS, buttonLongClicked);
  button.enableInterrupt(_buttonISR);

  // Reset display buffer
  display.clearDisplay();

  state = new StateController();
  view = new ViewController(&display, state);
}

void loop() {
  taskScheduler.execute();
}