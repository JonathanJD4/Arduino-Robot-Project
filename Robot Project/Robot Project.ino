/******1603905*******/
int servoPort = SERVO_0;
int servoPort1 = SERVO_1;
int S_Pin;
int pressed=0; //for switches pressed
int lcdpause=0; //to stop the flickrering
int follow=0;
static int Lfwd = 1000;                                           // Servo values for each motor and each direction
static int Rfwd = 2000;                                           // Servo values for each motor and each direction
static int Lbck = 2000;                                           // Servo values for each motor and each direction
static int Rbck = 1000;                                           // Servo values for each motor and each direction
static int Lstp = 1500;                                           // Servo values for each motor and each direction
static int Rstp = 1500;                                           // Servo values for each motor and each direction

void setup() {
  InitEmoro();
  EmoroServo.attach(servoPort);               // initialize servo motor on selected port
  EmoroServo.attach(servoPort1);
  Serial.begin(9600);                         // initialize serial communication:
  Serial.print("Pulse width: ");              // print string
  Serial.println(" us\n");                    // print string
  Ultrasonic.attach(S_Pin);
  int follow = 0;
  Gyr.init();
  pinMode(IO_0, INPUT_PULLUP);                             // Initiates input with pullup (infrared sensor on IO_1 port)
  pinMode(IO_1, INPUT_PULLUP);                             // Initiates input with pullup (infrared sensor on IO_2 port)
  // if LCD available print info:
  if (ReadEmoroHardware() & LCD_AVAILABLE) {                    // if LCD ia available
    Lcd.locate(0, 0);                                           // set LCD cursor to the first row, the first character
    Lcd.print("1.Obst 2.Circle");                                  // print menu options
    Lcd.locate(1, 0);                                           // set LCD cursor to the second row, the first character
    Lcd.print("3.Square 4.Reset");                            // print constant string
    lcdpause=1;
    
  }
}

void loop() {

    if (ReadSwitch(SW_1) == 1 || (pressed == 1)){ //Indicates that uf switch 1 is pressed the emoro robot will report switch 1 is pressed
      Serial.println("1.Obst");
      if (ReadEmoroHardware() & LCD_AVAILABLE) {
        Serial.println("1.Obst");
        Lcd.clear();
        Lcd.locate(0, 1);                                      // set LCD cursor to the second row, the first character
        Lcd.print("Moving Forward");
        lcdpause=2;
        // Set servo pulse to 1500us. For position servo motors this will set the middle position,
        // for servo motors with continuous rotation this will stop rotation
      }
      pressed = 1;
      obstacle();
    }
    if (ReadSwitch(SW_2) == 1 || (pressed == 2)) { //Indicates that if switch 2 is pressed the emoro robot will report switch 2 is pressed
      Serial.println("2.Circle");               
      if (lcdpause!=3) {                // stops the flickring
        Lcd.clear();
        Lcd.locate(0,1);                                       // prints circle track following
        Lcd.print("Circle track");
        Lcd.locate(1,1);
        Lcd.print("following");
        lcdpause=3;
        }
      pressed = 2;
      circle();
    }
  



  else if (ReadSwitch(SW_3) == 1 || (pressed == 3)) {                           // //Indicates that if switch 3 is pressed the emoro robot will report switch 3 is pressed
    Serial.println("3.Square");               // print constant string
    if (lcdpause!=4) {                
      Lcd.clear();
      Lcd.locate(0,1);                                       
      Lcd.print("Square track");                            // // prints square track following
      Lcd.locate(1,1);
      Lcd.print("following");
      lcdpause=4;
    }
    pressed = 3;
    square();
  }
  if (ReadSwitch(SW_4) == 1 || (pressed == 4)) {                           // if Switch SW_4 is activated then it indicates to the emoro robot will report switch 4 is pressed
    Serial.println("4.Reset");               
    if (lcdpause!=5) {                //To stop the flickering
      Lcd.clear();
      Lcd.locate(0,0);
      Lcd.print("1.Obst 2.Circle");                                  
      Lcd.locate(1, 0);                                           // prints menu options
      Lcd.print("3.Square 4.Reset");                            
      lcdpause=5;
      EmoroServo.write(servoPort, 1500);
      EmoroServo.write(servoPort1, 1500);
    }
    pressed = 4;
  }
}
void obstacle(){
  int ultrasonic;
  ultrasonic = Ultrasonic.read(S_Pin);
  EmoroServo.write(servoPort, 2000);
  EmoroServo.write(servoPort1, 1000);

  if (ultrasonic < 40) { //sensors detect obstacles that are 40cm away
    EmoroServo.write(servoPort, 0);
    EmoroServo.write(servoPort1, 2000);
    Lcd.clear();
    Lcd.locate(0,4); //location of where text is displayed
    Lcd.print("Obstacle");
    Lcd.locate(1,4);
    Lcd.print("detected");
  }
  delay(200);
}

void circle(){
  if (digitalRead(IO_0) == 0 && digitalRead(IO_1) == 0) {       // Both sensros are on the black, go forward.
      EmoroServo.write(servoPort1, 2450);                            // left servo forward 
      EmoroServo.write(servoPort, 1100);                            // right servo forward
    }
    else if (digitalRead(IO_0) == 1 && digitalRead(IO_1) == 1) {  // Both sensors are on the white, stop.
      EmoroServo.write(servoPort, Lstp);                            // left servo stop
      EmoroServo.write(servoPort1, Rstp);                            // right servo stop
    }
    else if (digitalRead(IO_0) == 0 && digitalRead(IO_1) == 1) {                            // Only left sensor is on the black, turn right.
      EmoroServo.write(servoPort1, 1250);                            // left servo forward
      EmoroServo.write(servoPort, 1250);                            // right servo backwards
      
    }
    else if (digitalRead(IO_1) == 0 && digitalRead(IO_0)==1) {                            // Only right sensor is on the black, turn left.
      EmoroServo.write(servoPort1, 1750);                            // left servo backwards
      EmoroServo.write(servoPort, 1750);                            // right servo forwawrd
      
    }
  
}

void square(){
    if (digitalRead(IO_0) == 0 && digitalRead(IO_1) == 0) { //Checks if both the left and right sensors are on the black line      
       
       EmoroServo.write(servoPort, 1050);                            
       EmoroServo.write(servoPort1, 1950); // moves backwords                           
       
       }
  else if (digitalRead(IO_0) == 1 && digitalRead(IO_1) == 1) {  //Checks if both the sensors are detecting white 
        Gyr.resetDegrees(); // resets the X.Y AND Z angles 
        EmoroServo.write(servoPort, 1250);       //spins both wheels clockwise and anti clockwise to turn at sharp turns                       
        EmoroServo.write(servoPort1, 1250);
        Lcd.clear();
        while (Gyr.readDegreesZ()<90){  //Set up the gyroscope reading for turning 90 degrees 
              Lcd.locate(0,0);
              Lcd.print("Sharp Turning");
              Lcd.locate(1,0);
              Lcd.print(Gyr.readDegreesZ()); // gives off the gyroscope Z reading
            }
         Lcd.clear();
         Lcd.locate(0,0);
         Lcd.print("Square Track");
         Lcd.locate(1,0);
         Lcd.print("Following");  //Printed when the robot is going straight
        
         EmoroServo.write(servoPort, 1500);                       
         EmoroServo.write(servoPort1, 1500);  //Stops motion on both wheels
         delay(100);
         
      
     }                                                                   
   else if (digitalRead(IO_0) ==0 && digitalRead(IO_1) == 1) {   //Checks if left the sensor is off black line                            
        EmoroServo.write(servoPort, 1250);                            
        EmoroServo.write(servoPort1, 1250);                              
        delay(1); //delays for 1 second      
        }
   else if (digitalRead(IO_1) == 0 && digitalRead(IO_0) == 1) { //Checks if the right sensor is off black line                             
        EmoroServo.write(servoPort, 1750);                            
        EmoroServo.write(servoPort1, 1750);                                                                             
        delay(1); //delays for 1 second 
        }
}



/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
