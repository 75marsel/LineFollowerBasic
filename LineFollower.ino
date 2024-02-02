/**
  STI Competition 2024
  NAME: Line Follower Robot Sample Template
  FUNCTION: Follows black lines for movement until the end of the circuit.

  @version: h 01/08/2024
*/

#define ENA 11 // Enable Pin for Speed -> RIGHT MOTOR
#define IN1 13 // Forward Motor Movement
#define IN2 12 // Backward Motor Movement

#define ENB 10 // Enable Pin for Speed -> LEFT MOTOR
#define IN4 9
#define IN3 8

#define IR_S1 2
#define IR_S2 3
#define IR_S3 4
#define IR_S4 5
#define IR_S5 6

              // s1 s2 s3 s4 s5
              // 0  1  2  3  4
int ir_arr[5] = {0, 0, 0, 0, 0};
int ir_arr_pins[5] = {IR_S1, IR_S2, IR_S3, IR_S4, IR_S5};

/**
 *  0 = None
 *  1 = Previously Left turn 
 *  2 = Previously Right turn
*/
int prevTurns = 0; 
bool deadEnd = false;


void setup() {
  Serial.begin(9600);
  // Pin declaration of L298N Motor Driver
  for(uint8_t i = 8; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  // Pin declaration of IR Array including CLP and NEAR
  for(uint8_t i = 2; i < 7; i++) {
    pinMode(i, INPUT);
  }
  // Sets the IN pin of L298N to LOW.
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  // reads the ir array and stores it to the ir_arr array
  updateIRArray();
  
  // 0 == black, 1 == white
  if(ir_arr[0] && ir_arr[1] && !ir_arr[2] && ir_arr[3] && ir_arr[4]) { // FW
    forward(200, 200);
    delay(10);
  }
  else if((ir_arr[0] && ir_arr[1] && ir_arr[2] && !ir_arr[3] && !ir_arr[4]) || (ir_arr[0] && ir_arr[1] && !ir_arr[2] && !ir_arr[3] && !ir_arr[4])) { // R
    prevTurns = 2;
    forward(200, 200);
    delay(30);
    right(150, 150);
    delay(20);
  }
  else if((!ir_arr[0] && !ir_arr[1] && ir_arr[2] && ir_arr[3] && ir_arr[4] )|| (!ir_arr[0] && !ir_arr[1] && !ir_arr[2] && ir_arr[3] && ir_arr[4])) { // L
    prevTurns = 1;
    forward(200, 200);
    delay(30);
    left(150, 150);
    delay(20);
  }

  // do correction

  updateIRArray();
  if(ir_arr[0] && ir_arr[1] && ir_arr[2] && !ir_arr[3] && ir_arr[4]) { // CORRECTION R 1
    prevTurns = 1;
    right(120, 120);
    delay(12);

    expReverse();
  }
  else if(ir_arr[0] && ir_arr[1] && ir_arr[2] && ir_arr[3] && !ir_arr[4]) { // CORRECTION R 2
    prevTurns = 2;
    right(150, 150);
    delay(40);

    expReverse();
  }
  else if(ir_arr[0] && !ir_arr[1] && ir_arr[2] && ir_arr[3] && ir_arr[4]) { // CORRECTION L 1
    prevTurns = 3;
    left(120, 120);
    delay(12);

    expReverse();
  }
  else if(!ir_arr[0] && ir_arr[1] && ir_arr[2] && ir_arr[3] && ir_arr[4]) { // CORRECTION L 2
    prevTurns = 4;
    left(150, 150);
    delay(40);

    expReverse();
  }
  
  /**
  if(ir_arr[0] && ir_arr[1] && ir_arr[2] && ir_arr[3] && ir_arr[4] && prevTurns != 0) { // all white
    if(prevTurns == 1) {
      left(150, 150);
      delay(20);
    }
    else {
      right(150, 150);
      delay(20);
    }
    prevTurns = 0;
  }*/

}

/**
  Moves the robot forward with the desired speed.

  @param l_spd speed of the left motor
  @param r_spd speed of the right motor
*/
void forward(int l_spd, int r_spd) {
  analogWrite(ENA, l_spd);
  analogWrite(ENB, r_spd);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

/**
  Moves the robot in reverse with the desired speed.

  @param l_spd speed of the left motor
  @param r_spd speed of the right motor
*/
void reverse(int l_spd, int r_spd) {
  analogWrite(ENA, l_spd);
  analogWrite(ENB, r_spd);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Stops the motor.
void stop(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/**
  Moves the robot to the right with the desired speed.

  @param l_spd speed of the left motor
  @param r_spd speed of the right motor
*/
void right(int l_spd, int r_spd) {
  analogWrite(ENA, l_spd); // left
  analogWrite(ENB, r_spd); // right
  digitalWrite(IN1, LOW); // left
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); // right
  digitalWrite(IN4, HIGH);
}

/**
  Moves the robot to the left with the desired speed.

  @param l_spd speed of the left motor
  @param r_spd speed of the right motor
*/
void left(int l_spd, int r_spd) {
  analogWrite(ENA, l_spd);
  analogWrite(ENB, r_spd);
  digitalWrite(IN1, HIGH); // left
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // right
  digitalWrite(IN4, LOW);
}

void updateIRArray() {
  for(int i = 0; i < 5; i++)
    ir_arr[i] = digitalRead(ir_arr_pins[i]);
}

void expReverse() {
  updateIRArray();
  if(ir_arr[0] && ir_arr[1] && ir_arr[2] && ir_arr[3] && ir_arr[4]) { // CORRECTION WHITE / DEAD END
    reverse(133, 133);
    delay(10);
  }
}