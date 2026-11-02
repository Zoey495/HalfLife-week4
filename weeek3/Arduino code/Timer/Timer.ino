//Timer: Set the time!
//---------------------------------------------
//Libraries
#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object
//Variables
int count = 0;
int restartCount = 0;
float min = 0;
bool OnOff = false;
unsigned long time_remaining = 0;
unsigned long elapsed_Time;
int buttonTime ;
int buttonOnOff ;
//-------------------------------------------------------------
void setup(){
  byte numDigits = 4;
  byte digitPins[] = {17, 16, 15, 14}; //The digit pins in order of 1 - 4
  byte segmentPins[] = {4, 5, 6, 7, 9, 10, 11, 12}; //Pins in order of a - DPX for segments
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = N_TRANSISTORS; // Means I used NPN transistors with common cathode
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
}

//---------------------------------------------
void Time_Set(){  //Change the minutes
  if (buttonTime == LOW && count >= 5 ){
    min--;
    if (min < 0){
      min = 99;
    }
  }else if (buttonTime == LOW ){
    min++;
    count++;
    if (min > 99){
      min = 0;
    }
  }
  else if (buttonTime == HIGH){
    count = 0;
  }
}


bool OnOff_Set(){ //Turn on/off or restart timer
  
  if (buttonOnOff == LOW && OnOff == true){ //Means stop timer
     return false;
  }
  else if (buttonOnOff == LOW){
    sevseg.refreshDisplay(); // Must run repeatedly
    delay(2000);
    sevseg.refreshDisplay(); // Must run repeatedly
    buttonOnOff = digitalRead(22);
    if (buttonOnOff == LOW){
      restartTimer();
      return false;
    }
    time_remaining = (unsigned long)min * 60;
    elapsed_Time = millis();
    return true; //means start timer
  }else {
    return OnOff;
  }
  
}


void restartTimer(){
  min = 0;
}


//--------------------------------------------
  void loop(){
    buttonTime = digitalRead(21);
    buttonOnOff = digitalRead(22);

    OnOff = OnOff_Set();

    if (OnOff == false){
      Time_Set();
    }
    else{
      if (millis() - elapsed_Time >= 1000) {// This subtracts total time running from recorded time of starting countdown to make sure a second actually took place
        elapsed_Time += 1000; //Adding onto elapsed time so that it keeps up wih millis
      
        if (time_remaining > 0){
          time_remaining--;
          unsigned long displayNum = (time_remaining/60 * 100) + (time_remaining % 60);
          sevseg.setNumber(displayNum,2);
        }else{
        OnOff = false;
        sevseg.setNumber(0000,0);
        restartTimer();
        }
      }
      
    }

    sevseg.refreshDisplay(); // Must run repeatedly
  }