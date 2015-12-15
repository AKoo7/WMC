#include <MsTimer2.h>

// constants won't change. They're used here to 
// set pin numbers:


const int DrainButton = 4;    // the number of the pushbutton pin
const int WashButton = 3;
const int OffButton = 2;    // the number of the pushbutton pin
const int SpinButton = 5;    // the number of the pushbutton pin
const int LidButton = 6;

const int RotationRelay = 9;
const int WashMotor = 8;      // the number of the LED pin
const int SpinMotor = 7;      // the number of the LED pin


// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

int SpinState = LOW;         // the current state of the output pin
int SpinButtonState;             // the current reading from the input pin
int lastSpinState = LOW;   // the previous reading from the input pin


int OffState = LOW;         // the current state of the output pin
int OffButtonState;             // the current reading from the input pin
int lastOffButtonState = LOW;   // the previous reading from the input pin

int DrainState = LOW;         // the current state of the output pin
int DrainButtonState;             // the current reading from the input pin
int lastDrainButtonState = LOW;   // the previous reading from the input pin

int A = 0 ;
int var = 0;
int RelayState = LOW;  

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;      

void setup() {
  
  pinMode(OffButton, INPUT_PULLUP);
  pinMode(DrainButton, INPUT_PULLUP);
  pinMode(SpinButton, INPUT_PULLUP);
  pinMode(WashButton, INPUT_PULLUP);
  pinMode(LidButton, INPUT_PULLUP);
  
  pinMode(WashMotor, OUTPUT);
  pinMode(SpinMotor, OUTPUT);
  pinMode(RotationRelay, OUTPUT);


  // set initial LED state
  digitalWrite(SpinMotor, LOW);
  digitalWrite(WashMotor, LOW);
  digitalWrite(RotationRelay, LOW);

  MsTimer2::set(1000, TimerEvent); // 5 minutes period
  
}

void loop() {
  
  Wash();
  Spin();
  Drain();
  Off();
  
}

void Spin()
{
 int reading = digitalRead(SpinButton);

  if (reading != lastSpinState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != SpinButtonState) {
      SpinButtonState = reading;

      if ((SpinButtonState == LOW) && (LidButton == LOW))  {
        var = 0;
        MsTimer2::start();
        digitalWrite(WashMotor, LOW);
        digitalWrite(SpinMotor, HIGH);
       
        delay(10000);
        digitalWrite(WashMotor, HIGH);
        
      }
    }
  }
 
  lastSpinState = reading;
  
  
}

void Wash()
{
  int reading = digitalRead(WashButton);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        digitalWrite(SpinMotor, LOW);
        digitalWrite(WashMotor, HIGH);

        var = 1;
        MsTimer2::start();
      }
    }
  }

  if (var == 1){
 
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
      
        previousMillis = currentMillis;

        if (RelayState == LOW){
          RelayState = HIGH;
          //delay(1000);
          digitalWrite(RotationRelay, LOW);
          digitalWrite(WashMotor, HIGH);
        } else {
          //delay(1000);
          digitalWrite(WashMotor, LOW);
          digitalWrite(RotationRelay, HIGH);
          RelayState = LOW;
        }
       
        
        //digitalWrite(WashMotor, HIGH);
      }
}
  // set the LED:

  lastButtonState = reading;
}

void Off()
{
  int reading = digitalRead(OffButton);

  if (reading != lastOffButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != OffButtonState) {
      OffButtonState = reading;

      if (OffButtonState == LOW) {
        digitalWrite(WashMotor, LOW);
        digitalWrite(SpinMotor, LOW);
        digitalWrite(RotationRelay, LOW);
        
       var = 0 ;
      }
    }
  }
  
  lastOffButtonState = reading;
}

void Drain()
{
  int reading = digitalRead(DrainButton);

  if (reading != lastDrainButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != DrainButtonState) {
      DrainButtonState = reading;

      if (DrainButtonState == LOW) {
        digitalWrite(WashMotor, LOW);
        digitalWrite(SpinMotor, HIGH);
       var = 0;
      }
    }
  }
  
  lastDrainButtonState = reading;
}


void TimerEvent()
{
  A++;
  
  if (A == 60*30 )
    {
      Off();
      MsTimer2::stop();
      A= 1;
    }
  
}

