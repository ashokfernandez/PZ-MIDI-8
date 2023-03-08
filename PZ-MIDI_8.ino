#include <TaskScheduler.h>

#include "PZMIDI8.h"
#include "MUX_4051.h"
#include "Channel.h"
#include "StateController.h"
#include "ViewController.h"

#include "Bitmaps/ChannelLabels.h"

// Allocate key objects to the heap to save dynamic memory

// Setup hardware objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(ROTARY_DATA_PIN, ROTARY_CLOCK_PIN, RotaryEncoder::LatchMode::FOUR3);
EasyButton button(ROTARY_PUSH_BUTTON_PIN);
MUX_4051 mux(MUX_SELECT_PIN_1, MUX_SELECT_PIN_2, MUX_SELECT_PIN_3, MUX_ANALOG_PIN_IN);

// // Setup software objects
// // HelloDrum objects with their MUX pin
// HelloDrum drumChannel1(0);
// HelloDrum drumChannel2(1);
// HelloDrum drumChannel3(2);
// HelloDrum drumChannel4(3);
// HelloDrum drumChannel5(4);
// HelloDrum drumChannel6(5);
// HelloDrum drumChannel7(6);
// HelloDrum drumChannel8(7);

// Channel channels[] = {
//   Channel(0, channelLabelBitmap_channel1, &drumChannel1), 
//   Channel(1, channelLabelBitmap_channel2, &drumChannel2), 
//   Channel(2, channelLabelBitmap_channel3, &drumChannel3), 
//   Channel(3, channelLabelBitmap_channel4, &drumChannel4), 
//   Channel(4, channelLabelBitmap_channel5, &drumChannel5), 
//   Channel(5, channelLabelBitmap_channel6, &drumChannel6), 
//   Channel(6, channelLabelBitmap_channel7, &drumChannel7), 
//   Channel(7, channelLabelBitmap_channel8, &drumChannel8)
// };

Channel channels[] = {
  Channel(0, channelLabelBitmap_channel1), //, &drumChannel1), 
  Channel(1, channelLabelBitmap_channel2), //, &drumChannel2), 
  Channel(2, channelLabelBitmap_channel3), //, &drumChannel3), 
  Channel(3, channelLabelBitmap_channel4), //, &drumChannel4), 
  Channel(4, channelLabelBitmap_channel5), //, &drumChannel5), 
  Channel(5, channelLabelBitmap_channel6), //, &drumChannel6), 
  Channel(6, channelLabelBitmap_channel7), //, &drumChannel7), 
  Channel(7, channelLabelBitmap_channel8) //, &drumChannel8)
};

StateController* state = new StateController(channels);
ViewController* view = new ViewController(&display, state, channels);


// ISR for rotary encoder change
void encoderISR(void) {
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
  mux.scanAnalogInputs(); // Scan the multiplexer
  

  // Serial.print("Floor:");
  // Serial.print(0);
  // Serial.print(",");
  // Serial.print("Ceil:");
  // Serial.print(1023);
  // Serial.print(",");
  // Serial.print("MUXIN0:");
  // // Serial.println(analogRead(MUX_ANALOG_PIN_IN));  
  // Serial.print(mux.getInput(0));  
  // Serial.print(",");
  // Serial.print("MUXIN1:");
  // Serial.print(mux.getInput(1));  
  // Serial.print(",");
  // Serial.print("MUXIN2:");
  // Serial.print(mux.getInput(2));  
  // Serial.print(",");
  // Serial.print("MUXIN3:");
  // Serial.print(mux.getInput(3));  
  // Serial.print(",");
  // Serial.print("MUXIN4:");
  // Serial.print(mux.getInput(4));  
  // Serial.print(",");
  // Serial.print("MUXIN5:");
  // Serial.print(mux.getInput(5));  
  // Serial.print(",");
  // Serial.print("MUXIN6:");
  // Serial.print(mux.getInput(6));  
  // Serial.print(",");
  // Serial.print("MUXIN7:");
  // Serial.println(mux.getInput(7));  
  
  

  for (uint8_t i = 0; i < NUM_CHANNELS; i++){
    channels[i].scanForDrumHit();
    channels[i].sendDrumHitOverMIDI();
  }
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
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Reset display buffer
  display.clearDisplay();
  
  // Attach interrupts for rotary encoder 
  attachInterrupt(digitalPinToInterrupt(ROTARY_DATA_PIN), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_CLOCK_PIN), encoderISR, CHANGE);

  // Attach callbacks and interrupt to push button switch
  button.begin();
  button.onPressed(buttonClicked);
  button.onPressedFor(LONG_PRESS_DURATION_MS, buttonLongClicked);
  button.enableInterrupt(buttonISR);
}

void loop() {
  taskScheduler.execute();
}