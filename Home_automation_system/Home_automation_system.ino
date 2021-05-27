#include <Keypad.h>
#include<Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int servoPin =A0;
Servo myServo;
const int Forward = A1;
const int Backward = A2;
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password_1 = "1234"; // door open
const String password_2 = "4321"; // door close
const String password_3 = "5642"; // fan

String input_password;

int red_light_pin= 13;
int green_light_pin = 12;
int blue_light_pin = 11;
void setup(){
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  myServo.attach(servoPin);
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  input_password.reserve(4); // maximum input characters is 4, change if needed
}
void setColour(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
void loop(){
  char key = keypad.getKey();

  if (key){
    Serial.println(key);
    lcd.setCursor(0, 0);
    lcd.print(key);
     switch (key)
        {
           case 'A':
              setColour(255,0,0);//GREEN
              break;
           case 'B':
              setColour(0,255,0);//GREEN
              break;
           case 'C':
              setColour(0,0,255);//BLUE
              break;
           case 'D':
              setColour(255,255,125);//RASPBERRY
              break;
           case '7':
              setColour(255,0,255);//MAGENTA
              break;
           case '8':
              setColour(225,255,0);//YELLOW
              break;
           case '9':
              setColour(225,255,255);//WHITE
              break;
           default:
                setColour(0,0,0); //NO COLOR
        }
     if(key == '*') {
      input_password = "";// reset the input password
      lcd.clear();
    } else if(key == '#') {
      lcd.clear();
      if(input_password == password_1) {
        Serial.println("door unlocked");
        myServo.write(90);
        lcd.setCursor(0, 0);
        lcd.print("CORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("DOOR UNLOCKED!");
        
        
      }
      else if(input_password == password_2){
        Serial.println("door locked");
        myServo.write(0);
        lcd.setCursor(0, 0);
        lcd.print("CORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("DOOR LOCKED!");
        
      }
      else if(input_password == password_3){
        Serial.println("fan on");
        digitalWrite(Forward , HIGH);
        digitalWrite(Backward , LOW);
        lcd.setCursor(0, 0);
        lcd.print("CORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("FAN ON!");
        
      }
      else {
        Serial.println("password is incorrect, try again");
        lcd.setCursor(0, 0);
        lcd.print("INCORRECT!");
        lcd.setCursor(0, 1);
        lcd.print("TRY AGAIN!");
        
      }

      input_password = ""; // reset the input password
    } else {
      input_password += key; // append new character to input password string
      lcd.setCursor(input_password.length(), 0); // move cursor to new position
      lcd.print('*');                 // print * key as hiden character
    }
  }
}
