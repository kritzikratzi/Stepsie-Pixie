//stephan

#define DISPLAY_SDA 4
#define DISPLAY_SCL 5

#define BTN1_GPIO 0
#define BTN2_GPIO 1
#define BTN3_GPIO 16



#define STEPPER_M1 14
#define STEPPER_M2 12
#define STEPPER_M3 13
#define STEPPER_M4 3

#define TRIGGER_PIN 2
#define ENABLE_PIN 15

#define DISPLAY_ROTATE 1

// ai-thinker variant
// btn1 on pin "rx"
/*#define DISPLAY_SDA 5
#define DISPLAY_SCL 4

#define BTN1_GPIO 3
#define BTN2_GPIO 1
#define BTN3_GPIO 0

// // 14=D5, 12=D6, 13=D7, 15=D8
#define STEPPER_M1 14
#define STEPPER_M2 13
#define STEPPER_M3 12 
#define STEPPER_M4 15

#define TRIGGER_PIN D0
#define DISPLAY_ROTATE 1

*/

//WIFI Configuration (also see server tab)
#define ACCESSPOINT_SSID "controller-1"
#define ACCESSPOINT_PASS "youkidouki"

#include <TimeLib.h>
#include <Wire.h>
#include "SH1106Wire.h"
#include <Stepper.h>
//h#include <HalfStepper.h>
#include <EEPROM.h>

// Initialize the OLED display using Wire library
SH1106Wire display(0x3c, DISPLAY_SDA, DISPLAY_SCL); // 5=D1, 4=D2

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper stepper(stepsPerRevolution, STEPPER_M1, STEPPER_M2, STEPPER_M3, STEPPER_M4); 


#include "OLEDDisplayUi.h"
OLEDDisplayUi ui ( &display );

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ENABLE_PIN, OUTPUT); 


  digitalWrite(ENABLE_PIN, LOW); 
//  ESP.wdtDisable();
//  ESP.wdtEnable(WDTO_8S);

//  EEPROM.begin(512); 
    setupMenu(); 
//  funnySetup(); 
  
//  setupServer(); 

  
  //pinMode(BTN1_GPIO, INPUT_PULLUP); 
  pinMode(BTN1_GPIO, INPUT_PULLUP);
  pinMode(BTN2_GPIO, INPUT_PULLUP); 
  //pinMode(BTN3_GPIO, INPUT_PULLUP); 
}

void loop() {
//  updateServer(); 
  updateMenu(); 
  drawMenu(); 
//  delay(1);
//  yield(); 

//  funnyLoop(); 
}
