/* // wemos variant
#define DISPLAY_SDA 5
#define DISPLAY_SCL 4

#define BTN1_GPIO D5
#define BTN2_GPIO D6
#define BTN3_GPIO D7

#define STEPPER_M1 D3
#define STEPPER_M2 D4
#define STEPPER_M3 D8
#define STEPPER_M4 D9

#define DISPLAY_ROTATE 1


#define TRIGGER_PIN 10*/


/* // martin variant

#define DISPLAY_SDA 5
#define DISPLAY_SCL 4

#define BTN1_GPIO D5
#define BTN2_GPIO D7
#define BTN3_GPIO D6

#define STEPPER_M1 D3
#define STEPPER_M2 D4
#define STEPPER_M3 D8
#define STEPPER_M4 D9

#define TRIGGER_PIN D10

#define DISPLAY_ROTATE 0
*/

//stephan

#define DISPLAY_SDA 5
#define DISPLAY_SCL 4

#define BTN1_GPIO D3
#define BTN2_GPIO D10
#define BTN3_GPIO D0 //wäre A0

#define STEPPER_M1 D5
#define STEPPER_M2 D6
#define STEPPER_M3 D7
#define STEPPER_M4 D9

#define TRIGGER_PIN D4
#define ENABLE_PIN D8

#define DISPLAY_ROTATE 0

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

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <Stepper.h>
//h#include <HalfStepper.h>
#include <EEPROM.h>

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, DISPLAY_SDA, DISPLAY_SCL); // 5=D1, 4=D2

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper stepper(stepsPerRevolution, STEPPER_M1, STEPPER_M2, STEPPER_M3, STEPPER_M4); 



void setup() {
  pinMode(TRIGGER_PIN, OUTPUT); 

  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);

  EEPROM.begin(512); 
    setupMenu(); 
  delay(100); 

  
//  setupServer(); 

  
  //pinMode(BTN1_GPIO, INPUT_PULLUP); 
  pinMode(BTN1_GPIO, INPUT_PULLUP);
  pinMode(BTN2_GPIO, INPUT_PULLUP); 
  pinMode(BTN3_GPIO, INPUT_PULLUP); 
}

void loop() {
//  updateServer(); 
  updateMenu(); 
  drawMenu(); 
//  delay(1);
//  yield(); 
}
