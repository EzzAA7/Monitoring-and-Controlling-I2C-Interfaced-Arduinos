#include <Wire.h>

#define TempPin A0	// TMP36 Pin
#define LDRpin A1

// Variables will change:
long previousMillis = 0;        // will store last time LED was updated
long previousZeroMillis = 0;        // will store last time five seconds passed
 
// the following variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long oneSecInterval = 1000;           // interval at which to blink (milliseconds)
long fiveSecInterval = 5000;

float myTemps[5];
char buff[7];	// buff string to save float to string data

int i=0;
float sum = 0.0;
float avg = 0.0;
int trigPin = 2;
int echoPin = 3;
long duration, cm;

int pushButton = 4;
int buttonState = 0;         // This variable 'buttonState' will be used to store the state of the button

int LDR;

void setup()
{
  pinMode(TempPin,INPUT);	// Temperature Pin is output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pushButton, INPUT_PULLUP);
   
  Wire.begin(2);                // join i2c bus with address #2
  Serial.begin(9600); 
}
/*
	TMP36 is an analog linear temperature sensor.
	This means that the output voltage is proportional 
	to the temperature. The output voltage rises 
	by 10mv for every 1 degree Celsius rise in temperature.
	The Arduino can read input from 0-5v. 
	The Arduino stores this as a 10bit number(0-1023).
*/

void loop(){
  sendMessage();
}


void sendMessage()
{
  
  unsigned long currentMillis = millis(); //current time
  buttonState = digitalRead(pushButton);	// checking state of pushbutton
  if (buttonState != LOW) {	// long pressing to not send value
  
    // find if 5 seconds have passed
    if(currentMillis - previousZeroMillis > fiveSecInterval) {
      //Serial.println("five seconds passed");
      previousZeroMillis = currentMillis;	// change previous so we can have a new interval not just the first one

      for (int j = 0; j < 5; j++) {	// get average temp
          sum += myTemps[j];
      }
      avg = sum/5; 
      //-----------------------TEMPERATURE--------------
      //Serial.print("Temp:");
      dtostrf(avg,7,2, buff); // convert float to string
      Wire.beginTransmission(2);// begin i2c transmission
      Wire.write(buff);//to write using i2c
      Wire.endTransmission(); //message buffer is sent with Wire.endTransmission()                  

      avg = 0.0;	// reset averge and sum for next interval
      sum = 0.0;

      //------------------------LDR---------------------
      LDR=analogRead(LDRpin);		// read LDR value
      //Serial.print("LDR:");
      dtostrf(LDR,7,2, buff); // convert float to string
      Wire.beginTransmission(2);
      Wire.write(buff);//to write using i2c
      Wire.endTransmission(); //message buffer is sent with Wire.endTransmission()                  


      //------------------------ULTRASONIC--------------	
		// calculate ultrasonic distance using trigger
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
	
      pinMode(echoPin, INPUT);	
      duration = pulseIn(echoPin, HIGH);
      cm = (duration/2) / 29.1;	// calculate and set to cm

      //Serial.print("Ultrasonic:");
      dtostrf(cm,7,2, buff); // convert float to string
      Wire.beginTransmission(2);
      Wire.write(buff);			//to write using i2c
      Wire.endTransmission(); //message buffer is sent with Wire.endTransmission()                  


    }
    // check if a second has passed
    if(currentMillis - previousMillis > oneSecInterval) {
      // save the last time you read the temperature
      previousMillis = currentMillis;   
      int tmp = analogRead(A0);//Reading data from the sensor.This voltage is stored as a 10bit number.
      float voltage = (tmp * 5.0)/1024;//(5*temp)/1024 is to convert the 10 bit number to a voltage reading.
      float milliVolt = voltage * 1000;//This is multiplied by 1000 to convert it to millivolt.

      //float tmpCel =  (milliVolt-500)/10 ;// Range(−40°C to +125°C)
      myTemps[i] =  (milliVolt-500)/10;	// store in an array

      //Serial.print("Celsius: ");
      //Serial.print(myTemps[i]);
      //Serial.println(degree);

      if(i<4)			// reset the array after 5 reading
          i=i+1;		// by returing to index 0 once we reached 5th index
      else
        i=0;

    }
  }
}