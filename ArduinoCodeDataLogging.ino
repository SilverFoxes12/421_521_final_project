#include <Wire.h> // Needed for the Sensors
#include <Adafruit_Sensor.h> // Needed for the light and Humidity Sensor
#include <Adafruit_TSL2561_U.h> // Needed for the Light Sensor
#include <Adafruit_MotorShield.h> // Needed for the Motor
#include "utility/Adafruit_MS_PWMServoDriver.h" // Needed for the Motor

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
long int seconds = 0;
long int hours = 0;
String plantwet;
String plantwatered;
int sincewater = 0;
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
{ // Play around and set the gain and integration time. Currently have no gain and an integration time of 13 ms
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


void setup()
{
  Serial.begin(9600); // Make sure this number and the number in the processing page is the same.
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
  configureSensor();   /*Light Sensor Ends here!!*/
}

void loop()
{
  myMotor->run(RELEASE); //To start, the motor is not running.
  uint8_t i;
  plantwatered = "unwatered";
  int countwater = 0;
  int sincewaterhours;
  int SensorValue = analogRead(A0); /*take a sample of Humidity Sensor*/
  int SensorValueTwo = analogRead(A1); /*take a sample of Bag level Sensor*/
  myMotor->run(RELEASE);

  //Serial.print("Current Soil Moisture- ");
  //Serial.println(" ---- ");
  delay(500);
  if(SensorValue >= 1000) {
   //Serial.println("Sensor is not in the Soil or DISCONNECTED");
   plantwet = "Plant Sensor Failed";
  }
  if(SensorValue < 1000 && SensorValue >= 250) { 
   //Serial.println("Soil is Dry -- Time to Water the plant!");
   plantwet = "Plant is Dry";
  }
  if(SensorValue < 250 && SensorValue >= 200) {
   //Serial.println("Soil is Damp"); 
   plantwet = "Plant is Damp";
  }
  if(SensorValue < 200) {
   //Serial.println("Soil is Wet");
   plantwet = "Plant is Wet";
  }
  //Serial.println(" ---- ");
 // Serial.print("Current Water Bag Level - ");   /*print the bag level
 // Serial.println(SensorValueTwo);
 // Serial.println(" ---- ");
   delay(500);

   // Get a new sensor event for light sensor
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) 
  // 120 is indoor lighting need to check and see what sunlight would be*/
  if (event.light)
  {
    if (event.light > 1000){
      seconds = seconds + 2; // if there is sunlight, the sunlight hour count will increase
        if (seconds == 3600) {
          hours = hours + 1;
          seconds = 0;
        }
      
     // Serial.print(" Plant has had "); Serial.print(hours); Serial.println(" hours of sunlight.");
    }
  }
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! 
    //Serial.println("Sensor overload");
  }
 // Serial.println(" ---- ");*/
  delay(500);
  // OMG lets put the motor into this thing.
   if(SensorValue < 1000 && SensorValue >= 250 && SensorValueTwo > 10) { //If the plant is dry and there is water in the container. Over ten is used to rule out false positives of the water meter.
   //if(SensorValue < 450 && SensorValueTwo > 0) { //for testing purposes
    myMotor->run(FORWARD); // Motor on, aka Watering.
    plantwatered = "watered";
    sincewater = 0;  // Time since last water is now zero.
    delay(100); // Water for X time. Can be changed if you want more/less water per motor run.
    myMotor->run(RELEASE); // Turn off motor.
    // later I want it to print the time. I am not going to bother with that right now tho
    countwater = 1; // Plant ahs been watered. 
  }
  else {
    sincewater= sincewater + 2; // If plant wastn't watered, Time sincewater is now 2 more seconds.
    if (sincewater == 3600) {
          sincewaterhours = sincewaterhours + 1;
          sincewater = 0;
  }
  }
  if (countwater > 0 && countwater <= 6){
    Serial.println("Plant was watered!" + plantwet + "; Current Water Bag Level is " + SensorValueTwo + "; Current light hours is " + hours + ";" + '\n');
    countwater++; // This section prints out Plant was watered + six times. This is a fail safe to makesure at least one print is picked up by the Pi.
  }
String totalstring = plantwet + "; Current Water Bag Level is " + SensorValueTwo + "; Current light hours:seconds is " + hours + ":" + seconds + ";" + " Last watered " + sincewaterhours + " hours and " + sincewater + " seconds ago." + '\n';
Serial.println(totalstring);
}

