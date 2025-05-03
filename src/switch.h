#define CLK_PIN 4
#define DT_PIN 5
#define SWITCH_PIN 6

#define PRESSED LOW
#define RELEASED HIGH

// Rotary encoder variables
int counter = 0;
int aState;
int aLastState;

// Switch state machine
enum ButtonState
{
    IDLE,
    DEBOUNCE_PRESS,
    SINGLE_CLICK,
    DEBOUNCE_RELEASE,
    WAIT_FOR_SECOND_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
};

ButtonState buttonState = IDLE;
unsigned long pressStartTime = 0;
unsigned long firstReleaseTime = 0;
const unsigned long debounceTime = 50;
const unsigned long doubleClickWindow = 300;
const unsigned long longPressTime = 1000;

void switch_init()
{
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    aLastState = digitalRead(CLK_PIN);
    Serial.begin(9600);
}

bool handleSwitch()
{
    int currentReading = digitalRead(SWITCH_PIN);
    unsigned long currentTime = millis();
    bool eventDetected = false;

    switch (buttonState)
    {
    case IDLE:
        if (currentReading == PRESSED)
        {
            pressStartTime = currentTime;
            buttonState = DEBOUNCE_PRESS;
        }
        break;

    case DEBOUNCE_PRESS:
        if (currentTime - pressStartTime > debounceTime)
        {
            if (currentReading == PRESSED)
            {
                buttonState = SINGLE_CLICK;
            }
            else
            {
                buttonState = IDLE;
            }
        }
        break;

    case SINGLE_CLICK:
        if (currentReading == RELEASED)
        {
            firstReleaseTime = currentTime;
            buttonState = DEBOUNCE_RELEASE;
        }
        else if (currentTime - pressStartTime > longPressTime)
        {
            Serial.println("Long Press Detected");
            buttonState = LONG_PRESS;
            eventDetected = true;
        }
        break;

    case DEBOUNCE_RELEASE:
        if (currentTime - firstReleaseTime > debounceTime)
        {
            buttonState = WAIT_FOR_SECOND_CLICK;
        }
        break;

    case WAIT_FOR_SECOND_CLICK:
        if (currentReading == PRESSED)
        {
            pressStartTime = currentTime;
            buttonState = DOUBLE_CLICK;
        }
        else if (currentTime - firstReleaseTime >= doubleClickWindow)
        {
            Serial.println("Single Click Detected");
            buttonState = IDLE;
            eventDetected = true;
        }
        break;

    case DOUBLE_CLICK:
        if (currentTime - pressStartTime > debounceTime)
        {
            if (currentReading == PRESSED)
            {
                Serial.println("Double Click Detected");
                buttonState = IDLE;
                eventDetected = true;
            }
            else
            {
                buttonState = WAIT_FOR_SECOND_CLICK;
            }
        }
        break;

    case LONG_PRESS:
        if (currentReading == RELEASED)
        {
            buttonState = IDLE;
        }
        break;
    }

    return eventDetected;
}

bool handleRotaryEncoder()
{
    aState = digitalRead(CLK_PIN);
    if (aState != aLastState)
    {
        if (digitalRead(DT_PIN) != aState)
        {
            counter--;
        }
        else
        {
            counter++;
        }
        Serial.print("Position: ");
        Serial.println(counter);
        aLastState = aState;
        return true;
    }
    aLastState = aState;
    return false;
}