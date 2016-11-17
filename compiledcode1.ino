#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
long int seconds = 0;
long int hours = 0;

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  delay(100);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
   tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  // tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
   //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
   tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  //Serial.println("------------------------------------");
  //Serial.print  ("Gain:         "); Serial.println("Auto");
  //Serial.print  ("Timing:       "); Serial.println("13 ms");
  //Serial.println("------------------------------------");
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

void setup()
{
  Serial.begin(9600);
  AFMS.begin();  // create with the default frequency 1.6KHz for motor
  myMotor->setSpeed(150);
  myMotor->run(RELEASE);
  pinMode(A0, INPUT);   /*Humidity Sensor*/
  pinMode(A1, INPUT);  /*Bag*/
   /* Initialise the sensor */
  if(!tsl.begin())     /*Light Sensor starts here!!*/
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  /*displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();   /*Light Sensor Ends here!!*/
}

void loop()
{
  uint8_t i;
  int SensorValue = analogRead(A0); /*take a sample of Humidity Sensor*/
  int SensorValueTwo = analogRead(A1); /*take a sample of Bag level Sensor*/

  //Serial.print("Current Soil Moisture- ");
  //Serial.println(SensorValue);
  //Serial.println(" ---- ");
  delay(500);
  if(SensorValue >= 1000) {
   Serial.println("Sensor is not in the Soil or DISCONNECTED");
  }
  if(SensorValue < 1000 && SensorValue >= 250) { 
   Serial.println("Soil is Dry -- Time to Water the plant!");
  }
  if(SensorValue < 250 && SensorValue >= 200) {
   Serial.println("Soil is Damp"); 
  }
  if(SensorValue < 200) {
   Serial.println("Soil is Wet");
  }
  Serial.println(" ---- ");
  Serial.print("Current Water Bag Level - ");   /*print the bag level*/
  Serial.println(SensorValueTwo);
  Serial.println(" ---- ");
  delay(500);

   /* Get a new sensor event for light sensor*/ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  // 120 is indoor lighting need to check and see what sunlight would be
  if (event.light)
  {
    /*Serial.print("Current light is "); Serial.print(event.light); Serial.println(" lux");*/
  Serial.print(event.light); Serial.println(" lux");
    if (event.light > 1000){
      seconds = seconds + 1;
        if (seconds == 3600) {
          hours = hours + 1;
          seconds = 0;
        }
      
      Serial.print(" Plant has had "); Serial.print(hours); Serial.println(" hours of sunlight.");
    }
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  Serial.println(" ---- ");
  delay(500);
  // OMG lets put the motor into this thing.
  //if(SensorValue < 1000 && SensorValue >= 250 && SensorValueTwo > 0) { //If the plant is dry and there is water
  if(SensorValue < 200 && SensorValueTwo > 0) { //for testing purposes
   Serial.println("Let's start watering!");
   /*myMotor->run(FORWARD);*/
    for (i=0; i<255; i++) {
    /*myMotor->setSpeed(i);*/  
    myMotor->run(FORWARD);
    delay(500);
    myMotor->run(RELEASE);
    
  }
  myMotor->run(RELEASE);
  }
}
