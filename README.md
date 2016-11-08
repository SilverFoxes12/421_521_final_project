# 421_521_final_project
Rice Bioe 421/521 final project
Mr. GARDEN – Geo-friendly Automated Robotic Dirt/Earth Nourisher
	We are proposing an automated gardening system that can sense the wetness of the soil used to house a plant and administer a specific volume of water as needed. The device will use a humidity sensor that will be placed in the soil. Periodically, the system will extract data regarding the soil hydration and will deliver the necessary volume to the plant. Additionally, the system will alert the user when the water housing unit is running low. This will be done using a different sensor (possibly a pressure, light, or different humidity sensor). This will be determined by price and efficacy.

# Added 11/8/16
Sensors:
Currently, the Arduino has three sensors to monitor the plant and the water bag. The sensors and all of their information is found below. A current schematic of the wiring can be found in ArduinoWiring.jpg. Sensors are currently working, however no Pi application has been created to interface with the arduino.
1) Light sensor "Adafruit Digital Light / Luminosity / Lux sensor" was purchased from Amazon. The light sensor code was adapted from https://github.com/adafruit/Adafruit_TSL2561. The following .pdf was followed for sensor setup and electrical setup https://cdn-learn.adafruit.com/downloads/pdf/tsl2561.pdf. Output is current detected Lux every half second. Current code can be found in light_sensor.ino.
2) Soil Humidity sensor "SUNKEE Soil Hygrometer Detection Module Soil Moisture Sensor" was purchased from Amazon. The code was adapted from https://github.com/TasmanianDevilYouTube/Arduino/blob/master/Moisture_Sensor/Moisture_Sensor.ino and the electrical setup was modified from this youtube toturial https://www.youtube.com/watch?v=EjJW0_VET8I. Output is Wet, Damp, Dry or Sensor is not in the Soil or DISCONNECTED. The thresholds for the numbers were computed through trial and error using a dry and watered plant. Current code can be found in soil_sensor.ino.
3) Water depth sensor "Phantom YoYo High Sensitivity Water Sensor" was purchased from Amazon. On the Amazon site, they provided a diagram for the electrical setup. The code was modified from the Soil Humidity Sensor code. Output is milimeters of depth. Current code can be found in bag_sensor.ino.

The GUI:
Initially, a GUI has been created that asks the user for desired soil humidity and total sunlight hours per day. The GUI is touch-based and users can manually type in their responses. An image of this can be seen in Picture.11.8.16.jpg.
