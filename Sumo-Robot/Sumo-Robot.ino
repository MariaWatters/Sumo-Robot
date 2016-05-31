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
// First off, I defined Motor 1 by entering motor shield pin name and 
// then the arduino pin number. Here I use the "int =;"
int enA = 6;
int in1 = 7;
int in2 = 2;
// Next is Motor 2.
int enB = 5;
int in3 = 3;
int in4 = 4;
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


void setup() {
  /* Here I defined what type of pin each one was, output means the Arduino sends out a 
   * signal via that pin, while input means the Arduino recieves a signal via that pin.
   */
pinMode(irLedPinC, OUTPUT);
pinMode(irSensorPinC, INPUT);
pinMode(irLedPinL, OUTPUT);
pinMode(irSensorPinL, INPUT);
pinMode(irLedPinR, OUTPUT);
pinMode(irSensorPinR, OUTPUT);

Serial.begin(9600);
//This function call is in the setup because it only needs to be run once and that is at the beginning.
CalibrateIR();
}

void loop() {
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
  for (j=0; j <= cycles; j++){
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
  for (k=0; k <= cycles; k++){
    digitalWrite(irLedPinR, HIGH);
    delayMicroseconds(halfPeriodF);
    digitalWrite(irLedPinR, LOW);
    delayMicroseconds(halfPeriodF - 1); 
  } 
  bool backColor = digitalRead(irSensorPinR);
  
  // Once the information from the sensors has been gathered, I compare them 
  // to the center one and call on the appropriate function if it is not the same.
  if (fronColor != borderColor){
    halt();
  } else {
    
  }
  if (backColor != borderColor){
    swerve();
  } else {
    
  }
}
void swerve(){
  // spin sharply
}
void halt(){
  // halt the motors and back up a bit
}

