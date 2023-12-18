// MButton_test.ino

// Multi button (on pins 12 and 13 here) library test

#include "MButton.h"
#include <elapsedMillis.h>

#define DELAY_TEST 60000   // Delay for the test (ms)

MButton mButton;
elapsedMillis timerTest;
bool active;

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  mButton.begin(2, 12, 13);
  timerTest = 0;
  active = true;
  Serial.println("Test started");
}

void loop() 
{
  if (timerTest < DELAY_TEST)
  {
    if (mButton.toProcessM())
    {
      Serial.print(mButton.getNum());
      switch (mButton.getAction())
      {
        case BA_SHORT_PRESS:
          Serial.println(" short pressed");
          break;
        case BA_DOUBLE_PRESS:
          Serial.println(" double pressed");
          break;
        case BA_LONG_PRESS:
          Serial.println(" long pressed");
          break;  
      }
      mButton.processed();     
    } 
  }
  else if (active)
  {
    mButton.end();
    active = false;
    Serial.println("Test ended");
  }
  delay(500);
}
