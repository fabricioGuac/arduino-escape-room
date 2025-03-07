#include <Servo.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Defines the length of the password array (6 characters + null terminator)
const byte PSSWRD_LENGHT = 7;
// Stores user input
char input[PSSWRD_LENGHT];
// Master password
char password[PSSWRD_LENGHT] = "123456";

// Pin assigments
const byte SERVO_OUTPUT = 10;
const byte BUZZER = 11;

// Keypad dimensions
const byte ROWS = 4;
const byte COLS = 4;

// Array representation of the keypad layout
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

// Row and column pin mapping
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Creates hardware objects
Servo unlockServo;
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Count for tracking user input position
byte input_count = 0;
// Stores the curretly pressed key
char key;

void setup() {
  // Initializes the LCD
  lcd.init(); 
  // Turns on the LCD backlight to illuminate the pixel panel
  lcd.backlight();
  // Attaches the servo object to the servo pin (10)
  unlockServo.attach(SERVO_OUTPUT);
  // Sets the servo initial position to 0 degrees
  unlockServo.write(0);
  // Sets the buzzer pin (11) as output
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Sets the cursor to the top left
  lcd.setCursor(0,0);
  // Prints the enter password message to the LCD
  lcd.print("Enter password");

  // Reads keypad input (only updates when a new key is pressed)
  key = keypad.getKey();  

  // Ensures the key is present
  if (key) { 
    // Stores the pressed key
    input[input_count] = key;
    // Displays the input with an offset
    lcd.setCursor(input_count +4, 1);
    //Prinst the new key to the LCD
    lcd.print(input[input_count]);
    // Increases the input count
    input_count++;
  }

  // Checks if the entered password reaches the required length
  if(input_count == PSSWRD_LENGHT-1){
    // Clears the LCD
    lcd.clear();

    // Validates the password comparing the input with the stored password
    if(!strcmp(input, password)){
      // Prints the message to let the user know it is correct
      lcd.print("Correct!");

     // Victory tone
     tone(BUZZER, 523);  // C5
      delay(150);
      tone(BUZZER, 659);  // E5
      delay(150);
      tone(BUZZER, 784);  // G5
      delay(150);
      tone(BUZZER, 1046); // C6
      delay(300);
      noTone(BUZZER);

      // Unlocks mechaminsm
      unlockServo.write(180);
      delay(5000); // Waits 5 seconds
      unlockServo.write(0); // Locks again
    } else{
      // If the password does not match notifies the user
      lcd.print("Incorrect");
      
      // Defeat tune (attempt of sad trombone)
      tone(BUZZER, 294);  // D4
      delay(300);
      tone(BUZZER, 277);  // C#4
      delay(300);
      tone(BUZZER, 261);  // C4
      delay(300);
      tone(BUZZER, 247);  // B3
      delay(600);
      noTone(BUZZER);
      delay(1000);
    }

    // Clears the LCD screen
    lcd.clear();
    // Resets input for next attempt
    input_count = 0;
  } 
}
