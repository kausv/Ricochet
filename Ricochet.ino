#include "blinklib.h"
#include "blinkstate.h"

// Spin around in goovy HSB color space
// HSB stands for Hue, Saturation, and Brightness
uint8_t litLed = 0;
uint8_t neighborState[6];
bool sendBall, hasBall, changeDir;
const uint32_t INTERVAL = 100;
uint32_t maxTrySend = INTERVAL;


void setup() {

  // No setup needed for this simple example!  

  setColor(BLUE);
  delay(500);
  setColor(OFF);
  delay(500);
}


void loop() {

  while(1) {
    if (buttonLongPressed()) {
      break;
    }
    if (buttonSingleClicked()) {
      changeDir = true;
    }
    if (buttonDoubleClicked()) {
      if (hasBall) {
        litLed = litLed + 1;
      }
      else
      {
        setFaceColor( litLed , RED );  
        hasBall = true;
        delay(500);  
      }
    }
  
    else {
      FOREACH_FACE(x) {
        neighborState[x] = getNeighborState(x);
      
        // Recieved data
        if (neighborState[x] == 2 && !hasBall) {
          
          hasBall = true;
          litLed = x;
          sendBall = false;
          while (getNeighborState(litLed) != 1)
          {
            irSendData(litLed, 3);
          }
        }
        else if (neighborState[x] == 3) {
          hasBall = false;
          setColor(OFF);
          irSendData(x, 1);
        }
      }
    }
    
    
    if (hasBall){    
      if(!sendBall) {
        setColor(OFF);
        setFaceColor( litLed , RED );  
        sendBall = true;    
      }
      else {
        litLed = (litLed+3)%6;
        if (changeDir) {
          litLed += 1;
          changeDir = false;
        }
        setColor(OFF);
        setFaceColor( litLed , RED );
        uint32_t timer = millis();
        while (millis() - timer < INTERVAL)
        {
          irSendData(litLed, 2);
          if (getNeighborState(litLed) == 3 || getNeighborState(litLed) == 3) {
            setColor(OFF);
            sendBall = false;
            hasBall = false;
            break;
          }
        }
      }
    }

    delay(INTERVAL);
  }
}


