// MButton.h

/*
ESP32 LIBRARY TO DETECT ASYNCHRONOUSLY SHORT AND LONG PRESS OF SEVERAL PUSH BUTTONS 
Restrictions :
  - Each button is connected to the ground and a different pin
  - Their gpios need to have a pull-up resistance (e.g. pin number 12, 13, etc.)
  - Only one button can be pressed at a time (no buffer to register the actions) 
  - Need to wait that an action has been processed before a new action
Usage :
  - declare an instance of the class
  - in the setup function call begin(<no Buttons>, <num1>, <num2>, ...) to precise the pins and attach their callbacks
  - in the loop function call successively toProccess(), getNum(), getAction(), processed()
  - can call end() to detach callbacks

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

# pragma once

#include "Arduino.h"
#include <Ticker.h>

#define DELAY_DEBOUNCE 200                // Delay for debounce (ms)
#define DELAY_LONG_PRESS 1000             // Delay for long press (ms)

class MButton
{
  public:
    MButton();                            // Constructor
    void begin(int noButtons, ...);       // To define the pin numbers and attach callbacks
    void end();                           // To detach the callbacks
    bool toProcess();                     // True if there is a pending action on the button
    int getNum();                         // Pin number
    int getAction();                      // Action number : 1 => pressed, 2 => long pressed
    void processed();                     // To call after action treatment (needed !)

  private:
    static volatile int _num;             // Pin number
    static volatile bool _toProcess;      // True if action to process
    static volatile int _action;          // 1 => pressed, 2 => longpressed
    static bool _okButton;                // To avoid debounce
    static int _oldNum;                   // To save pin number so that mixed action are forbidden  
    static unsigned long _oldTime;        // To save time (ms) for detecting long press action
    static int _oldLevel;                 // To save pin level for detecting actions (LOW or HIGH)  
    static Ticker _timer;                 // Alarm timer
    static void IRAM_ATTR buttonInterrupt12();
    static void IRAM_ATTR buttonInterrupt13();
    // Here can add ISR for other pins
    static void IRAM_ATTR timerInterrupt();  
};
