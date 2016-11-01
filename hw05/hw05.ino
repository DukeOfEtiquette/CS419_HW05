#include <SFE_MMA8452Q.h>
#include <Wire.h>
#include <SFE_MMA8452Q.h>
#include <MsTimer2.h>

#define FLEX_PIN A0
#define VCC 4.98
#define R_DIV 10000.0
#define STRAIGHT_RES 30500.0
#define BEND_RES 75000.0

bool sensor = true; //ACC == true, FLX == false
bool sample = true; //50hz == true, 5hz == false

MMA8452Q accel;


String readString;


//TODO(adam): Update to not be based off the rheostat
double getSamplingTime()
{
  double hz;
  if(sample)
    hz = 50;
  else
    hz = 5;
    
  //Convert frequency to milliseconds and return value
  return 1000 / hz;
}

void sampleAcc()
{
  
  accel.read();
  int xAcceleration = accel.x; // Read in raw x-axis
  int yAcceleration = accel.y; // Read in raw y-axis
  int zAcceleration = accel.z; // Read in raw z-axis

  // print the values
  Serial.print(xAcceleration + " ");
  Serial.print(yAcceleration + " ");
  Serial.print(zAcceleration + " ");  

  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
      Serial.print("Portrait Up");
      break;
  case PORTRAIT_D:
      Serial.print("Portrait Down");
      break;
  case LANDSCAPE_R:
      Serial.print("Landscape Right");
      break;
  case LANDSCAPE_L:
      Serial.print("Landscape Left");
      break;
  case LOCKOUT:
      Serial.print("Flat");
      break;
  }
  Serial.print("\n");
}

void sampleFlex()
{
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  Serial.println("Resistance: " + String(flexR) + " ohms");

  float angle = map(flexR, STRAIGHT_RES,  BEND_RES, 0, 90.0);
  Serial.println("Bend: " + String(angle) + " degrees");
  Serial.println();
}

void startSampling()
{
  if(sensor)
    MsTimer2::set(getSamplingTime(), sampleAcc);
  else 
     MsTimer2::set(getSamplingTime(), sampleFlex);

  MsTimer2::start();
}

void stopSampling()
{
  MsTimer2::stop();
}

void getSamplingInfo()
{
  char arg;

  arg = Serial.read();
  delay(100);

  //F is for FLEX
  if(arg == 'F')
    sensor = false;

  arg = Serial.read();
  delay(100); //Give the serial port some time to catch up

  //S is for slow (5hz)
  if(arg == 'S')
    sample = false;

  startSampling();
}

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  accel.init();
}

void loop() {
  //While serial port has nothing in buffer, busy wait
  //while(!Serial.available()){}
  
  //Wait until something comes over the serial port
  while(Serial.available() < 1){}

  char arg = Serial.read();
  delay(100);

  //N == new sampling request
  //T == stop current sampling
  if(arg == 'N')
     getSamplingInfo();
  else if(arg == 'T')
     stopSampling();

}
