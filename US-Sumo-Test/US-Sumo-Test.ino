//Test 1.03

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
/* First off, I defined Motor 1 (A) by entering motor shield pin name and 
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
 */
#define trigPinL 13
#define trigPinC 11
#define trigPinR 9
#define echoPinL 12
#define echoPinC 10
#define echoPinR 8
/* Next are the IR sensors. Here I used the #define method, but with the analog pins, so  
 * it attached a name to the corresponding analog pin denoted by A0-A5 (A for analog). 
 * These sensors work by turning an LED on and off and using a phototransitor
 * to measure the amount of reflected light. The more light reflected, the more white an
 * object is and the higher the voltage across the transitor.
 */
#define irLedPinF A3
#define irLedPinC A4
#define irLedPinB A5
#define irSensorPinF A0
#define irSensorPinC A1
#define irSensorPinB A2
/* The purpose of this function is to calibrate the IR sensors for the comparative cases. 
 * The front and back IR sensors will be compared to the center one and which color
 * it detects. There will be two colors to the arena, the border color and the arena color,
 * but I only need to know the arean because I know that these colors will be opposite of each other. So all
 * I need to test is the color of the arena, as the robot will be initially placed in the 
 * center. These are also all 'int' because of the boolean case where true = 1 and false = 0.
 * As digitalRead() returns the boolean values HIGH or LOW, where HIGH = true, and 
 * LOW = false, and because of how the phototransitor works, white = HIGH and blakc = LOW.
 * Thus, white = HIGH = true = 1, and black = LOW = false = 0, allowing me to only need to work
 * with numbers to simplify the code.
 */
int CalibrateIR();
int arenaColor;
int white = 1;
int black = 0; 
/* Here I defined the functions that will hold the code for what the Sumo will do if the 
 *  front or back IR sensor detects a color that is not the arena color. I also defined a  
 *  function that will continuously loop so the front and back are constantly sensing, and
 *  the entire code is simplifed. This function doesn't return anything so it is void.
 */
void irSensors(); 
void swerve();  // If the back IR sensor detects a color change
void halt();  // If the front IR sensor detects a color change
/* Here I defined the function and variables that is responsible for the ultrasonic sensors 
 * and how the Sumo should do in response. Since the robots will be placed back to back, 
 * I need my Sumo to spin until the center ultrasonic sensor detects an object in front of it.
 * Then I need the Sumo to proceed directly towards that sensed object. 
 */
void locateOpponent();
void ramOpponent();
// Each side must have its own duration and corresponding distance.
long durationC, distanceC;
long microsecondsToCentimeters(long microseconds);
/* This function is used to simplify the functions that determine distance for every ultrasonic sensor.
 * the trigPin and echoPin are ints because the pins are numbered and those numbers are what is passed to the function,
 * under the given name for our understanding of what's happening. */
long senseDistance(int trigPin, int echoPin);
bool objectPresent(long distanceC);
bool objectDetected;

void setup() {
  /* Here I defined what type of pin each one was, output means the Arduino sends out a 
   * signal via that pin, while input means the Arduino recieves a signal via that pin.
   */
  pinMode(irLedPinC, OUTPUT), pinMode(irSensorPinC, INPUT);
  pinMode(irLedPinF, OUTPUT), pinMode(irSensorPinF, INPUT);
  pinMode(irLedPinB, OUTPUT), pinMode(irSensorPinB, INPUT);
  pinMode(trigPinL, OUTPUT), pinMode(echoPinL, INPUT);
  pinMode(trigPinC, OUTPUT), pinMode(echoPinC, INPUT);
  pinMode(trigPinR, OUTPUT), pinMode(echoPinR, INPUT);
  pinMode(speedControlA, OUTPUT), pinMode(speedControlB, OUTPUT);
  pinMode(direction1A, OUTPUT), pinMode(direction1B, OUTPUT);
  pinMode(direction2A, OUTPUT), pinMode(direction2B, OUTPUT);
}

void loop() {
  /* These call upon the senseDistance function for each side, one at a time, so the sound waves don't interfer and so the code
   * doesn't get out of sync trying to do all three at the same time, as it was originally. This will happen in a matter of milliseconds. */
  distanceC = senseDistance(trigPinC, echoPinC);
  
  /* This calls on the function that takes the measured distances and sees if they compare to a given range.
   * This way Sumo will know if there is an object between it and the determined range. */
  locateOpponent(distanceC);
}

long senseDistance(int trigPin, int echoPin)
{
  /* Generic duration and distance is used here so I only needed to write the function once, with the function calls determining the side
   * it corresponds to. */
  long duration;
  long distance;
  
  /* This clears the trigPin so that they start with 'off,' that way the first ultrasonic
   * signal sent is easier to detect the beginning and end of.
   */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  /* This sets the trigPin on HIGH (on) state for 10 microseconds, then it is turned 
   * off again. This is the ultrasonic ping that the sensors send out.
   */
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  /* Times how long the ping took to return, using the echoPin, in microseconds. It times how 
   * long it takes before it hears the echo that the trigger pin sent out. */
  duration = pulseIn(echoPin, HIGH);
  
  /* This is the time to distance converter. It takes how long the ping
  took to be echoed back, since it was triggered, and uses the standard
  speed of sound in air (a constant) to convert time to distance 
  (velocity/time = distance). */
  distance = microsecondsToCentimeters(duration);

  return distance;
}
void locateOpponent(long distanceC){
  /*  Here I defined a range that the robot will be sensing in. If the ultrasonic sensors
   *  doesn't detect something within 2 feet of it (~60 cm) of them, then there is nothing there.
   *  However, if there is something there, the recorded distance will be shorter than the range and
   *  Sumo will know there is another robot in the way (denoted by the less than or equal to symbols).
   *  Since the sensors have a range of 4 meters (400 cm) and the arena is smaller than 4 meters in diameter, 
   *  I am putting a cap on the range of the sensors so people standing outside of the arena will not 
   *  interfer with Sumo and its ability to find the other robot; this is where the int range comes from. 
   */
   objectPresent(distanceC);
   
   if (objectDetected==true){
    // The center sensor has detected the other robot so full power straight ahead!
     ramOpponent();
   } else {
    
   }
}
bool objectPresent(long distanceC){
  int range = 30;
  if(distanceC <= range){
    return objectDetected = true;
  } else {
    
  }
  
}
void ramOpponent(){
  // Drive directly towards other robot by rotating wheels together at full speed.
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH);
  analogWrite(speedControlA, 100), analogWrite(speedControlB, 100);
}
/* This function is called on by the distance converter in the void loop().
 * It calculates the distance by converting microseconds to centimeters.  
 * The speed of sound is 340 m/s, which is 29 microseconds per 
 * centimeter. Since the ping is sent out and then bounced back,
 * the true distance is half the recieved distance.*/ 
long microsecondsToCentimeters(long microseconds){
  return microseconds /2 /29;
}
