#include <SFE_MMA8452Q.h>
#include <MsTimer2.h>

#define FLEX_PIN A0
#define VCC 4.98
#define R_DIV 10000.0
#define STRAIGHT_RES 30500.0
#define BEND_RES 75000.0

bool sensor = true; //ACC == true, FLX == false
bool sample = true; //50hz == true, 5hz == false

//TODO(adam): Update to not be based off the rheostat
double getSamplingTime()
{
  int hz;
  if(sample)
    hz = 50;
  else
    hz = 5;
  //Interpolate rheostate value to desired Hz value
  double hz = ( ( (MAX_HZ - MIN_HZ) /1023) * hz ) + MIN_HZ;

  //Convert frequency to milliseconds and return value
  return 1000 / hz;
}

void sampleAcc()
{
}

void sampleFlex()
{
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0
  float flexR = R_DIV * (VCC / flexV - 1.0);
  Serial.println("Resistance: " + String(FlexR) + " ohms");

  float angle = map(flexR, STRAIGHT_RES,  BEND_RES, 0, 90.0);
  Serial.println("Bend: " + String(angle) + " degrees");
  Serial.println();
  
}

void startSampling()
{
  if(sensor)
     if(sample)
        MsTimer2::set(getSamplingTime(), sampleACC);
     else
        MsTimer2::set(getSamplingTime(), sampleACC);
  else if(!sensor)
     if(sample)
        MsTimer2::set(getSamplingTime(), sampleFlex);
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
  if(arg == F)
    sensor = false;

  arg = Serial.read();
  delay(100); //Give the serial port some time to catch up

  //S is for slow (5hz)
  if(arg == S)
    sample = false;

  startSampling();
}

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  

}

void loop() {
  //While serial port has nothing in buffer, busy wait

  //Wait until something comes over the serial port
  while(Serial.available() < 1){}

  char arg = Serial.read();
  delay(100);

  //N == new sampling request
  //T == stop current sampling
  if(arg == N)
     getSamplingInfo();
  else if(arg == T)
     stopSampling();

}
