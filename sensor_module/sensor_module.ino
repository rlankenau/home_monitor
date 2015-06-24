#include <SPI.h>

#include <RHMesh.h>
#include <RH_NRF24.h>
#include <RadioHead.h>


#include <DHT.h>

#define SENSOR 10

#define SENSOR_TYPE DHT22

DHT dht(SENSOR, SENSOR_TYPE);
/* TODO: Move key to EEPROM and have each module read it from there. */


#define CLIENT_ADDRESS 1
#define AGGREGATOR_ADDRESS 255

RH_NRF24 driver;

RHMesh manager(driver, CLIENT_ADDRESS);

void setup()
{
  pinMode(13,OUTPUT);
  dht.begin();
  Serial.begin(9600);

  if(!manager.init())
  {
    Serial.println("init failed!");
  }

}

typedef struct sensor_msg_t_stct {
  float humidity;
  float tempF;
  float heatIndex;
} sensor_msg_t;

sensor_msg_t msg;

void indicate_error()
{
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
}

void loop()
{
  /* Sleep for 1 minute. */
  msg.humidity = dht.readHumidity();
  msg.tempF = dht.readTemperature(true);
  msg.heatIndex = dht.computeHeatIndex(msg.tempF, msg.humidity);
 
  if(manager.sendtoWait((uint8_t *)&msg, sizeof(sensor_msg_t), AGGREGATOR_ADDRESS) == RH_ROUTER_ERROR_NONE)
  {
    indicate_error();
  }
  
  delay(60000);
}
