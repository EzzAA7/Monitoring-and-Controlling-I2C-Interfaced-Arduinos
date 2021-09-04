# Monitoring-and-Controlling-I2C-Interfaced-Arduinos

Two Arduinos  communicate with each other using I2C protocol.
- The first Arduino reads the temperature every one second.
- It calculates the average of the temperature every 5 seconds, send the average to the second Arduino via I2C cable, and print it on the LCD at the first row (i.e. Average should update every 5 seconds, so at the first 5 seconds the average will be zero).
- If the temperature average is larger than 35 ̊C, the DC motor will operate with 20% duty cycle. In addition, a LED should start blinking every 0.5 second. Otherwise the same led should start blinking every 1 second. All the timing (0.25 and 1 second) in the blinking should be using software interrupt. For each degree above 35 ̊C, you should increase the speed of the motor by 10%. (Hint: Be careful about exceeding the max. duty cycle). For a temperature of less than 35 ̊C, the DC motor should be off, just the LED plinks every 1 second.
- LDR sensor in the first Arduino, is used to control the state of the light bulb in the second Arduino as follow: for 0<=V<= 400 the light bulb will be on, else will be off.
- The first Arduino should use the ultrasonic sensor to measure the distance.
