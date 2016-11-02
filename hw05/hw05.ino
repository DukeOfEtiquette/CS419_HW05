#include <SFE_MMA8452Q.h>
#include <Wire.h>
#include <MsTimer2.h>
#include <Arduino.h>

#define FLEX_PIN A0
#define VCC 4.98
#define R_DIV 10000.0
#define STRAIGHT_RES 30500.0
#define BEND_RES 75000.0

bool sensor = true; //ACC == true, FLX == false
bool sample = true; //50hz == true, 5hz == false
bool readAccl = false;

MMA8452Q accel;

double getSamplingTime()
{
  double hz;

  //If true then sample at 50hz
  //Else sample at 5hz
  if (sample)
    hz = 50;
  else
    hz = 5;

  //Convert frequency to milliseconds and return value
  return 1000 / hz;
}

void sampleAcc()
{
  //Set flag to true to let the main loop know it
  //needs to read the accelerometer
  readAccl = true;
}

void sampleFlex()
{
  //Gather the angle of the flex sensor
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0; //
  float flexR = R_DIV * (VCC / flexV - 1.0);
  float angle = map(flexR, STRAIGHT_RES, BEND_RES, 0, 90.0);

  //Send the info to the base station
  Serial.println(String(angle)); // degree of bend
}

void startSampling()
{
  //If sensor is true then sample the accel
  //Else sample the flex
  if (sensor)
    MsTimer2::set(getSamplingTime(), sampleAcc);
  else if (!sensor)
    MsTimer2::set(getSamplingTime(), sampleFlex);

  //Start the timer
  MsTimer2::start();
}

void setup() {

  Serial.begin(9600);

  //Set flex sensor pin to input and init the accel sensor
  pinMode(FLEX_PIN, INPUT);
  accel.init();

  delay(1000);
}

void loop() {

  //If something is available to read, then read it ya dingus
  if (Serial.available())
  {
    char arg = "";

    //While there is something in the serial buffer
    //pull it out one byte at a time
    while (Serial.available())
    {
      //Make sure serial has enough time to catch up
      delay(100);

      //Read the byte
      arg = Serial.read();

      //F is for FLEX
      if (arg == 'f')
        sensor = false;

      //S is for slow (5hz)
      if (arg == 's')
        sample = false;
    }

    //While statement is false which means we read everything
    //that was passed through the serial port so start sampling
    startSampling();
  }

  //Refer to discussion in office, this is a work around to the
  //accelerometer not working with MsTimer2
  if (readAccl)
  {
    String res = "";
    
    if (accel.available())
    {
      //Read data from accel
      accel.read();

      //Build up data so it is formatted "x,y,z"
      res += accel.cx;
      res += ",";
      res += accel.cy;
      res += ",";
      res += accel.cz;

      //Send the info to the base station
      Serial.println(res);

      //Reset flag
      readAccl = false;
    }
  }

}
