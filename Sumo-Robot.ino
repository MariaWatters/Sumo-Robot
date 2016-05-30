/* This is the complete code for the Arduino-based Sumo Robot. I wrote it based on my 
 *  understanding of each compmonent, logic, and physics. 
 *  Theory:
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
 * object is and the (higher) the voltage across the transitor.
 */
#define irLedPinL A3
#define irLedPinC A4
#define irLedPinR A5
#define irSensorPinL A0
#define irSensorPinC A1
#define irSensorPinR A2

/* The purpose of this function is to calibrate the IR sensors for the comparative cases. 
 * The front and back IR sensors will be compared to the center one and which color
 * it detects. Since there are no paramaters for the function, nothing in the (), then 
 * I assigned the function to a variable.
 */
String CalibrateIR();
String arenaColor; 

void setup() {
  // put your setup code here, to run once:
pinMode(irLedPinC, OUTPUT);
pinMode(irSensorPinC, INPUT);

Serial.begin(9600);
}

void loop() {
  

}

String CalibrateIR(){
// White = true (1), Black = false (0).
  if(digitalRead(irSensorPinC)==HIGH){
    arenaColor = "White";
  } else {
    arenaColor = "Black";
  }   
  return arenaColor;
}

