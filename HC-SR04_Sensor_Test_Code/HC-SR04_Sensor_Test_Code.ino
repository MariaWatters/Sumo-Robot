/* HC-SR04 - Connect VCC to 5v on Arduino, GND to GND on Arduino, Echo to
 *  pin 13, and Trig to pin 12. Note that these are not PWM.
 */

#define trigPin 13
#define echoPin 12

  // define variables the code uses
long duration, distance, cm;

void setup() {
  /* This starts the serial communication and sets the rate that the 
   * sensor sends out pings. Make sure the serial monitor screen (baud) 
   * matches this number. */
   
  Serial.begin(9600);
  
  // Sets trigpin as an output pin and echoPin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clears the trigPin, so when it starts it is 'off'
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);

  /* Sets the trigPin on HIGH state for 10 microseconds, then turns it 'off.' 
   * This is the ping that the sensor sends out. */
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* Times how long the ping took to return, using the echoPin, in 
   * microseconds. */
   duration = pulseIn(echoPin, HIGH);

  /* This is the time to distance converter. It takes how long the ping
  took to be echoed back, since it was triggered, and uses the standard
  speed of sound in air (a constant) to convert time to distance 
  (velocity/time = distance). */
   cm = ((duration/2)/29); 

   // Finally, this prints the distance on the serial monitor.
   Serial.print("cm: ");
   Serial.println(cm);

}

/* This function is called on by the distance converter in the void loop().
 *  It calculates the distance by converting microseconds to centimeters.  
    * The speed of sound is 340 m/s, which is 29 microseconds per 
    * centimeter. Since the ping is sent out and then bounced back,
    * the true distance is half the recieved distance. 
long microsecondsToCentimeters(long microseconds){
  return (microseconds/2)/29;
}*/

