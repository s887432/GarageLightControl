//pin assignments
#define LED_PIN           13
#define SWITCH_PIN       2
#define ADC_PIN         A0
#define LIGHT_CTRL_PIN  7

// light threshold
#define LIGHT_THRESHOLD     300

//variables
bool buttonState = 0;
volatile bool Interrupt = false;     // indicates whether MPU interrupt pin has gone high

typedef enum __DOOR_STATUS__
{
  DOOR_OPEN = 1,
  DOOR_CLOSE = 0,
}DOOR_STATUS;

void SwitchISR() 
{
    Interrupt = true;
}

void setup() {
    Serial.begin(115200);
    
    // configure pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    pinMode(LIGHT_CTRL_PIN, OUTPUT);
    digitalWrite(LIGHT_CTRL_PIN, LOW);

    buttonState = digitalRead(SWITCH_PIN);
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), SwitchISR, CHANGE);

    digitalWrite(LED_PIN, LOW);
}

void loop() 
{
    if (Interrupt)
    {
        delayMicroseconds(50);
        delay(100);//ignore bounces for 100mSec
        Interrupt = false;

        DOOR_STATUS doorStatus = (DOOR_STATUS)(digitalRead(SWITCH_PIN) & 0x1);
        Serial.println(doorStatus);

         int adcValue = analogRead(ADC_PIN);

        if( doorStatus == DOOR_OPEN )
        {
          // door opened
          // check light status. if dark, turn on light
          if( LIGHT_THRESHOLD <= adcValue )
          {
            digitalWrite(LIGHT_CTRL_PIN, HIGH);
          }
        } else {
          // door closed. turn off light
          digitalWrite(LIGHT_CTRL_PIN, LOW);
        }
    }

    //delay(1000);
    // go to sleep
    // TODO...
}
