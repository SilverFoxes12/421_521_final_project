import java.util.List;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import processing.serial.*;
Serial sPort;  
String sData;

String file1 = "/home/pi/Arduino/DataLog.txt"; //File path of the saved txt file can be changed here.
String comPort = "COM5";
int logDelay = 3000; //change depending on how long between log entries you want.

void setup()
{
  size(640,480);//window size
  //if serial port is available
  try {
    String portName = Serial.list()[0];// Connecting to the serial port
    sPort = new Serial(this, portName, 9600);
   // don’t read the serial buffer until a new line char
   // should use Serial.println("") in arduino code
   sPort.bufferUntil('\n');   
  } finally {} 
  
}

void draw()
{
  // If data is available,
  if (sPort.available() > 0) {  // If we can reach the port...
    sData =  sPort.readStringUntil('\n');    // Read the incoming data from the Arduino
    //sData =  sPort.readString(); 
    if (sData.startsWith("Plant")){          // each line should start with Plant so just making sure we don't get any interupted lines.  
      logData(file1,getDateTime() + sData,true); // log the data to the file you have set.
    }
    delay(logDelay); // Wait X time before continuing.
  }
}

void logData( String fileName, String newData, boolean appendData)
 {
    BufferedWriter bw=null;
    try { //try to open the file
    FileWriter fw = new FileWriter(fileName, appendData);
    bw = new BufferedWriter(fw);
    bw.write(newData);// + System.getProperty("line.separator"));
    } catch (IOException e) {
    } finally {
    if (bw != null){ //if file was opened try to close
        try {
        bw.close();
        } catch (IOException e) {}
    }
    }
}

String getDateTime()
{
      SimpleDateFormat df = new SimpleDateFormat("[ dd/MM/yyyy HH:mm:ss ] ");
      Calendar date = Calendar.getInstance(); // the current date and time
      return(df.format(date.getTime()));
}