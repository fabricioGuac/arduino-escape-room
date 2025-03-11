#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Count of TURNS
const int TURNS = 5;
// Array to store the sequence of the game
int randomSqunce[TURNS];
// Defines the pin for the buzzer
const int BUZZER = 2;
// Array for the buttons sounds (G3, A3, B3, C4)
const int FREQ[4] = {196,220,247,262};
// Array to define the pins for the LEDs
const int LED_PINS[] = {13,12,11,10};
// Defines the pins for the buttons
const int BTN_PINS[] = {6,5,4,3};
// Variable to manage the level the player is in
int level = 1;
// Delay for the tone and LED display duration
int seqDelay = 350;
// Creates the LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Sets the buzzer pin as outputs
  pinMode(BUZZER,OUTPUT);

  // Initializes the LCD
  lcd.init(); 
  // Turns on the LCD backlight to illuminate the pixel panel
  lcd.backlight();

  for(int i = 0; i < 4; i++){
  // Sets the LED pin as outputs
  pinMode(LED_PINS[i],OUTPUT);
  // Sets the button as INPUT_PULLUP (a mode that sets the pin value as HIGH helping us avoid pin floating)
  pinMode(BTN_PINS[i],INPUT_PULLUP);
  }

}

void loop() {

  // Sets the cursor to the top left
  lcd.setCursor(0,0);
  lcd.clear();

  if(level == 1){
    lcd.print("Starting game");
    delay(1000);
    lcd.clear();
    genRandomSequence();
    showSequence();
    lcd.print("Level 1");
    readUserInput();
  } else {
    lcd.clear();
    lcd.print("Level: ");
    lcd.print(level);

    showSequence();
    readUserInput();
  }
}

void genRandomSequence(){
  // Initializes the random number generator using noise from the unconnected A0 pin
  randomSeed(analogRead(A0));
  // Loop to fill the randome sequence array with numbers between zero and three
  for(int i = 0; i < TURNS; i++){
    randomSqunce[i] = random(0,4);
  }
}

void showSequence(){
  // Ensures all LEDs are off
  for(int i = 0; i < 4; i++){
    digitalWrite(LED_PINS[i], LOW);
  }
  // Loop for showing the sequence up to the current level
  for(int i = 0; i < level; i++){
    int index = randomSqunce[i];
    // TURNS on the corresponding LED
    digitalWrite(LED_PINS[index], HIGH);
    // Plays the tone for the corresponding LED using its frequency
    tone(BUZZER, FREQ[index]);
    // Delay for the tone and LED duration
    delay(seqDelay);
    // TURNS off the corresponding LED and stops the buzzer
    digitalWrite(LED_PINS[index], LOW);
    noTone(BUZZER);
    // Short delay before going to the next step
    delay(50);
  }
}

void readUserInput(){
  // Loops through the levels
  for (int i = 0; i < level; i++){
    // Flag to wait for a valid input
    bool validInput = false;

    while(!validInput){
      // Checks each button
      for(int j = 0; j < 4; j++){
        if(digitalRead(BTN_PINS[j]) == LOW){
          // Lights the corresponding LED
          digitalWrite(LED_PINS[j], HIGH);
          // Plays the corresponding tone
          tone(BUZZER, FREQ[j]);
          // Delay for the tone and LED
          delay(seqDelay);
          // Stops the buzzer
          noTone(BUZZER);

          // Validates the input
          if(j == randomSqunce[i]){
            // Sets the valid input flag to true to move to the next input
            validInput = true;
            // Delays to avoid misreads when pressing and holding the button
            delay(200);
            while(digitalRead(BTN_PINS[j]) == LOW){
              delay(20);
            }
            // Turs the corresponding LED off
            digitalWrite(LED_PINS[j], LOW);
            // Exits the loop once the correct button is pressed
            break;
          } else {
            // Handles the defeat
            incorrectSequence();
            // Exits the function
            return;
          }
        }
      }
    }
  }
  // If all inputs are correct handles the victory
  correctSequence();
}

void correctSequence() {
  // Ensures the level does not exceed the number of TURNS
  if (level < TURNS) {
    level++; // Go to the next level
  } else {
    // Prints the victory message
    lcd.clear();
    lcd.print("Congratulations!");
    lcd.setCursor(0,1);
    lcd.print("You won :O!");

    
    for (int i = 0; i < 10; i++) {
      // Lights LEDs one after another and play their respective tunes
      for (int j = 0; j < 4; j++) {
        tone(BUZZER, FREQ[j]);
        digitalWrite(LED_PINS[j], HIGH);
        delay(100);
        noTone(BUZZER);
        digitalWrite(LED_PINS[j], LOW);
        delay(50); // short delay between notes and LEDs
      }
    }
    
    delay(2000); // Waits 2 seconds before restarting the game
    level = 1; // Sets the level back to 1 to restart the game
  }
}


void incorrectSequence(){
  // Blink all LEDs to indicate defeat
  for(int i = 0; i < 4; i++){
  digitalWrite(LED_PINS[i],HIGH);
  }
  delay(300);
  for(int i = 0; i < 4; i++){
  digitalWrite(LED_PINS[i],LOW);
  }

  // Plays a fail buzz
  tone(BUZZER, 100);
  delay(500);
  noTone(BUZZER);

  // Prints the defeat message
  lcd.clear();
  lcd.print("You Lost :(");

  delay(2000);
    // Sets the level back to 1 to restart the game
  level = 1;
}