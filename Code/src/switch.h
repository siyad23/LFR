#define CLK_PIN 4
#define DT_PIN 5
#define SWITCH_PIN 6 // Switch connected to pin 6

#define PRESSED LOW
#define RELEASED HIGH

int counter = 0;
int aState;
int aLastState;
int switchState = RELEASED;         // Current state of the switch
int lastSwitchState = RELEASED;     // Previous state of the switch
unsigned long lastDebounceTime = 0; // Last time the switch state was toggled
const unsigned long debounceDelay = 50;

void switch_init()
{
    pinMode(CLK_PIN, INPUT);           // Set CLK_PIN as input
    pinMode(DT_PIN, INPUT);            // Set DT_PIN as input
    pinMode(SWITCH_PIN, INPUT_PULLUP); // Set the switch pin as input with pull-up resistor
    aLastState = digitalRead(CLK_PIN); // Initialize the last state of CLK_PIN
    Serial.begin(9600);                // Initialize serial communication at 9600 baud rate
}

// Function to handle the rotary encoder
bool handleRotaryEncoder()
{
    aState = digitalRead(CLK_PIN); // Reads the "current" state of the CLK_PIN
    if (aState != aLastState)      // If the previous and the current state of the CLK_PIN are different
    {
        if (digitalRead(DT_PIN) != aState) // If the DT_PIN state is different to the CLK_PIN state
        {
            counter--; // Rotating clockwise
        }
        else
        {
            counter++; // Rotating counterclockwise
        }
        Serial.print("Position: ");
        Serial.println(counter);
        aLastState = aState;
        return true;
    }
    aLastState = aState; // Updates the previous state of the CLK_PIN with the current state
    return false;        // Return false if no rotation occurred
}

// Function to handle the switch with debounce
bool handleSwitch()
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
            switchState = reading;      // Update the switch state
            if (switchState == PRESSED) // If the switch is pressed
            {
                Serial.println("Switch pressed!");
                lastSwitchState = reading; // Update the last switch state
                return true;
            }
        }
    }

    lastSwitchState = reading;
    return false; // Update the last switch state
}