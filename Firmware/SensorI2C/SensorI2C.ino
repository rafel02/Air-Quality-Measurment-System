#include <PubSubClient.h>

#include <PubSubClient.h>

#include <Wire.h>

#define sensoraddr 0
#define read_Regaddr 0
#define num_Byterd 0

int data, msb, lsb;

int convert(int B0)
{
  // Read data sheet and write math for proper conversion
}

void readCO2value() {
  
  Wire.beginTransmission(sensoraddr);
  Wire.write(read_Regaddr);   
  Wire.endTransmission();
  delay(100);
  Wire.requestFrom(sensoraddr, num_Byterd );

  if(Wire.available()>= num_Byterd){
    msb = Wire.read();
    lsb = Wire.read();
  }
  data = (msb << 8) + lsb;
  return convert(data);
}

void setup() {
  Serial.begin(9600);
  while(!Serial); 
  Serial.println("---- I2C Begain ----");
  Wire.begin();
}

void loop(){
  float co2val = readCO2value();
  Serial.print("Measured Co2 Value: ");
  Serial.println(co2val);
  delay(500);
} 
