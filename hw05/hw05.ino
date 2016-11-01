#include <SFE_MMA8452Q.h>
#include <MsTimer2.h>
#include <Arduino.h>

bool sensor = true; //ACC == true, FLX == false
bool sample = true; //50hz == true, 5hz == false
String arg;

//TODO(adam): Update to not be based off the rheostat
double getSamplingTime()
{
  int hz;
  if(sample)
    hz = 50;
  else
    hz = 5;

  //Convert frequency to milliseconds and return value
  return 1000 / hz;
}

void sampleAcc()
{
  int x = 42;
}

void sampleFlex()
{
}

void startSampling()
{
  if(sensor)
    MsTimer2::set(getSamplingTime(), sampleAcc);
  else if(!sensor)
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

  Serial.begin(9600);

}

void loop() {
  //While serial port has nothing in buffer, busy wait
  arg = "";
  //Wait until something comes over the serial port
  //while(Serial.available() < 1){}

  if(Serial.available() > 0)
  {
    arg += Serial.read();
    delay(1000);
    Serial.println("test");
  }


  //N == new sampling request
  //T == stop current sampling
  //if(arg == 'N')
     //getSamplingInfo();
  //else if(arg == 'T')
     //stopSampling();

}
