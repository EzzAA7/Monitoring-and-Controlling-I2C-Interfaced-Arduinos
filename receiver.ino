// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char degree = 176; //ASCI value of Degree
float LDR;		// received LDR
float distance;	// received distance
int i=0;		// flag to check which float is received (avg/ldr/distance)
int degreesDiff;	// degree difference between 35-temp
int duty;	// duty cycle value to be used

int ledPin = 7;	// pin for led 
int motorPin=9;	// pin for motor
int bulbPin = 6;	// pin for light bulb
int counterValue; // 1 or 0.5 sec led blink interval variable

void setup(){
  
  	lcd.begin(16,2); // intialises the 16x2 lcd
  	Wire.begin(2);        // join i2c bus (address optional for master)
  	Wire.onReceive(receiveEvent);
  	Serial.begin(9600); 
  
	pinMode(motorPin,OUTPUT); 
  	pinMode(bulbPin,OUTPUT); 
	pinMode(ledPin ,OUTPUT);
  
  	// initialize timer1    
    noInterrupts();           // disable all interrupts   
    TCCR1A = 0;   
    TCCR1B = 0; 
	// preload timer 65536-16MHz/256/1Hz
  	TCNT1 = 3036; 	// 1 sec              
    TCCR1B |= (1 << CS12);    // 256 prescaler 
  	TIMSK1 |= (1 << TOIE1);
    interrupts();             // enable all interrupts 
  
}

String data = "";	//empty string to save charcters received over i2c into string


ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt 
{   
  
  TCNT1 = counterValue;            // preload for 1 or 0.5 sec   
  
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1); // inverse state of led (on/off)
  
} 
void loop(){
}

void receiveEvent(int howMany){
  
  data = "";				//empty buffer
  while( Wire.available()){	// when a ch is written go read it into string
    data += (char)Wire.read();
  }

  float value = data.toFloat();	// parse it as float
  if( value != 0.0){		// check its a digit value
    if(i==0){				// flag i@0 to distinguish avg temp
      float avg= value;
      Serial.print("AVERAGE TEMP: ");	// then print in onto the serial
      Serial.print(avg);
      Serial.print(degree);
      Serial.println(" after 5 seconds ");
      i++;

      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(avg);		// writing temperature

      if(avg < 35){
        analogWrite(motorPin, 0);	// if the temp < 35 then 0% duty cycle
        counterValue = 3036;		// 1 sec led blink interval
      }

      else if(avg = 35){// if the temp = 35 then 20% duty cycle

        analogWrite(motorPin, 51);	// 20% duty cycle
      }

      else if(avg>= 35 & avg<43){
        degreesDiff = avg - 35 ;	// calculate how many degrees off the 35 degree threshold instructed
        duty = degreesDiff * 26;	// since 10% = 0.1*255= 26 so we multiply by it to get needed duty
        duty += 55;			// since the 10%s are added to the 20% (so 30 or 40 or 50 ...)
        analogWrite(motorPin, duty);	// if the temp between 25 and 30 then 30% duty cycle                    
        counterValue = 34286;	// 0.5 sec led blink interval
      }


      else if(avg>= 43){			// since numbers higher than 42 will exceed the max duty cycle
        analogWrite(motorPin, 255);	// if the temp higher than or equal 
        counterValue = 34286;			// 0.5 sec led blink interval
      }
    }

    else if(i==1){	// flag i@0 to distinguish ldr value
      LDR= value;
      Serial.print("LDR: ");	// then print in onto the serial
      Serial.println(LDR);

      // checking LDR value so we can turn on/off
      if(LDR<=400.0){	//0<=V<= 400 the light bulb will be on	
        digitalWrite(bulbPin,HIGH);}
      else{			// else will be off				
        digitalWrite(bulbPin,LOW);
      }
      i++;
    }
    else if(i==2){	// flag i@0 to distinguish distance

      distance= value;
      Serial.print("Distance: ");	// then print in onto the serial
      Serial.print(distance);
      Serial.println(" cm");

      lcd.setCursor(0,1);
      lcd.print("Dist.: ");
      lcd.print(distance);		// writing temperature

      i=0;
    }


  }	
	
}
	