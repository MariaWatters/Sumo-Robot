Test 2.0
/* This is the code for the IR reflective sensors. This does not us a specific
 *  library so I am going to define the pins directly. You can use #define or 
 *  int =; approach; in both cases, the pin number that is equated is the 
 *  connected pin number.
 *  This code works on the premise of turning the LED on and then off at a 
 *  frequency of 38.5 kHz for 1 millisecond. During the pause the IR sensor
 *  measures the amount of reflected light. If there is a lot of reflected
 *  light, then the object that is reflecting the light is white. The less
 *  light that is reflected, then the darker it is. 
 */
 
#define irLedPin A3
#define irSensorPin A0

void setup() {
  /* This defines the pin type, either an input or an output: 
   *  The IR sensor receives a signal so it is input.
   *  The LED is sending a signal so it is an output. */
  pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);

  // Serial.begin intitiates the sent signal
  Serial.begin(9600);
}

void loop(){
  /* First you need to send a faux sinewave to the LED to turn it on and off.
   * Since a wave with a frequency of 38.5 kHz has a period of about 26 
   * microseconds, turning it on for half that time would be half a period,
   * which is where int halfPeriod comes from. The cycles comes from the fact
   * that these sensors detect the light wave's reflection and you need to pulse
   * the light multiple times to create a continuous 'wave' that lasts about 
   * 1 millisecond. Since one crest to crest pulse takes 26 microseconds (13 for 
   * a half period), and there are 1000 microseconds in a millisecond, you would 
   * have to turn the LED on and then off (which makes one complete period) 
   * 1000/26 times, which is rounded to 38 times. Here I used a for loop to ensure 
   * that a the command to turn the LED on and off was looped exactly 38 times.
   */
  int halfPeriod = 13;
  int cycles = 38;
  int i;
  for (i=0; i <= cycles; i++){
    digitalWrite(A3, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(A3, LOW);
    delayMicroseconds(halfPeriod - 1); 
  }
  // This displays the values the phototransistor picks up
  Serial.println(analogRead(A0));
  delay(50);
}
