// MButton.cpp

#include "MButton.h"
#include <Ticker.h>

// Implement static members
volatile int MButton::_num;        
volatile bool MButton::_toProcess; 
volatile int MButton::_action;     
bool MButton::_okButton;           
int MButton::_oldNum;              
unsigned long MButton::_oldTime;   
int MButton::_oldLevel;              
Ticker MButton::_timer; 

MButton::MButton()
{
  _okButton = true;
  _oldLevel = HIGH;
  _toProcess = false;
}

void MButton::begin(int noMButtons, ...)
{ 
	va_list arg;
	va_start(arg, noMButtons);
	for (uint8_t i = 0; i < noMButtons; i++)
	{
    uint8_t num = va_arg(arg, int);
    pinMode(num, INPUT_PULLUP);
    switch (num)
    {
      case 12:
        attachInterrupt(12, buttonInterrupt12, CHANGE);
        break;
      case 13:
        attachInterrupt(13, buttonInterrupt13, CHANGE);
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
  _timer.detach(); 
}

int MButton::getNum()
{
  return _num;
}

int MButton::getAction()
{
  return _action;
}

bool MButton::toProcess()
{
  return _toProcess;
}

void MButton::processed()
{
  _toProcess = false; 
}

void IRAM_ATTR MButton::buttonInterrupt12()
{
 if (_okButton && !_toProcess)  
  {
    _num = 12;
    _okButton = false;
    _timer.once_ms(DELAY_DEBOUNCE, timerInterrupt); 
  }
}

void IRAM_ATTR MButton::buttonInterrupt13()
{
 if (_okButton && !_toProcess)  
  {
    _num = 13;
    _okButton = false;
    _timer.once_ms(DELAY_DEBOUNCE, timerInterrupt); 
  }
}

void MButton::timerInterrupt()
{
  int level = digitalRead(_num);
  if (!_toProcess)
  {
    if (_oldLevel == HIGH)
    {
      if (level == HIGH)
      {
        // Short pressed
        _action = 1; 
        _toProcess = true;
      }
      else
      {
        // May start a long press action
        _oldNum = _num;  
        _oldTime = millis();
        _oldLevel = LOW;
      }
    }
    else if ((level == HIGH))
    {
      if (_num != _oldNum)  // Forbidden mixed case
      {
        _num = _oldNum; 
      }
      else
      {
        if (millis() >= (_oldTime + DELAY_LONG_PRESS))
        {
           // Long press 
          _action = 2; 
        }
        else
        {
          // Short press not long enough...
          _action = 1;
        }
        _oldLevel = HIGH,
        _toProcess = true;
       }
    }
  }
  _okButton = true; // Enable button interrupts
}
