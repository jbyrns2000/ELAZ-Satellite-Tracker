

//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 12;  //connect pin  to step
int dir = 13;  // connect pin  to dir
int stp2 = 10;  //connect pin  to step
int dir2 = 11;  // connect pin  to dir

bool DEBUG = false;

int iElDelay = 750 ;
int iAzDelay = 1000 ;

int iCurrentEl = 0;
int iCurrentAz = 0;


    

//int iElevationDegree = int(1600/90 * 16); //? steps per degree
//int iAzimuthDegree = int(1000/360 * 16); //? steps per degree 13760  7.3

int iElevationDegree = 270; //? steps per degree
int iAzimuthDegree = 38; //? steps per degree


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String sOutput = "+0000+0000";

//Circle TesT
//x squared + b squared =  c squared
// 5 degree radius circle would be
// x * x  + y * y = 25


void setup() 
{                
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);    
  pinMode(stp2, OUTPUT);
  pinMode(dir2, OUTPUT);   

  
  
  Serial.begin(9600); 
  if (DEBUG) Serial.println("Initializing System");
  //mySerial.println("+0051+0044");
}


void serialEvent() {
  int lf = 13;
  while (Serial.available()) {
    inputString = Serial.readStringUntil(lf);
    
      stringComplete = true;
    }
  }

void GotoElevation(int iEl)
{
  unsigned int iGotoSteps =0;
  unsigned int iStepCount = 0;
  int iDelay = 0;
  
  if (iEl <= 180 && iEl >= 0){
    //if (DEBUG) Serial.println ("Elevation within scope.");
    if (iEl > iCurrentEl){
      //if (DEBUG) Serial.println ("Going up!");
      digitalWrite(dir2, LOW);
      iGotoSteps = iElevationDegree * (iEl - iCurrentEl);
      //if (DEBUG) Serial.println (iGotoSteps);
    }
    if (iEl < iCurrentEl){
       //if (DEBUG) Serial.println ("Going Down!");
      digitalWrite(dir2, HIGH);
      iGotoSteps = iElevationDegree * (iCurrentEl - iEl );
      //if (DEBUG) Serial.println (iGotoSteps);
    }
    if (iEl == iCurrentEl){
      //if (DEBUG) Serial.println ("Not going to move.");
      return;
    }
    
    iCurrentEl = iEl;
    if (DEBUG) Serial.println ("Current Elevation:");
    if (DEBUG) Serial.println (iCurrentEl);
    delayMicroseconds(iElDelay);

    
    int iPercent = int(iGotoSteps * .005); //  a percentage of the steps to move
    iDelay = iElDelay + iPercent ;    //  a percentage for the lowest of the main delay.
    int iDivision =  1;
    
    

    
    
    for(iStepCount = 0;iStepCount <= iGotoSteps ;iStepCount++) {
      if (iStepCount <= iPercent){
        iDelay = iDelay - iDivision ;
      }
      else if (iStepCount > (iGotoSteps-iPercent)){
        iDelay = iDelay + iDivision;
      }

      else {
        iDelay = iElDelay;
      }
      
      digitalWrite(stp2, HIGH);   
      delayMicroseconds(iDelay);               
      digitalWrite(stp2, LOW);   
      delayMicroseconds(iDelay/2);
    }
    
  }
}

void GotoAzimuth(int iAz)
{
  unsigned int iGotoSteps =0;
  unsigned int iStepCount = 0;
  int iDelay = 0;
  
  if (iAz <= 360 && iAz >= 0){
    if (iAz == iCurrentAz){
      return;
    }
    if (iAz > iCurrentAz){
      digitalWrite(dir, HIGH);
      iGotoSteps = iAzimuthDegree * (iAz - iCurrentAz);
    }
    if (iAz < iCurrentAz){
      digitalWrite(dir, LOW);
      iGotoSteps = iAzimuthDegree * (iCurrentAz - iAz);
    }
    
   
    
    int iPercent = int(iGotoSteps * .30); //  a percentage of the steps to move
    iDelay = iAzDelay + iPercent ;    //  a percentage for the lowest of the main delay.
    int iDivision =  1;
    

    if (DEBUG) Serial.println(iGotoSteps);
    if (DEBUG) Serial.println(iPercent);
    if (DEBUG) Serial.println(iDelay);
    if (DEBUG) Serial.println(iDivision);
    
    iCurrentAz = iAz ;
    delayMicroseconds(iAzDelay);
    
    for(iStepCount = 0;iStepCount <= iGotoSteps ;iStepCount++) {

      if (iGotoSteps > 180){
        if (iStepCount <= iPercent){
          iDelay = iDelay - iDivision ;
          
        }
        else if (iStepCount >= (iGotoSteps-iPercent)){
          iDelay = iDelay + iDivision;
        }
        else {
          iDelay = iAzDelay;
        }
      }
      
      digitalWrite(stp, HIGH);   
      delayMicroseconds(iDelay);               
      digitalWrite(stp, LOW);   
      delayMicroseconds(iDelay/2);
    }
  }
}

void loop() 
{
    
  String AZ = "";
  String EL = "";
  serialEvent();

    if (stringComplete)
  {
    
    if (DEBUG) Serial.println(inputString);//display number of character received in readData variable.
    if (DEBUG) Serial.println(inputString[0]);
    
    switch (inputString[0]) {
      case 'H':
        if (inputString[1] == '2') {
          Serial.println("");
          Serial.println("U  UP Direction Rotation (Not Implemented)");
          Serial.println("D  DOWN Direction Rotation (Not Implemented)");
          Serial.println("E  UP/DOWN Direction Rotation Stop (Not Implemented)");
          Serial.println("C2 Antenna Direction Value");
          Serial.println("W  Antenna Direction Setting.");
          Serial.println("T  Start Command in the time interval direction setting (Not Implemented)");
          Serial.println("N  Total number of setting angle in “W” mode and traced (Not Implemented)");
          Serial.println("S  All Stop");
          Serial.println("O2 Offset Calibration (Not Implemented)");
          Serial.println("F2 Full Scale Calibration (Not Implemented)");
          Serial.println("B  Elevation Antenna Direction Value");
        } else {
            Serial.println("");
            Serial.println("R  Clockwise Rotation (Not Implemented)");
            Serial.println("L  Counter Clockwise Rotation (Not Implemented)");
            Serial.println("A  CW/CCW Rotation Stop (Not Implemented)");
            Serial.println("C  Antenna Direction Value");
            Serial.println("M  Antenna Direction Setting. MXXX (Not Implemented)");
            Serial.println("T  Start Command in the time interval direction setting (Not Implemented)");
            Serial.println("N  Total number of setting angles in “M” mode and traced (Not Implemented)");
            Serial.println("X1 Rotation Speed 1 (Horizontal) Low (Not Implemented)");
            Serial.println("X2 Rotation Speed 2 (Horizontal) Middle 1 (Not Implemented)");
            Serial.println("X3 Rotation Speed 3 (Horizontal) Middle 2 (Not Implemented)");
            Serial.println("X4 Rotation Speed 4 (Horizontal) High");
            Serial.println("S  All Stop");
            Serial.println("O  Offset Calibration  (Not Implemented)");
            Serial.println("F  Full Scale Calibration  (Not Implemented)");
        }
        break;
      case 'B':
          if (DEBUG) Serial.println("Elevation Status Request...");
          
          Serial.println(sOutput.substring(5,10));
        break;
      case 'C':
        if (inputString[1] == '2') {
          if (DEBUG) Serial.println("Status Request...");
          Serial.println(sOutput);
        } else {
            if (DEBUG) Serial.println("Azimuth Status Request...");
            Serial.println(sOutput.substring(0,5));
        }
        
        break;
      case 'M':
        
        break;
      case 'S':
        if (DEBUG) Serial.println("Stop command received.");
        Serial.println("");
        break;
      case 'X':
        if (inputString[1] == '4') if (DEBUG) Serial.println("Speed 4...");
        Serial.println("");
        break;
      case 'W':
        if (DEBUG) Serial.println("Goto Command Received.");
        //Serial.println(SerialIn);
        AZ = inputString.substring(1,4);
        EL = inputString.substring(5,8);

        if (DEBUG) Serial.println(EL);
        sOutput = "+0";
        sOutput += AZ;
        sOutput += "+0";
        sOutput += EL;
        
        if (DEBUG) Serial.println(sOutput);
        Serial.println("");
        
        GotoElevation (EL.toInt());
        GotoAzimuth (AZ.toInt());
        break;
      case 'D':
          if (inputString[1] == '3') DEBUG = !DEBUG;
          if (DEBUG) Serial.println("Toggle Debug");
          break;
      default:
        if (DEBUG) Serial.println("Command not recognized.");
        Serial.println("?>");
    } //end switch
    
    stringComplete = false;
    inputString = "";   
  }
}
