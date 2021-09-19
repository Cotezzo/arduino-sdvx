#include "Encoder.h"
#include "Keyboard.h"
#include "Mouse.h"

/* ==== Pinout ======================================================================================== */
const byte BT_A = 4;
const byte BT_B = 5;
const byte BT_C = 6;
const byte BT_D = 7;
const byte FX_L = 8;
const byte FX_R = 9;
const byte START = 10;

const byte LED_BT_A = 21; // A3;
const byte LED_BT_B = 20; // A2;
const byte LED_BT_C = 19; // A1;
const byte LED_BT_D = 18; // A0;
const byte LED_FX_L = 15;
const byte LED_FX_R = 14;
const byte LED_START = 16;

Encoder KNOB_L(3, 2), KNOB_R(1, 0);

/* ==== Bindings ====================================================================================== */
const char BIND_BT_A = 'a';
const char BIND_BT_B = 's';
const char BIND_BT_C = 'd';
const char BIND_BT_D = 'f';
const char BIND_FX_L = 'z';
const char BIND_FX_R = 'x';
const char BIND_START = KEY_RETURN;

/* ==== Arrays ======================================================================================== */
const byte buttonPins[] = { BT_A, BT_B, BT_C, BT_D, FX_L, FX_R, START };
const byte ledPins[] = { LED_BT_A, LED_BT_B, LED_BT_C, LED_BT_D, LED_FX_L, LED_FX_R, LED_START };
const byte bindKeys[] = { BIND_BT_A, BIND_BT_B, BIND_BT_C, BIND_BT_D, BIND_FX_L, BIND_FX_R, BIND_STRT };
const byte buttonCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

/* ==== Init values =================================================================================== */
const byte MOUSE_SENS = 3;  // Mouse Sensibility Multiplier

long KNOB_L_VALUE = 0;
long KNOB_R_VALUE = 0;
long KNOB_L_OLD_VALUE = 0;
long KNOB_R_OLD_VALUE = 0;

/* ==== SETUP ====================== */
void setup(){
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
}

/* ==== LOOP ======================= */
void loop(){
  /* ==== READ ENCODERS === */
  KNOB_L_VALUE = KNOB_L.read();                                     // Read encoders value
  KNOB_R_VALUE = KNOB_R.read();
  if(KNOB_L_VALUE != KNOB_L_OLD_VALUE) {                            // If the value is different since the previous loop
    Mouse.move((KNOB_L_VALUE - KNOB_L_OLD_VALUE) * MOUSE_SENS, 0);  // Change mouse position using the difference between the two values
    
    if(KNOB_L_VALUE < -255 || KNOB_L_VALUE > 255) {
      KNOB_L.write(0);                                              // Reset value if we exceed int limit
      KNOB_L_OLD_VALUE = 0;
    } else {
      KNOB_L_OLD_VALUE = KNOB_L_VALUE;
    }
  }
  
  if(KNOB_R_VALUE != KNOB_R_OLD_VALUE) {
    Mouse.move(0, (KNOB_R_VALUE - KNOB_R_OLD_VALUE) * MOUSE_SENS);
    
    if(KNOB_R_VALUE < -255 || KNOB_R_VALUE > 255) {
      KNOB_R.write(0);
      KNOB_R_OLD_VALUE = 0;
    } else {
      KNOB_R_OLD_VALUE = KNOB_R_VALUE;
    }
  }
  
  /* ==== READ BUTTONS ==== */
  byte i = 0;
  while(i < buttonCount){                                           // For each button
    if (digitalRead(buttonPins[i]) == LOW){                         // If the signal is low
      Keyboard.press(bindKeys[i]);                                  // Input a keyboard press
      digitalWrite(ledPins[i], HIGH);                               // And turn on the LED
    } else {
      Keyboard.release(bindKeys[i]);                                // Else, release the key
      digitalWrite(ledPins[i], LOW);                                // And turn off the LED
    }
    i++;
  }
}
