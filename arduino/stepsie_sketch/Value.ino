
bool needsCommit = false; 
unsigned long lastCommit = 0; 

class Value{
public: 
  Value(int defaultValue, int eepromAddress, int limitA, int limitB) : 
      defaultValue(defaultValue), lastStored(0), 
      eepromAddress(eepromAddress), limitA(limitA), limitB(limitB){
  }

  void load(){
    if(eepromAddress>=0){
      EEPROM_readAny(eepromAddress, storedValue); 
      storedValue = limit(storedValue); 
      value = storedValue; 
    }
  }
  
  operator int() const{
    return value; 
  }

  int get(){
    return value; 
  }

  int limit(int newValue){
    if(limitA != limitB){
      return constrain(newValue, limitA, limitB); 
    }
    else{
      return constrain(newValue,-1000,1000); 
    }
  }

  Value& operator =(const int & val){
    value = limit(val); 
    lastChanged = millis(); 
    return *this; 
  }

  void reset(){
    *this = defaultValue; 
  }

  bool update(unsigned long now){
    // after 5 sec, write to eeprom
    if(value != storedValue && now>lastStored && now-lastStored>5000 && now-lastChanged>5000 && eepromAddress>=0){
      EEPROM_writeAny(eepromAddress, value); 
      needsCommit = true; 
      storedValue = value; 
      lastStored = now; 
    }

    if(needsCommit && now-lastCommit>5000){
      EEPROM.commit(); 
      needsCommit = false; 
      lastCommit = now; 
    }
  }
  
private: 
  int eepromAddress; 
  int value; 
  int storedValue; 
  int defaultValue; 
  unsigned long lastStored = 0;
  unsigned long lastChanged = 0; 
  int limitA = 0; 
  int limitB = 0; 
  
  // eeprom tricks from https://forum.arduino.cc/index.php?topic=52159.0
  // we only use it for ints. still neat that it's templated
  typedef unsigned char byte;
  template <class T> int EEPROM_writeAny(int ee, const T& value){
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
    return i;
  }

  template <class T> int EEPROM_readAny(int ee, T& value){
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
    return i;
  }
}; 

