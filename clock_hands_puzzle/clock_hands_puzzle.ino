#include <Servo.h>

// Defines the pins for the potentiometers
const int HOURS_PIN = A0;
const int MINUTES_PIN = A1;

// Variables to store the readings from the potentiometers
int minutesVal;
int hoursVal;

// Variables to store the potentiometer values mapped to the clock
int minutes;
int hours;

// Secret time that will trigger the mechanism
const int TARGET_HOUR = 3;
int TARGET_MIN = 20;

// Initializes the Servo object
Servo myServo;

void setup()
{
    // Attaches the Servo object to pin 7
    myServo.attach(7);
    // Sets the Servo to the initial position
    myServo.write(180);
}

void loop()
{
    // Reads the values from the potentiometers
    minutesVal = analogRead(MINUTES_PIN);
    hoursVal = analogRead(HOURS_PIN);

    // Maps potentiometer values to the clock's range
    minutes = (map(minutesVal, 0, 1023, 0, 45) + 40) % 60;
    hours = (map(hoursVal, 0, 1023, 0, 9) + 8) % 12;

    // Checks for correct combo (with a 5 minute error margin)
    if (hours == TARGET_HOUR && abs(minutes - TARGET_MIN) <= 5)
    {
        // Activates the mechanism
        myServo.write(0);
    }
    else
    {
        myServo.write(180);
    }

    // Small delay to avoid jitter
    delay(200);
}