#include <DHT11.h>
#include <SPI.h>
// Sets global variables and sets up temperature sensor
int sound_sensor = 2; 
int dummy = 0;
bool clapDetected = false;
bool waitForSilence = false;
unsigned long lastClap = 0;
const unsigned long clapCooldown = 400;
unsigned long lastTempRead = 0;
const unsigned long tempInterval = 10000; // Records temperature every 10 seconds
DHT11 dht11(7);
int temperature = 0;
byte motor_speed = 0;
byte temp_motor = 0;

void setup() 
{
  //begin Serial Communication
  Serial.begin(9600);
  // Setting pins in arduino
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(sound_sensor, INPUT);
  pinMode(7, INPUT);
  pinMode(SS, OUTPUT);
  // SPI communication
  SPI.begin();
  SPI.beginTransaction(SPISettings(374500, MSBFIRST, SPI_MODE0));
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS,HIGH);
}
 
void loop()
{
  int soundDetected = digitalRead(sound_sensor);
  // Records temperature every 10 seconds to avoid library lag
  if (millis() - lastTempRead >= tempInterval) {
    lastTempRead = millis();
    temperature = dht11.readTemperature();
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }
  // Waits 400ms to detect another clap for "debouncing" capabilities
  if(!waitForSilence && soundDetected == HIGH && (millis() - lastClap) > clapCooldown){
    clapDetected = true;
    waitForSilence = true;
    lastClap = millis();
    Serial.println("Clap detected, waiting for silence...");
  }

  // State machine logic progressing
  if (waitForSilence && soundDetected == LOW){
    if (clapDetected){
      if(dummy == 0){
        dummy = 1;
      }
      else if(dummy == 1){
        dummy = 2;
      }
      else if(dummy == 2){
        dummy = 3;
      }
      else if(dummy == 3){
        dummy = 0;
      }
    }
    clapDetected = false;
    waitForSilence = false;
  }
  // State machine
  switch(dummy){
    case 0:
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(SS, LOW);
    motor_speed = 0x02;
    SPI.transfer(motor_speed);
    digitalWrite(SS, HIGH);
    break;
    case 1:
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(SS, LOW);
    motor_speed = 0x01;
    SPI.transfer(motor_speed);
    digitalWrite(SS, HIGH);
    break;
    case 2:
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW); 
    digitalWrite(SS, LOW);
    motor_speed = 0x03;
    SPI.transfer(motor_speed);
    digitalWrite(SS, HIGH);
    break;
    case 3:
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    if(temperature < 21){
      temp_motor = 0x02;
    }
    else if(temperature >= 21){
      temp_motor = 0x03;
    }
    digitalWrite(SS, LOW);
    motor_speed = temp_motor;
    SPI.transfer(motor_speed);
    digitalWrite(SS, HIGH);
    break;
  }
  // For debugging purposes
  Serial.println(temp_motor);
  Serial.println(temperature);
}
