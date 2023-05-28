#include <Keypad.h>
#include <Servo.h>
#include <TimeLib.h>
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte triggerPin = 12;
byte echoPin = 11;
byte ledPin = 10;

byte servoPin = 13;
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
Servo myservo;  

void setup(){
  Serial.begin(9600);
  myservo.attach(servoPin);
  myservo.write(0);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}
char password [5] = "2340";
char input [5] = "";
int counter = 0;
bool open = false;
time_t epoch_since_close = 0;

void loop(){
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;



  if (distance < 10) {
    epoch_since_close = now();
    //Serial.println(epoch_since_close);
    digitalWrite(ledPin, HIGH);
  } 
  
  if (now() - epoch_since_close > 1) {
    digitalWrite(ledPin, LOW);
  }


  char key = keypad.getKey();
  if (open) {
    
    if (now() - epoch_since_close > 3) {
      myservo.write(0);
      open = false;
    }
  }
  if (key){
    //Serial.println(key);
    if (key == 'A') {
        Serial.println("Password sent");
        if (strcmp(input, password) == 0) {
            Serial.println("Password correct");
            myservo.write(135);
            open = true;
        }
        else {
            Serial.println("Password incorrect");
        }
        counter = 0;
        memset(input, 0, sizeof(input));
    }
    else if (counter < 4) {
        input[counter++] = key;
        Serial.println(input);
    }
    
  }
}
