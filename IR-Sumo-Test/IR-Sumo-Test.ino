//Test 3.4.1
/* This is the code for the IR reflective sensors. This does not us a specific
 *  library so I am going to define the pins directly. You can use #define or 
 *  int =; approach; in both cases, the pin number that is equated is the 
 *  connected pin number.
 *  If there is a lot of reflected light, then the object that is 
 *  reflecting the light is white. The less light that is reflected, 
 *  then the darker it is. 
 */

#define irLEDB A5
#define irB A2
int irAverage;
int aveSum;
int threshhold = 534;
char colorDetected;

void setup() {
  /* This defines the pin type, either an input or an output: 
   *  The IR sensor receives a signal so it is input.
   *  The LED is sending a signal so it is an output. */
  pinMode(irB, INPUT);
  pinMode(irLEDB, OUTPUT);

  // Serial.begin intitiates the sent signal
  Serial.begin(9600);
}

void loop(){
  aveSum = 0;
  
  // Check if this works with Joe, especially array assigning
  int i;
  for (i=0; i <= 10; i++){
    digitalWrite(irLEDB, HIGH);
    delay(3);
    // This displays the values the phototransistor picks up
    aveSum += analogRead(irB);
    delay(3);
    digitalWrite(irLEDB, LOW);
    delay(50);
  }
  
  irAverage =(aveSum/10);

  /*The threshhold was experimentally found to be 483; the half way
   * point between 'white,' which has an average analog input of
   * 445, and 'black,' which has an average analog input of 522.
   */
  if (irAverage < threshhold){
    colorDetected='W';
    //White
  } else if (irAverage > threshhold){
    colorDetected='B';
    //Black
  } else {
    colorDetected='N';
    //None
  }
  Serial.println(irAverage);
  Serial.println(colorDetected);
}
