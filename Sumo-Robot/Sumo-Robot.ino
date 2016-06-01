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
#define irLedPinL A3
#define irLedPinC A4
#define irLedPinR A5
#define irSensorPinL A0
#define irSensorPinC A1
#define irSensorPinR A2
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
long durationL, durationC, durationR, distanceL, distanceC, distanceR;
long microsecondsToCentimeters(long microseconds);


void setup() {
  /* Here I defined what type of pin each one was, output means the Arduino sends out a 
   * signal via that pin, while input means the Arduino recieves a signal via that pin.
   */
pinMode(irLedPinC, OUTPUT), pinMode(irSensorPinC, INPUT);
pinMode(irLedPinL, OUTPUT), pinMode(irSensorPinL, INPUT);
pinMode(irLedPinR, OUTPUT), pinMode(irSensorPinR, INPUT);
pinMode(trigPinL, OUTPUT), pinMode(echoPinL, INPUT);
pinMode(trigPinC, OUTPUT), pinMode(echoPinC, INPUT);
pinMode(trigPinR, OUTPUT), pinMode(echoPinR, INPUT);
pinMode(speedControlA, OUTPUT), pinMode(speedControlB, OUTPUT);
pinMode(direction1A, OUTPUT), pinMode(direction1B, OUTPUT);
pinMode(direction2A, OUTPUT), pinMode(direction2B, OUTPUT);


Serial.begin(9600);
//This function call is in the setup because it only needs to be run once and that is at the beginning.
CalibrateIR();
}

void loop() {
  /* This clears the trigPins so that they start with 'off,' that way the first ultrasonic
   *  signal sent is easier to detect the beginning and end of.
   */
  digitalWrite(trigPinL, LOW), digitalWrite(trigPinC, LOW), digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  /* This sets the trigPins on HIGH (on) state for 10 microseconds, then it is turned 
   * off again. This is the ultrasonic ping that the sensors send out.
   */
  digitalWrite(trigPinL, HIGH), digitalWrite(trigPinC, HIGH), digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW), digitalWrite(trigPinC, LOW), digitalWrite(trigPinR, LOW);
  /* Times how long the ping took to return, using the echoPin, in microseconds. It times how 
   * long it takes before it hears the echo that the trigger pin sent out. */
  durationL = pulseIn(echoPinL, HIGH);
  durationC = pulseIn(echoPinC, HIGH);
  durationR = pulseIn(echoPinR, HIGH);
  /* This is the time to distance converter. It takes how long the ping
  took to be echoed back, since it was triggered, and uses the standard
  speed of sound in air (a constant) to convert time to distance 
  (velocity/time = distance). */
  distanceL = microsecondsToCentimeters(durationL);
  distanceC = microsecondsToCentimeters(durationC);
  distanceR = microsecondsToCentimeters(durationR); 
  /* This calls on the function that takes the measured distances and sees if they compare to a given range.
   * This way Sumo will know if there is an object between it and the determined range. */
  locateOpponent(distanceL, distanceC, distanceR);
  irSensors();  

}

int CalibrateIR(){  
// White = true (1), Black = false (0).
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
    digitalWrite(irLedPinC, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(irLedPinC, LOW);
    delayMicroseconds(halfPeriod - 1); 
  }
  /* While the LED sent out its wave, the sensor will read it and return a boolean value.
   * This because digitalRead returns either HIGH or LOW, where HIGH means true and LOW 
   * means false. In this case HIGH means white and LOW means white. For what HIGH means,
   * refer back to the theory at the beginning of the code.
   */
bool colorDetected = digitalRead(irSensorPinC);
  if(colorDetected == true){
    arenaColor = white;
  } else {
    arenaColor = black;
  }   
  return arenaColor;
}
void irSensors(){
  //First is the initilizing code for the front and back IR sensors.
  // F stands for Front and corresponds to the variables on the front of the robot.
  int halfPeriodF = 13;
  int cyclesF = 38;
  int j;
  for (j=0; j <= cyclesF; j++){
    digitalWrite(irLedPinL, HIGH);
    delayMicroseconds(halfPeriodF);
    digitalWrite(irLedPinL, LOW);
    delayMicroseconds(halfPeriodF - 1); 
  } 
  bool frontColor = digitalRead(irSensorPinL);
  
  // B stand for Back and corresponds to the variables on the back of the robot.
  int halfPeriodB = 13;
  int cyclesB = 38;
  int k;
  for (k=0; k <= cyclesB; k++){
    digitalWrite(irLedPinR, HIGH);
    delayMicroseconds(halfPeriodF);
    digitalWrite(irLedPinR, LOW);
    delayMicroseconds(halfPeriodF - 1); 
  } 
  bool backColor = digitalRead(irSensorPinR);
  
  // Once the information from the sensors has been gathered, I compare them 
  // to the center one and call on the appropriate function if it is not the same.
  if (frontColor != arenaColor){
    halt();
  } else {
    
  }
  if (backColor != arenaColor){
    swerve();
  } else {
    
  }
}
void swerve(){
  // This spins Sumo sharply at 100% speed. The explanation for the code is in locateOpponent().
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, HIGH);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, LOW);
  analogWrite(speedControlA, 255), analogWrite(speedControlB, 255);
}
void halt(){
  // Halt the motors by changing the speed to 0% and then backs up slowly after a delay by reversing the wheels' current flow
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH);
  analogWrite(speedControlA, 0), analogWrite(speedControlB, 0);
  delay(50);
  digitalWrite(direction1A, LOW), digitalWrite(direction1B, HIGH);
  digitalWrite(direction2A, HIGH), digitalWrite(direction2B, LOW);
  analogWrite(speedControlA, 100), analogWrite(speedControlB, 100);
  delay(500);
}
void locateOpponent(long distanceL, long distanceC, long distanceR){
  /* This code will spin Motor 1 clockwise and Motor 2 clockwise. Since all motors
   * are created the same, and not as left/right, they will have to be coded opposite of each other 
   * for the entire robot to move in unison. This is due to the symetry of the motor placement.
   * However, in this case, we want Sumo to spin. So instead of moving them together, forward, we are
   * moving them together but in opposite directions. As the front wheel moves forward, the back wheel
   * will move backwards, causing Sumo to pivot in place. DigitalWrite dictates the direction
   * of current flow through the motors, in this case it will enter 'pin 1' (HIGH) and exit 
   * 'pin 2' (LOW). AnalogWrite dictates the speed at which the motor will spin. 255 is the maximum
   * value that analogWrite can output, so that is 100% speed. I put in a delay so I don't exhaust the motors
   * with quick direction changes.
   */
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, HIGH);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, LOW);
  analogWrite(speedControlA, 255), analogWrite(speedControlB, 255);
  delay(20);
  /*  Here I defined a range that the robot will be sensing in. If the ultrasonic sensors
   *  doesn't detect something within 2 feet of it (~60 cm) of them, then there is nothing there.
   *  However, if there is something there, the recorded distance will be shorter than the range and
   *  Sumo will know there is another robot in the way (denoted by the less than or equal to symbols).
   *  Since the sensors have a range of 4 meters (400 cm) and the arena is smaller than 4 meters in diameter, 
   *  I am putting a cap on the range of the sensors so people standing outside of the arena will not 
   *  interfer with Sumo and its ability to find the other robot; this is where the int range comes from. 
   */
   int range = 60;
   if (distanceC <= range && distanceL > range && distanceR > range){
    // The center sensor has detected the other robot so full power straight ahead!
     ramOpponent();
   } else if (distanceC > range && distanceL <= range && distanceR > range){
    // Turn left, this comparison means the left sensor has detected robot, so Sumo will turn towards it.
    // The turn is done by changing the speed of the wheel rotation. As one spins faster, it covers more distance so the Sumo will turn.
    digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
    digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH);
    analogWrite(speedControlA, 100), analogWrite(speedControlB, 255);
   } else if (distanceC > range && distanceL > range && distanceR <= range){
    // Turn right, this comparison means the right sensor has detected a robot
    digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
    digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH;
    analogWrite(speedControlA, 255), analogWrite(speedControlB, 100);
   } else {
    
   }
}
void ramOpponent(){
  // Drive directly towards other robot by rotating wheels together at full speed
  // Note that roational directions are opposite to maintain physical symmetry.
  digitalWrite(direction1A, HIGH), digitalWrite(direction1B, LOW);
  digitalWrite(direction2A, LOW), digitalWrite(direction2B, HIGH);
  analogWrite(speedControlA, 255), analogWrite(speedControlB, 255);
}
/* This function is called on by the distance converter in the void loop().
 * It calculates the distance by converting microseconds to centimeters.  
 * The speed of sound is 340 m/s, which is 29 microseconds per 
 * centimeter. Since the ping is sent out and then bounced back,
 * the true distance is half the recieved distance.*/ 
long microsecondsToCentimeters(long microseconds){
  return microseconds /2 /29;
}

