
/*
 * Author: a forum user that I don't remember and matitalatina
 *
 */
 
 
//  define LED:
#define LEDpin 13

// define MIDI Parameters
#define btnON 144
#define btnOFF 128

// define Setup Time in ms(for optic sensors)
#define setupTime 2500

// Variables
/************************ 
 *  Define Hold Buttons *
 ************************/
// Set pins with this behaviour
int ButtonPin[5] = {
  2,3,4,5,6};

// Set numbers of inputs with this behavior
// it takes the first pins of the array setted above
int inputBtnNumber = 0;

// Do not Change
boolean ButtonValue[5] = {
  0,0,0,0,0};
boolean lastButtonValue[5]={
  0,0,0,0,0};
int noteButton[5]={
  10,11,12,13,14};


/********************************
 *  Define Toggle Type Buttons  *
 ********************************/
 // Set pins with this behaviour
int ButtonTogglePin[5] = {
  2,3,4,5,6};
  
// Set numbers of inputs with this behavior
// it takes the first pins of the array setted above
int inputBtnToggleNumber = 5;

// Do not change 
boolean ButtonToggleValue[5] = {
  0,0,0,0,0};
boolean lastButtonToggleValue[5]={
  0,0,0,0,0};
int noteToggleButton[5]={
  20,21,22,23,24};


/********************************
 *  Define Optic Type Analog    *
 ********************************/
 // They need calibration in setup
 
 // Set pins with this behaviour
int AnalogOptoPin[5] = {
  1,3,4,5,6};
  
// Set numbers of inputs with this behavior
// it takes the first pins of the array setted above
int inputAnalogOptoNumber = 1;

// Do Not Change
int lastAnalogOptoValue[5]={
  0,0,0,0,0};
int AnalogOptoMin[5]={
  999,999,999,999,999};
int AnalogOptoMax[5]={
  0,0,0,0,0};

int midiCCOptoselect[5] = {
  7,8,9,10,11};

/**********************************
 *  Define Type Analog (No Setup) *
 **********************************/
// define variables for the controller data
int AnalogValue[6] = {
  0,0,0,0,0,0};

// select number of desired analog inputs (max 6)
int input_no = 1;
  
// define the "lastValue" variables
int lastAnalogValue[6] = {
  0,0,0,0,0,0}; 

// select the midi Controller Number for each input
int midiCCselect[6] = {
  1,2,3,4,5,6};

// select threshold for each analog input  
int thresh[6] = {
  1,1,1,1,1,1};   

void setup() {
  //  set the states of the I/O pins:
  pinMode(LEDpin, OUTPUT);
  //  Set MIDI baud rate:
  Serial.begin(57600);

  // calibrate Optic Sensors during the first time
  digitalWrite(LEDpin, HIGH);
  while (millis() < setupTime) {
    for (int i=0;i<inputAnalogOptoNumber;i++) {
      int sensorValue = analogRead(AnalogOptoPin[i]);

      // record the maximum sensor value
      if (sensorValue > AnalogOptoMax[i]) {
        AnalogOptoMax[i] = sensorValue;
      }

      // record the minimum sensor value
      if (sensorValue < AnalogOptoMin[i]) {
        AnalogOptoMin[i] = sensorValue;
      }
    }
  }
  digitalWrite(LEDpin, LOW);

}

// main program loop
void loop() {

  //Analog Input
  for(int i=0;i<input_no;i++){

    AnalogValue[i] = (analogRead(i))/8;    // read the value from the analog input and divide by 8 for range 0-127
    // check if value is greater than defined threshold (good for resistive touchpads etc)
    if ( AnalogValue [i]>thresh[i] ) {
      // check if analog input has changed
      if ( AnalogValue[i] != lastAnalogValue[i] ) {

        //send control change on cc#i
        midiCC(0xB0, midiCCselect[i], AnalogValue[i]);
        // update lastAnalogValue variable
        lastAnalogValue[i] = AnalogValue[i];

        //End if
      }
      //End if
    }

    //Analog Optic Input
    for(int i=0;i<inputAnalogOptoNumber;i++){

      int valueAnalogOpto = map(analogRead(AnalogOptoPin[i]), AnalogOptoMin[i], AnalogOptoMax[i], 0, 127);


      if ( valueAnalogOpto != lastAnalogOptoValue[i] ) {

        //send control change on cc#i
        midiCC(0xB0, midiCCOptoselect[i], valueAnalogOpto);
        // update lastAnalogValue variable
        lastAnalogOptoValue[i] = valueAnalogOpto;

        //End if
      }
    }


    // Button Hold Input
    for (int i=0;i<inputBtnNumber;i++) {
      int stateButton = digitalRead(ButtonPin[i]);

      if (stateButton != lastButtonValue[i]) {
        if (stateButton == 1)
          midiCC(btnON, noteButton[i], 80);
        else
          midiCC(btnOFF, noteButton[i], 80);   
        // update lastAnalogValue variable
        lastButtonValue[i] = stateButton;
      }        
    }

    // Button Toggle Input
    for (int i=0;i<inputBtnToggleNumber;i++) {
      int stateButton = digitalRead(ButtonTogglePin[i]);

      if (stateButton != lastButtonToggleValue[i]) {
        if (stateButton == HIGH) {
          ButtonToggleValue[i] = !ButtonToggleValue[i];
          if (ButtonToggleValue[i] == 1)
            midiCC(btnON, noteToggleButton[i], 80);
          else
            midiCC(btnOFF, noteToggleButton[i], 80);
        }  
        // update lastAnalogValue variable
        lastButtonToggleValue[i] = stateButton;
      }        
    }


    //End for 
  }


  //End Loop
}



// This function sends a Midi data. 
void midiCC(char CC_data, char c_num, char c_val){
  Serial.write(CC_data);
  Serial.write(c_num);
  Serial.write(c_val);

}



