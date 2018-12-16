class Button{
public: 

  Button( bool(*rf)(void), int triggerInterval = 250 ) : read_func(rf), triggerInterval(triggerInterval){}
  
  bool update(unsigned long now){
    lastNow = now; 
    bool state = read_func();
    if(state){
      pressed = true; 
      if(pressedSince==0) pressedSince = now;
      if(now-lastTrigger>triggerInterval){
        trigger = true; 
        lastTrigger = now; 
      }
      else{
        trigger = false; 
      }
    }
    else{
      pressed = false; 
      pressedSince = 0; 
      lastTrigger = 0; 
      trigger = false; 
      numSuccessTriggers = 0; 
    }

    return trigger; 
  }

  bool triggered(){
    return trigger; 
  }

  bool pressedLongerThan(int duration){
    return pressed && (lastNow-pressedSince) > duration; 
  }

  int successiveTriggers(){
    return numSuccessTriggers; 
  }

  bool (*read_func)(void);
  
private: 
  unsigned long pressedSince = 0;
  unsigned long lastTrigger = 0; 
  unsigned long triggerInterval; 
  unsigned long lastNow = 0; 
  bool pressed; 
  bool trigger = false; 
  int numSuccessTriggers = 0; 
  
}; 
