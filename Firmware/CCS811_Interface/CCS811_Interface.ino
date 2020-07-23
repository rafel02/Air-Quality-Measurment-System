#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

CCS811 myCCS811(CCS811_ADDR);

void setup()
{
    //myCCS811.begin();
    Serial.begin(9600);
  Serial.println("CCS811 Basic Example");
}

void loop()
{
  Serial.println("CCS811 Basic Example");
  if (myCCS811.dataAvailable()) {
    
    myCCS811.readAlgorithmResults();
    int tempCO2 = myCCS811.getCO2();
    //int tempVOC = myCCS811.gettVOC();
    
  } else if (myCCS811.checkForStatusError()) {
    while(1);
  }

  delay(1000); //Wait for next reading
}

