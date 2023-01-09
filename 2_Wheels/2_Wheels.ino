int servoPort = SERVO_0; // select the pin for the servo motor
int servoPort1 = SERVO_1;
int waitState = 2000;                         // time in milliseconds between actions
static int Lfwd = 2000;                                           // Servo values for each motor and each direction
static int Rfwd = 2000;  
int robotdirfwd;                                                  // initialize variable - store forward direction
int robotdirbck;    
  
void setup() {
  Serial.begin(9600);                         // initialize serial communication:
  Serial.println("Example: SingleServo");     // send example name to console 
  EmoroServo.attach(servoPort);               // initialize servo motor on selected port
  EmoroServo.attach(servoPort1);

  Serial.print("Pulse width: ");              // print string
  Serial.print(EmoroServo.read(servoPort));   // print current servo pulse width
  Serial.print(EmoroServo.read(servoPort1));
  Serial.println(" us\n");                    // print string
 
}

void loop() {
  // put your main code here, to run repeatedly:
    if (ReadSwitch(SW_1) == 1) {                                    // if switch 1 is pressed

    robotdirfwd=Mag.readDirection();                              // read magnetometer direction in degrees - forward orientation

    // calculate opposite direction angle:
    if (robotdirfwd<=180){                                        // if forward direction angle < 180
      robotdirbck=robotdirfwd+180;                                // calculate opposite direction - add 180 degrees
    } else {                                                      // else
      robotdirbck=robotdirfwd+180;                                // calculate opposite direction - subtract 180 degrees
    }
    }
    
    if (ReadSwitch(SW_2) == 2) {                                    // if switch 1 is pressed

    robotdirfwd=Mag.readDirection();                              // read magnetometer direction in degrees - forward orientation

    // calculate opposite direction angle:
    if (robotdirfwd>=180){                                        // if forward direction angle < 180
      robotdirbck=robotdirfwd+180;                                // calculate opposite direction - add 180 degrees
    } else {                                                      // else
      robotdirbck=robotdirfwd+180;                                // calculate opposite direction - subtract 180 degrees
    }
    {
    EmoroServo.write(servoPort, Lfwd);
    EmoroServo.write(servoPort1, Rfwd);
    }
    }
}
