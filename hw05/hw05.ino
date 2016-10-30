#include <SFE_MMA8452Q.h>
#include <MsTimer2.h>

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