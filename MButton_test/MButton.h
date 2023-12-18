// MButton.h

/***********************************************************************************
 
ESP32 Library to detect actions on several push buttons

Functionalities :
  - Acceptable actions : short, double (short) and long press button.
  - Works asynchonously, using change and timer interrupts.
  - Debounce protection
  - Calls the Ticker library.

Restrictions :
  - Each button is connected to the ground and a different pin.
  - Their gpios need to have a pull-up resistance (e.g. pin number 12, 13, etc.).
  - Only one button can be pressed at a time (no buffer to register the actions).
  - Need to wait that an action has been processed before a new action.
  - 2*DELAY_DEBOUNCE > DELAY_DOUBLE_PRESS so that maintained press is not possible in a double press

Usage :
  - Declare an instance of the class.
  - In the setup function call begin(<no Buttons>, <num1>, <num2>, ...) to precise the pins and attach their callbacks.
  - In the loop function call successively toProccess(), getNum(), getAction(), processed().
  - Can call end() to detach callbacks.

Reference :
  https://github.com/jlemaire06/Esp32-async-multi-button-library
  https://github.com/espressif/arduino-esp32/tree/master/libraries/Ticker
	
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.


***********************************************************************************/

#ifndef _MBUTTON_H
#define _MBUTTON_H

/***********************************************************************************
  Libraries and types
***********************************************************************************/

#include "Arduino.h"
#include <Ticker.h>

enum ButtonAction
{
	BA_SHORT_PRESS,
	BA_DOUBLE_PRESS,
	BA_LONG_PRESS
};

/***********************************************************************************
  Constants
***********************************************************************************/

#define DELAY_DEBOUNCE 250                    // Delay for debounce (ms)
#define DELAY_DOUBLE_PRESS 500                // Delay for double press (ms)
#define DELAY_LONG_PRESS 1000                 // Delay for long press (ms)

/***********************************************************************************
  MButton class
***********************************************************************************/

class MButton
{
public:
  MButton();                                  // Constructor
  void begin(int nbButtons, ...);             // To define the pin numbers and attach callbacks
  void end();                                 // To detach the callbacks
  bool toProcessM() {return toProcess;}       // True if there is a pending action on the button
  int getNum() {return num;}                  // Pin number
  ButtonAction getAction() {return action;}   // Action on the button
  void processed();                           // To call after action treatment (needed !)

private:
  static volatile int num;                    // Pin number
  static volatile bool toProcess;             // True if action to process
  static volatile ButtonAction action;        // Action on the button
  static bool okButton;                       // To avoid debounce 
  static int oldNum;                          // To save pin number so that mixed action are forbidden
  static int oldLevel;                        // To save pin level for detecting actions (LOW or HIGH)
  static Ticker timerDebounce;                // Alarm timer for debounce protection
  static Ticker timerDoublePress;             // Alarm timer for double press detection
  static Ticker timerLongPress;               // Alarm timer for long press detection
  static bool timerDoublePressActive;         // State of the double press timer
  static bool timerLongPressActive;           // State of the long press timer
  static void IRAM_ATTR buttonInterrupt12();
  static void IRAM_ATTR buttonInterrupt13();
  // Here can add ISR for other pins
  static void IRAM_ATTR timerDebounceInterrupt();
  static void IRAM_ATTR timerDoublePressInterrupt();
  static void IRAM_ATTR timerLongPressInterrupt();
};

#endif
