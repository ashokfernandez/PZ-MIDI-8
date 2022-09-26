#include <TaskScheduler.h>

#include "PZMIDI8.h"
#include "Channel.h"
#include "StateController.h"
#include "ViewController.h"

// Setup hardware objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(ROTARY_DATA_PIN, ROTARY_CLOCK_PIN, RotaryEncoder::LatchMode::FOUR3);
EasyButton button(ROTARY_PUSH_BUTTON_PIN);

// Single global instances in heap mem of key objects
StateController* state;
ViewController* view;
// Channel* channels[NUM_CHANNELS];

// ChannelSettings settings[] = {
//   ChannelSettings(channel1Settings),
//   ChannelSettings(channel2Settings),
//   ChannelSettings(channel3Settings),
//   ChannelSettings(channel4Settings),
//   ChannelSettings(channel5Settings),
//   ChannelSettings(channel6Settings),
//   ChannelSettings(channel7Settings),
//   ChannelSettings(channel8Settings)
// };

#include "BitmapLabels.h"

Channel channels[] = {
  Channel(bitmapLabel_channel1, 0), 
  Channel(bitmapLabel_channel2, 1), 
  Channel(bitmapLabel_channel3, 2), 
  Channel(bitmapLabel_channel4, 3), 
  Channel(bitmapLabel_channel5, 4), 
  Channel(bitmapLabel_channel6, 5), 
  Channel(bitmapLabel_channel7, 6), 
  Channel(bitmapLabel_channel8, 7)
};

// ISR for rotary encoder change
ISR(PCINT1_vect) {
  encoder.tick();

  switch(encoder.getDirection()){
    case RotaryEncoder::Direction::CLOCKWISE:
      state->encoderIncremented();
      break;
    case RotaryEncoder::Direction::COUNTERCLOCKWISE:
      state->encoderDecremented();
      break;
  }
}
  
// ISR for pushbutton  
void buttonISR(void) { 
  button.read(); 
}

// Button clicked callback
void buttonClicked(void) { 
  state->buttonClicked();    
}

// Button clicked callback
void buttonLongClicked(void) { 
  state->buttonLongClicked();
}

// Callbacks to be executed by the task scheduler
void drawDisplay() { view->drawDisplay(); }
void updateState() {
  
  button.update(); // Needed for long press functionality to work
}

void simulateInputLevels() {
  for(int8_t i=0; i<NUM_CHANNELS; i++){
    channels[i].setLevel(i*15);
  }
}

// Setup the task scheduler to manage what should run when
Scheduler taskScheduler;
Task scanInputs(TASK_IMMEDIATE, TASK_FOREVER, &updateState, &taskScheduler, true);
Task updateDisplay(33 * TASK_MILLISECOND, TASK_FOREVER, &drawDisplay, &taskScheduler, true); // 33ms ~ 30fps
Task mockInputHits(2 * TASK_SECOND, TASK_FOREVER, &simulateInputLevels, &taskScheduler, true);

void setup() {
  
  Serial.begin(9600);
  // int8_t chanSetTest[] = {63, 5, 100, 10, 50};
  // Serial.println(chanSetTest[0]);
  // Serial.println(chanSetTest[1]);
  // Serial.println(chanSetTest[2]);
  
  // ChannelSettings settingtest = ChannelSettings(chanSetTest);
  // Serial.println(settingtest.setting[NOTE]);
  // Serial.println(settingtest.peak);
  // Serial.println(settingtest.threshold);
  // Serial.println(settingtest.attackScan);
  // Serial.println(settingtest.retriggerDelay);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Reset display buffer
  display.clearDisplay();
  
  // Attach interrupts for rotary encoder  
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.

  // Attach callbacks and interrupt to push button switch
  button.begin();
  button.onPressed(buttonClicked);
  button.onPressedFor(LONG_PRESS_DURATION_MS, buttonLongClicked);
  button.enableInterrupt(buttonISR);

  // Create all the channels, assigning a bitmap image for the list view and a channel
  // number which is used to address settings
  // channels[0] = new Channel(bitmapLabel_channel1, 0);
  // channels[1] = new Channel(bitmapLabel_channel2, 1); 
  // channels[2] = new Channel(bitmapLabel_channel3, 2); 
  // channels[3] = new Channel(bitmapLabel_channel4, 3); 
  // channels[4] = new Channel(bitmapLabel_channel5, 4); 
  // channels[5] = new Channel(bitmapLabel_channel6, 5); 
  // channels[6] = new Channel(bitmapLabel_channel7, 6); 
  // channels[7] = new Channel(bitmapLabel_channel8, 7);

  // channels = {[
  //   Channel(bitmapLabel_channel1, 0), 
  //   Channel(bitmapLabel_channel2, 1), 
  //   Channel(bitmapLabel_channel3, 2), 
  //   Channel(bitmapLabel_channel4, 3), 
  //   Channel(bitmapLabel_channel5, 4), 
  //   Channel(bitmapLabel_channel6, 5), 
  //   Channel(bitmapLabel_channel7, 6), 
  //   Channel(bitmapLabel_channel8, 7)
  // ]};
    
  // }

  // (Channel*) channels[0] is the pointer to the first channel
  state = new StateController(channels);
  view = new ViewController(&display, state, channels);

  
}

void loop() {
  taskScheduler.execute();
}