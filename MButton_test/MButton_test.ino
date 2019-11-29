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
    if (mButton.toProcess())
    {
      Serial.print(mButton.getNum());
      switch (mButton.getAction())
      {
        case 1:
          Serial.println(" short pressed");
          break;
        case 2:
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
