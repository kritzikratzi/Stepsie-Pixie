
const int ee_numSteps = 0; // 0..3 
const int ee_stepSpeed = 4;  // 4..7
const int ee_interval = 8; // 8..11
const int ee_record = -1;  // not stored!

#include <vector>


Value numSteps{10, ee_numSteps,-10000,10000}; 
Value stepSpeed{10, ee_stepSpeed,1,60}; 
Value interval{5, ee_interval,1,10000}; 
Value record{0, ee_record,0,0}; 


std::vector<Value*> values({&numSteps, &stepSpeed, &interval, &record}); 

// eeprom address to store settings


Value * editValue = &numSteps; 

Button btnUp([](){ return (bool)!digitalRead(BTN1_GPIO);}); 
Button btnDown([](){ return (bool)!digitalRead(BTN2_GPIO);}); 
Button btnNext([](){ return analogRead(A0) < 512;}); 

typedef enum State{
  STEP_INIT,
  STEP_DRIVE, 
  STEP_PICTURE
} State; 

State state = STEP_INIT; 
unsigned long stepStartMillis = 0; 
unsigned long stepRemainingMillis = 0; 
unsigned long stepRemainingPct = 0; 
unsigned long nextLoopTime = 0; 

int stepsDriven = 0; 
bool shouldTakePicture = false; 

void setupMenu(){
  // Initialising the UI will init the display too.
  display.init();
  delay(100); 
  display.clear(); 
  display.init(); 
  delay(100); 
  
  if(DISPLAY_ROTATE){
    display.flipScreenVertically();
  }
  display.setFont(ArialMT_Plain_10);

  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5, 0, "It's timelapse time! ...");

  display.display();


  for( auto value : values){
    value->load(); 
  }

  record = false; 
}

void updateMenu(){
  stepRemainingPct = random(100); 
  
  unsigned long now = millis(); 
  btnNext.update(now); 
  btnUp.update(now); 
  btnDown.update(now); 
  for(auto value : values){
    value->update(now); 
  }
  if(btnNext.triggered()){
    if(btnNext.pressedLongerThan(5000)){
      // reset to defaults! 
      for(auto value : values){
        value->reset(); 
      }
    }
    
    if(editValue == &numSteps ) editValue = &stepSpeed; 
    else if(editValue == &stepSpeed ) editValue = &interval; 
    else if(editValue == &interval ) editValue = &record; 
    else editValue = &numSteps; 
  }

  auto incValue = [](Button & btn, Value & value, int increment){
    if(btn.triggered() && btn.pressedLongerThan(5000)){
      increment *= 10;
    }
    value = value + increment; 
  }; 
  

  if(btnUp.triggered()){
    incValue(btnUp, *editValue, +1); 
  }
  
  if(btnDown.triggered()){
    incValue(btnDown, *editValue, -1); 
  }
  
  record = ((record%2)+2)%2; 
  digitalWrite(ENABLE_PIN, record?HIGH:LOW); 

  if(record){
    unsigned long T = std::max(1,std::max(interval.get(), -interval.get()))*1000; 
    if(nextLoopTime == 0) nextLoopTime = now + T; 
    
    switch(state){
      case STEP_INIT: {
        stepStartMillis = T; 
        state = STEP_DRIVE; 
        stepsDriven = 0; 
        break; 
      }
      case STEP_DRIVE: {
        bool reverse = numSteps<0; 
        int numStepsPos = reverse?-numSteps:numSteps; 
        int motorDuration = constrain((numStepsPos*1000)/stepSpeed/4/*?*/, 1,std::max(numStepsPos*50,(int)T-500)); // 500
        int stepsWanted = constrain(map(T-(nextLoopTime-now), 0,motorDuration,  0,numStepsPos), 0, numStepsPos); 
        int driveNow = std::max(0,stepsWanted-stepsDriven);
        stepper.setSpeed(std::max(10,(int)stepSpeed));
        driveNow = std::min(driveNow,10); // never more than 10 steps ^
        stepper.step(reverse?-driveNow:driveNow); 
        stepsDriven += driveNow; 
        
        if(now>nextLoopTime-500){
          //drive remaining steps
          state = STEP_PICTURE; 
          shouldTakePicture = true; 
        }
        break; 
      }
      case STEP_PICTURE:
        if(shouldTakePicture){
          digitalWrite(TRIGGER_PIN, HIGH); 
          delay(50); 
          digitalWrite(TRIGGER_PIN, LOW); 
          triggerServer(); 
          shouldTakePicture = false; 
        }
        
        if(now>=nextLoopTime){
          state = STEP_INIT; 
          nextLoopTime += T; 
        }
        break; 
    }

    stepRemainingMillis = nextLoopTime<now?0:(nextLoopTime-now); 
    stepRemainingPct = std::min(100,(int)(100-stepRemainingMillis*100/T)); 
  }
  else{
    state = STEP_INIT; 
    nextLoopTime = 0; 
    stepRemainingPct = 0; 
    stepRemainingMillis = 0; 
  }
}


void drawMenu(){
  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5, 0, "Steps");
  display.drawString(5,10, "Speed"); 
  display.drawString(5,20, "Interval"); 
  display.drawString(5,54, record?"Run ...":"Stopped"); 

  if(editValue == &numSteps) display.drawString(0,2, "*"); 
  display.drawString(50, 0, String(numSteps)); 
  if(editValue == &stepSpeed) display.drawString(0,12, "*"); 
  display.drawString(50,10, String(stepSpeed)); 
  if(editValue == &interval) display.drawString(0,22, "*"); 
  display.drawString(50,20, String(interval)); 
  if(editValue == &record) display.drawString(0,56, "*"); 


  if(record){
    display.drawProgressBar(50, 55, 50, 8, std::min(100,(int)stepRemainingPct));
  }
  
  display.drawString(80,20,"sec"); 

  display.drawFastImage(118,0, 8, 8, icon_tri_up); 
  display.drawFastImage(118,30, 8, 8, icon_tri_down); 
  display.drawFastImage(118,52, 8, 8, icon_tri_next); 
  
  display.display();  
}

