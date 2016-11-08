void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  int SensorValue = analogRead(A0); //take a sample
  Serial.print(SensorValue); Serial.print(" - ");
  
  if(SensorValue >= 1000) {
   Serial.println("Sensor is not in the Soil or DISCONNECTED");
  }
  if(SensorValue < 1000 && SensorValue >= 250) { 
   Serial.println("Dry");
  }
  if(SensorValue < 250 && SensorValue >= 200) {
   Serial.println("Damp"); 
  }
  if(SensorValue < 200) {
   Serial.println("Wet");
  }
  delay(500);
}
