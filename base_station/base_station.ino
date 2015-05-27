#include <SPI.h>

#include <RHSPIDriver.h>
#include <RH_NRF24.h>
#include <RH_ASK.h>
#include <RadioHead.h>


#include <DHT.h>

#define SENSOR 10

#define SENSOR_TYPE DHT22

DHT dht(SENSOR, SENSOR_TYPE);

void setup()
{
  dht.begin();
  Serial.begin(9600);
}

void loop()
{
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  float hi = dht.computeHeatIndex(t, h);
  
  Serial.print("field1=");
  Serial.print(h);
  Serial.print("&field2=");
  Serial.print(t);
}
