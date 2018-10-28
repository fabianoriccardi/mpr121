#include <MPR121.h>
#include <Wire.h>

// this is the touch threshold - setting it low makes it more like a proximity trigger
// default value is 40 for touch
const int touchThreshold = 40;
// this is the release threshold - must ALWAYS be smaller than the touch threshold
// default value is 20 for touch
const int releaseThreshold = 20;

void setup(){  
  Serial.begin(115200);
  while(!Serial); // only needed for Arduino Leonardo or Bare Touch Board 
  
  if(!MPR121.begin(0x5A)){ 
    Serial.println("error setting up MPR121");  
    switch(MPR121.getError()){
      case NO_ERROR:
        Serial.println("no error");
        break;  
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;      
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;      
    }
    while(1);
  }

  MPR121.setTouchThreshold(0);
  MPR121.setReleaseThreshold(0);
  // Set the electrode's baselines
  for(int i=0; i<13;i++){
    MPR121.setBaseline(i,900+50*i);
  }
  MPR121.setCalibrationLock(CAL_LOCK_DISABLED);
}

void loop(){
   readRawInputs();  
}

void readRawInputs(){
    int i;
    
    MPR121.updateAll();

    // True or False
    Serial.print("TOUCH: ");
    for(i=0; i<13; i++){          // 13 touch values
      Serial.print(MPR121.getTouchData(i), DEC);
      if(i<12) Serial.print(" ");
    }    
    Serial.println();   

    // The previous configured touch thresholds
    Serial.print("TTHS: ");
    for(i=0; i<13; i++){          // 13 touch thresholds
      Serial.print(MPR121.getTouchThreshold(i), DEC); 
      if(i<12) Serial.print(" ");
    }   
    Serial.println();

    // The previous configured release thresholds
    Serial.print("RTHS: ");
    for(i=0; i<13; i++){
      Serial.print(MPR121.getReleaseThreshold(i), DEC); 
      if(i<12) Serial.print(" ");
    }   
    Serial.println();

    // Actual filtered values
    Serial.print("FDAT: ");
    for(i=0; i<13; i++){
      Serial.print(MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();

    // Actual baseline
    Serial.print("BVAL: ");
    for(i=0; i<13; i++){
      Serial.print(MPR121.getBaselineData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();
    
    // the trigger and threshold values refer to the difference between
    // the filtered data and the running baseline - see p13 of 
    // http://www.freescale.com/files/sensors/doc/data_sheet/MPR121.pdf
    
    Serial.print("DIFF: ");
    for(i=0; i<13; i++){          // 13 value pairs
      Serial.print(MPR121.getBaselineData(i)-MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }           
    Serial.println();

    delay(500);
}
