#include <TaskScheduler.h>

#include "PZMIDI8.h"
#include "MUX_4051.h"
#include "Channel.h"
#include "StateController.h"
#include "ViewController.h"

#include "Bitmaps/ChannelLabels.h"


USBMIDI_CREATE_DEFAULT_INSTANCE();

// ------------------------------
// Setup hardware interfaces and main objects
// ------------------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(ROTARY_DATA_PIN, ROTARY_CLOCK_PIN, RotaryEncoder::LatchMode::FOUR3);
EasyButton button(ROTARY_PUSH_BUTTON_PIN);
MUX_4051 mux(MUX_SELECT_PIN_1, MUX_SELECT_PIN_2, MUX_SELECT_PIN_3, MUX_ANALOG_PIN_IN);

// Declare space in memory for the static pointers in the channel class to the mux 
// and midi interface, and instantiate the channels
MUX_4051* Channel::mux = nullptr;
usbMidi::MidiInterface<usbMidi::usbMidiTransport>* Channel::MIDI = nullptr;
Channel channels[] = {
  Channel(0, channelLabelBitmap_channel1),
  Channel(1, channelLabelBitmap_channel2),
  Channel(2, channelLabelBitmap_channel3),
  Channel(3, channelLabelBitmap_channel4),
  Channel(4, channelLabelBitmap_channel5),
  Channel(5, channelLabelBitmap_channel6),
  Channel(6, channelLabelBitmap_channel7),
  Channel(7, channelLabelBitmap_channel8)
};

// State and view controllers that handle inputs from hardware, and updating the display accordingly
StateController* state = new StateController(channels);
ViewController* view = new ViewController(&display, state, channels);

// ------------------------------
// Hardware ISRs and callbacks
// ------------------------------

// Rotary encoder ISR
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
  
// Push button ISR
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

// ------------------------------
// Task scheduler
// ------------------------------

// Tell the view controller to redraw the display based on the current state
void drawDisplay() { view->drawDisplay(); }

// Loop through all the updates needed to gather new inputs from hardware, and process
// them to update the state
void updateState() {
  button.update(); // Needed for long press functionality to work
  mux.scanAnalogInputs(); // Scan the multiplexer to update the analog input values
  
  // Update the state of the channels
  for (uint8_t i = 0; i < NUM_CHANNELS; i++){
    channels[i].update();
  }
}

// TODO: Remove this mock up, currently just tests the display meters work
void simulateInputLevels() {
  for(int8_t i=0; i<NUM_CHANNELS; i++){
    channels[i].setLevel(i*15);
  }
}

// TODO: Remove this mock up, currently just tests the MIDI output works
unsigned long previousMillis = 0;  // will store last time note was updated
const long interval = 1000;  // interval at which to send note (milliseconds)
bool noteIsOn = false;  // state of the note

void simulateMidiMessage() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you sent a note
    previousMillis = currentMillis;

    if (noteIsOn) {
      // Turn the note off
      MIDI.sendNoteOff(60, 0, 1);  // Note number, velocity, channel
      noteIsOn = false;
    } else {
      // Turn the note on
      MIDI.sendNoteOn(60, 127, 1);  // Note number, velocity, channel
      noteIsOn = true;
    }
  }
}

// Instantiate the task scheduler and define the tasks
Scheduler taskScheduler;
Task scanInputs(TASK_IMMEDIATE, TASK_FOREVER, &updateState, &taskScheduler, true);
Task updateDisplay(33 * TASK_MILLISECOND, TASK_FOREVER, &drawDisplay, &taskScheduler, true); // 33ms ~ 30fps
Task mockInputHits(2 * TASK_SECOND, TASK_FOREVER, &simulateInputLevels, &taskScheduler, true);
Task mockMidiMessages(500 * TASK_MILLISECOND, TASK_FOREVER, &simulateMidiMessage, &taskScheduler, true);

// ------------------------------
// Setup and loop
// ------------------------------

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

  // Connect the multiplexer and MIDI interface the channels
  Channel::setMux(&mux);
  Channel::setMIDI(&MIDI);
}

void loop() {
  taskScheduler.execute();
}