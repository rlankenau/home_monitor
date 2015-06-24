#include <SPI.h>

#include <RH_NRF24.h>
#include <RadioHead.h>


#include <DHT.h>

#define SENSOR 10

#define SENSOR_TYPE DHT22

DHT dht(SENSOR, SENSOR_TYPE);
/* TODO: Move key to EEPROM and have each module read it from there. */

#define AGGREGATOR_ADDRESS 255

RH_NRF24 driver(8,9);

void setup()
{
  dht.begin();
  Serial.begin(9600);
  
  if(!driver.init())
  {
    Serial.println("radio init failed.");
    while(1)
    {

    }
  }
  
  Serial.println("Starting main loop.");
}

typedef struct sensor_msg_t_stct {
  float humidity;
  float tempF;
  float heatIndex;
} sensor_msg_t;

sensor_msg_t msg;
long delta=0;

void loop()
{
  long loop_start = millis();
  
  uint8_t len = sizeof(sensor_msg_t);
  /* Just spin on the radio, checking for available data. */
  if(driver.available()) { 
    driver.recv((uint8_t *)&msg, &len);
    Serial.print(" humidity=");
    Serial.print(msg.humidity);
    Serial.print(" tempF=");
    Serial.print(msg.tempF);
    
  } 
 
  delta = millis() - loop_start + 1;
  
  /* If we rack up more than 30 seconds servicing others, do a read
  for ourselves. */
  if(delta > 30000) {
    delta = 0;
    float h = dht.readHumidity();
    float t = dht.readTemperature(true);
    float hi = dht.computeHeatIndex(t, h);
  
    Serial.print("origin=255");
    Serial.print(" humidity=");
    Serial.print(h);
    Serial.print(" tempF=");
    Serial.print(t);
  }
}
