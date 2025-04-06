#define outputA 4
#define outputB 5
#define SWITCH_PIN 6 // Switch connected to pin 6

int counter = 0;
int aState;
int aLastState;
int switchState = HIGH;             // Current state of the switch
int lastSwitchState = HIGH;         // Previous state of the switch
unsigned long lastDebounceTime = 0; // Last time the switch state was toggled
const unsigned long debounceDelay = 50;

void switch_init()
{
    pinMode(outputA, INPUT);           // Set outputA as input
    pinMode(outputB, INPUT);           // Set outputB as input
    pinMode(SWITCH_PIN, INPUT_PULLUP); // Set the switch pin as input with pull-up resistor
    aLastState = digitalRead(outputA); // Initialize the last state of outputA
    Serial.begin(9600);                // Initialize serial communication at 9600 baud rate
}

// Function to handle the rotary encoder
void handleRotaryEncoder()
{
    aState = digitalRead(outputA); // Reads the "current" state of the outputA
    if (aState != aLastState)      // If the previous and the current state of the outputA are different
    {
        if (digitalRead(outputB) != aState) // If the outputB state is different to the outputA state
        {
            counter++; // Rotating clockwise
        }
        else
        {
            counter--; // Rotating counterclockwise
        }
        Serial.print("Position: ");
        Serial.println(counter);
    }
    aLastState = aState; // Updates the previous state of the outputA with the current state
}

// Function to handle the switch with debounce
void handleSwitch()
{
    int reading = digitalRead(SWITCH_PIN);
    if (reading != lastSwitchState) // If the switch state has changed
    {
        lastDebounceTime = millis(); // Reset the debounce timer
    }

    if ((millis() - lastDebounceTime) > debounceDelay) // If the debounce time has passed
    {
        if (reading != switchState) // If the switch state is stable
        {
            switchState = reading;  // Update the switch state
            if (switchState == LOW) // If the switch is pressed
            {
                Serial.println("Switch pressed!");
            }
        }
    }

    lastSwitchState = reading; // Update the last switch state
}