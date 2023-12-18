// MButton.cpp

#include "MButton.h"

/***********************************************************************************
  MButton class
***********************************************************************************/

// Implement static members
volatile int MButton::num;
volatile bool MButton::toProcess;
volatile ButtonAction MButton::action;
bool MButton::okButton;
int MButton::oldNum;
int MButton::oldLevel;
Ticker MButton::timerDebounce;
Ticker MButton::timerDoublePress;
Ticker MButton::timerLongPress;
bool MButton::timerDoublePressActive;
bool MButton::timerLongPressActive;

MButton::MButton()
{
  okButton = true;
  oldLevel = HIGH;
  toProcess = false;
  timerDoublePressActive = false;
  timerLongPressActive = false;
}

void MButton::begin(int nbMButtons, ...)
{
  va_list arg;
  va_start(arg, nbMButtons);
  for (uint8_t i = 0; i < nbMButtons; i++)
  {
    uint8_t num = va_arg(arg, int);
    pinMode(num, INPUT_PULLUP);
    switch (num)
    {
    case 13:
      attachInterrupt(13, buttonInterrupt13, CHANGE);
      break;
    case 12:
      attachInterrupt(12, buttonInterrupt12, CHANGE);
      break;
      // Here can add attachments for other pins
    }
  }
}

void MButton::end()
{
  detachInterrupt(12);
  detachInterrupt(13);
  // Here can add detachments for other pins
  timerDebounce.detach();
  timerDoublePress.detach();
  timerLongPress.detach();
}

void MButton::processed()
{
  toProcess = false;
}


void IRAM_ATTR MButton::buttonInterrupt12()
{
  if (okButton && !toProcess)
  {
    num = 12;
    okButton = false;
    timerDebounce.once_ms(DELAY_DEBOUNCE, timerDebounceInterrupt);
  }
}

void IRAM_ATTR MButton::buttonInterrupt13()
{
  if (okButton && !toProcess)
  {
    num = 13;
    okButton = false;
    timerDebounce.once_ms(DELAY_DEBOUNCE, timerDebounceInterrupt);
  }
}

void MButton::timerDebounceInterrupt()
{
  int level = digitalRead(num);
  if (!toProcess)
  {
    if (oldLevel == HIGH)
    {
      if (level == HIGH)
      {
        if (timerDoublePressActive)
        {
          if (num != oldNum) num = oldNum; // Forbidden mixed case
          else 
          {
            action = BA_DOUBLE_PRESS;
            timerDoublePress.detach(); // Stop timer double press
            timerDoublePressActive = false;
            toProcess = true;
          }
        }
        else // First short press
        {
          timerDoublePress.once_ms(DELAY_DOUBLE_PRESS, timerDoublePressInterrupt); // Activate timer double press
          timerDoublePressActive = true;
          oldNum = num;
        }       
      }
      else // Level == LOW 
      {
        timerLongPress.once_ms(DELAY_LONG_PRESS, timerLongPressInterrupt); // Activate timer long press
        timerLongPressActive = true;     
        oldNum = num;
        oldLevel = LOW;
      }
    }
    else // oldLevel = LOW
    {
      if ((level == HIGH))
      {
        if (num != oldNum) num = oldNum; // Forbidden mixed case
        else 
        {
          if (!timerLongPressActive) action = BA_LONG_PRESS;
          else // Maintained Short press
          {
            timerLongPress.detach(); // Stop timer long press
            timerLongPressActive = false;
            action = BA_SHORT_PRESS;
          }
          toProcess = true;
          oldLevel = HIGH;
        }
      }
    } 
  }
  okButton = true; // Enable button interrupts
}

void MButton::timerDoublePressInterrupt()
{
  timerDoublePressActive = false;
  action = BA_SHORT_PRESS;
  toProcess = true;
}

void MButton::timerLongPressInterrupt()
{
  timerLongPressActive = false;
}
