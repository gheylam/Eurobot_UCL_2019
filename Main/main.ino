// Motor Driver
#define MOTOR_PIN_1 11
#define MOTOR_PIN_2 12

// Contact Switches
#define CONTACT_FL_PIN 2  
#define CONTACT_FR_PIN 3
#define CONTACT_BL_PIN 6
#define CONTACT_BR_PIN 7
#define CONTACT_B_PIN 8

// IR Sensor
#define IR_L1_PIN A0
#define IR_L2_PIN A1
#define IR_R1_PIN A2
#define IR_R2_PIN A3

// Check one which side of the map
// Connect pin to ground if distributor on left of PT
#define MAP_SIDE_PIN 20 
#define LEFT 0
#define RIGHT 1

enum state_enum {PT_CHAOS, CHAOS_COLLECT, CHAOS_PT, CHAOS_RELEASE, PT_DIST, DIST_JESUS, DIST_COLLECT, DIST_PT, DIST_RELEASE, FINISH};

state_enum PC; //Program Counter (instruction index)


int distLocation;


void setup() {
  pinMode(CONTACT_FL_PIN, INPUT_PULLUP);
  pinMode(CONTACT_FR_PIN, INPUT_PULLUP);
  pinMode(CONTACT_BL_PIN, INPUT_PULLUP);
  pinMode(CONTACT_BR_PIN, INPUT_PULLUP);
  pinMode(CONTACT_B_PIN, INPUT_PULLUP);
  pinMode(MAP_SIDE_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  PC = PT_CHAOS;

  int val = digitalRead(MAP_SIDE_PIN);
  if (val == 0){
    distLocation = LEFT;
  }else{
    distLocation = RIGHT;
  }

}

void loop() {
  if (distLocation == LEFT){
    distOnLeft();
  }else if (distLocation == RIGHT){
    distOnRight();
  }
}

void distOnLeft(){
  switch(PC){
    // 1-2
    case PT_CHAOS:
      // Starting position to CHAOS AREA
      MoveForward(100); // Towards CHAOS
      RotateRight(90); // Face Pucks
      PC = CHAOS_COLLECT;
      break;
    
    // 2-3
    case CHAOS_COLLECT:
      MoveForward(100); // Eat Pucks
      RotateRight(90); // Face PT
      PC = CHAOS_PT;
      break;
    
    // 3-4
    case CHAOS_PT:
      MoveForward(100); // Return Pucks Home
      RotateRight(90); 
      MoveForward(100);
      PC = CHAOS_RELEASE;
      break;

    // 4-5
    case CHAOS_RELEASE:
      MoveReverse(100); //Spit out Pucks    
      PC = PT_DIST;
      break;

    // 5-6
    case PT_DIST:
      RotateRight(180); 
      MoveForward(100);
      RotateLeft(90);
      MoveForward(100);
      RotateLeft(90);
      PC = DIST_JESUS;
      break;

    // 6-7
    case DIST_JESUS:
      MoveForward(100);
      RotateRight(180);
      PC = DIST_COLLECT;
      break;

    // 7-8
    case DIST_COLLECT:
      MoveForward(100);
      RotateRight(90);
      PC = DIST_PT;
      break;

    // 8-9
    case DIST_PT:
      MoveForward(100);
      RotateRight(90);
      PC = DIST_RELEASE;
      break;

    // 9-10
    case DIST_RELEASE:
      MoveReverse(100);
      PC = FINISH;
      break;

    case FINISH:
      delay(100);
      break;

    default: 
      Serial.println("WE DONE F*CKED UP");    
      break;
  }
}


void distOnRight(){
  switch(PC){
    // 1-2
    case PT_CHAOS:
      // Starting position to CHAOS AREA
      MoveForward(100); // Towards CHAOS
      RotateLeft(90); // Face Pucks
      PC = CHAOS_COLLECT;
      break;
    
    // 2-3
    case CHAOS_COLLECT:
      MoveForward(100); // Eat Pucks
      RotateLeft(90); // Face PT
      PC = CHAOS_PT;
      break;
    
    // 3-4
    case CHAOS_PT:
      MoveForward(100); // Return Pucks Home
      RotateLeft(90); 
      MoveForward(100);
      PC = CHAOS_RELEASE;
      break;

    // 4-5
    case CHAOS_RELEASE:
      MoveReverse(100); //Spit out Pucks    
      PC = PT_DIST;
      break;

    // 5-6
    case PT_DIST:
      RotateLeft(180); 
      MoveForward(100);
      RotateRight(90);
      MoveForward(100);
      RotateRight(90);
      PC = DIST_JESUS;
      break;

    // 6-7
    case DIST_JESUS:
      MoveForward(100);
      RotateLeft(180);
      PC = DIST_COLLECT;
      break;

    // 7-8
    case DIST_COLLECT:
      MoveForward(100);
      RotateLeft(90);
      PC = DIST_PT;
      break;

    // 8-9
    case DIST_PT:
      MoveForward(100);
      RotateLeft(90);
      PC = DIST_RELEASE;
      break;

    // 9-10
    case DIST_RELEASE:
      MoveReverse(100);
      PC = FINISH;
      break;

    case FINISH:
      delay(100);
      break;

    default: 
      Serial.println("WE DONE F*CKED UP");    
      break;
  }
}