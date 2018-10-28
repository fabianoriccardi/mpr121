/**
 * A simple example to show the manual calibration of electrodes.
 */
#include <Wire.h>
#include <MPR121.h>

void setup(){  
  Serial.begin(115200);
  while(!Serial);

  Wire.begin();
  
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

  // In this case, we don't really need touch-release thresholds...
  MPR121.setTouchThreshold(0);
  MPR121.setReleaseThreshold(0);
  
  // Set the electrode's baselines
  for(int i=0; i<13;i++){
    // NOTE: the values can be only multiple of 4, 
    // otherwise they are floored to the nearest multiple
    MPR121.setBaseline(i,50+50*i);
  }
  MPR121.setCalibrationLock(CAL_LOCK_DISABLED);
}

void loop(){
   readRawInputs();  
}

void readRawInputs(){
    MPR121.updateAll();

    // True or False
    Serial.print("TOUCH: ");
    for(int i=0; i<13; i++){
      Serial.print(MPR121.getTouchData(i), DEC);
      if(i<12) Serial.print(" ");
    }    
    Serial.println();

    // Actual filtered values
    Serial.print("FDAT: ");
    for(int i=0; i<13; i++){
      Serial.print(MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();

    // Actual baseline
    Serial.print("BVAL: ");
    for(int i=0; i<13; i++){
      Serial.print(MPR121.getBaselineData(i), DEC);
      if(i<12) Serial.print(" ");
    } 
    Serial.println();
    
    Serial.print("DIFF: ");
    for(int i=0; i<13; i++){          // 13 value pairs
      Serial.print(MPR121.getBaselineData(i)-MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }           
    Serial.println();

    Serial.println();

    // Fair delay to avoid overhelming messages
    delay(500);
}
