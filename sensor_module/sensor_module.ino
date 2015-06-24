#include <SPI.h>

#include <RH_NRF24.h>
#include <RadioHead.h>


#include <DHT.h>

#define SENSOR 10

#define SENSOR_TYPE DHT22

DHT dht(SENSOR, SENSOR_TYPE);
/* TODO: Move key to EEPROM and have each module read it from there. */


#define CLIENT_ADDRESS 1
#define AGGREGATOR_ADDRESS 255

RH_NRF24 driver(8,9);

void setup()
{

  dht.begin();
  Serial.begin(9600);

  if(!driver.init())
  {
    Serial.println("radio init failed.");
    while(1){}
  }
  Serial.println("Starting main loop.");
}

typedef struct sensor_msg_t_stct {
  float humidity;
  float tempF;
  float heatIndex;
} sensor_msg_t;

sensor_msg_t msg;

void loop()
{
  /* Sleep for 1 minute. */
  msg.humidity = dht.readHumidity();
  msg.tempF = dht.readTemperature(true);
  msg.heatIndex = dht.computeHeatIndex(msg.tempF, msg.humidity);
 
  for(int i=0;i<4;i++) {
    driver.send((uint8_t *)&msg, sizeof(sensor_msg_t));
    delay(random(10,30));
  }
  
  delay(500);
}
