//Test 2.3

/* This is the complete code for the Arduino-based Sumo Robot. I wrote it based on my 
 *  understanding of each compmonent, logic, and physics. 
 *  Theory Section (including notes before definitions):
 *  Note that in the first step I defined the pins differently. The motor shields
 *  had to be defined with the 'int =;' method because it is a pin to pin relationship. 
 *  I needed to relate the motor sheild pins to the connected Ardunio pins.
 *  The sensors could be defined with the #define method because they were a pin to 
 *  emitor/reciever relationship.
 *  There are also multiple functions in this code, apart from the void setup() and 
 *  the void loop() functions. These functions are initially defined as prototypes
 *  because the code reads top to bottom, but the actually function is written after the
 *  line that calls upon it. By defining it initially, the code is prepared for the call
 *  line and knows what to do when it sees the call; the call line tells the program to 
 *  run the function and return to the call line the value the function evaluated.
 */
 /* First off, I am using the NewPing.h library to simplify the ultrasonic sensors
  *  command code. This library also adds timing percision, gets rid of lag,
  more accurate distance calculations, and avoid pulseIn which is 'slow'. For more info
  on this library visit: http://playground.arduino.cc/Code/NewPing */
#include <NewPing.h>

/* Secondly, I defined Motor 1 (A) by entering motor shield pin name and 
 * then the arduino pin number. Here I use the "int =;" 
 * enA = speedControlA, enB = speedControlB, in1 = direction1A, and so on.
 * Note that enA and enB need to be connected to digital pins that have PWM,
 * which pins 6 and 5 do.
 */
int speedControlA = 6;
int direction1A = 7;
int direction2A = 2;
// Next is Motor 2 (B).
int speedControlB = 5;    
int direction1B = 3;
int direction2B = 4;
/* Next are the Ultrasonic sensors. Here I use the #define method, which names the pin 
 * in the corresponding digital pin that is numbered.
 * L = left side, C = center, R = right side. The sensors work by sending out a
 * sound wave (with the 'trig pin') and then measuring the time it takes for the
 * wave to return to the sensor, which is read by the 'echo pin.'
 * I also defined the range of the sensors as between 9 cm and 100 cm.
 * Since the sensors have a range of 4 meters (400 cm) and the arena is smaller than 4 meters in diameter, 
 * I am putting a cap on the range of the sensors so people standing outside of the arena will not 
 * interfer with Sumo and its ability to find the other robot; this is where the max and min distance 
 * comes from. I gave it a min distance for testing purposes (don't just ram something all the time).
 * The minimum range is 10 cm because the is the distance from the sensor to the tip of the ramp, plus 1 cm
 * for good measure.
 */
#define trigPinL 13
#define echoPinL 12
int maxDistance = 60;
int minDistance = 10;

/* Here I defined the functions that will hold the code for what the Sumo will do if the 
 *  front or back IR sensor detects a color that is not the arena color. I also defined a  
 *  function that will continuously loop so the front and back are constantly sensing, and
 *  the entire code is simplifed. This function doesn't return anything so it is void.
 */
void swerve();
void halt();
/* Here I defined the function and variables that is responsible for the ultrasonic sensors 
 * and how the Sumo should do in response. Since the robots will be placed back to back, 
 * I need my Sumo to spin until the center ultrasonic sensor detects an object in front of it.
 * Then I need the Sumo to proceed directly towards that sensed object. 
 */
void locateOpponent();
void ramOpponent(); 
/* This function is specific to the NewPing library and gives percise control over the sensor.
 * Note that you must use correct pins you want to use with this function and it is off the type 
 * NewPing, kind of like 'Bool' is a unique function type.
 */
NewPing sonar(trigPinL, echoPinL, maxDistance);
int distanceL;
  
void setup() {
  Serial.begin(9600);
  /* Here I defined what type of pin each one was, output means the Arduino sends out a 
   * signal via that pin, while input means the Arduino recieves a signal via that pin.
   */
  pinMode(trigPinL, OUTPUT), pinMode(echoPinL, INPUT);
  
  pinMode(speedControlA, OUTPUT), pinMode(speedControlB, OUTPUT);
  pinMode(direction1A, OUTPUT), pinMode(direction1B, OUTPUT);
  pinMode(direction2A, OUTPUT), pinMode(direction2B, OUTPUT);
}

void loop() {
  /* I define the distance of the center sensor to be the distance (in cm) that the sonar ping gives. 
   * sonar.ping_cm() tells the sensor to both send out a signal and wait for a response, a condensed and
   * percise version of my earlier code. */
  // After discovering it was a faulty sensor and swapping it out, I lowered the delay for "shove" tests
  delay(1000);
  
  distanceL = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(distanceL); 
  Serial.print("cm.");
  Serial.print('\n');
  /* This calls on the function that takes the measured distances and sees if they compare to a given range.
   * This way Sumo will know if there is an object between it and the determined range. */
  locateOpponent(distanceL);
}

void locateOpponent(int distanceL){
  
   
   if (distanceL > minDistance){
    // The center sensor has detected the other robot so full power straight ahead!
     ramOpponent();
   } else {
     halt();
   }
}

void ramOpponent(){
  // Drive directly towards other robot by rotating wheels together at full speed.
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH);
  analogWrite(speedControlA, 100), analogWrite(speedControlB, 100);
}
void halt(){
  digitalWrite(direction1A, LOW), digitalWrite(direction1B, LOW);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, LOW);
  analogWrite(speedControlA, 100), analogWrite(speedControlB, 100);
}
