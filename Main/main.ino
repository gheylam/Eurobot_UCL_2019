// Motor Driver
#define MOTOR_PIN_1 11
#define MOTOR_PIN_2 12
#define MOTOR_PIN_3 13

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


enum state_enum {PT_CHAOS, CHAOS_COLLECT, CHAOS_PT, PT_RELEASE, PT_DIST, DIST_JESUS, DIST_COLLECT, DIST_PT};

state_enum PC; //Program Counter (instruction index)


void setup() {
  Serial.begin(9600);
  PC = PT_CHAOS;

}

void loop() {
  // STARTING AREA
  switch(PC){
    case PT_CHAOS:
        
      break;
    
    case CHAOS_COLLECT:

      break;
    
    case CHAOS_PT:

      break;

    case PT_RELEASE:
    
      break;

    case PT_DIST:
    
      break;

    case DIST_JESUS:
    
      break;

    case DIST_COLLECT:
    
      break;

    case DIST_PT:
    
      break;

    default: 
    
      break;


  }
}
