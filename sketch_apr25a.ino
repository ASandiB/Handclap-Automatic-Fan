int red_pin = 9;
int blue_pin = 10;
int green_pin = 11;

int dark_threshold = 20;
int light_level = analogRead(A2);
int potentiometer = analogRead(A0); 


void turnoff(){
  analogWrite(red_pin, 0);
  analogWrite(blue_pin, 0);
  analogWrite(green_pin, 0);
}

enum ButtonStates{INIT,off, on} current_state;
void SM0_Tick() { 
   // State Transistions
   switch(current_state) {
       case INIT:
         current_state = off;
         break;
       case off:
         current_state = on;
         break;
       case on:
         current_state = off;
         
         break;
       default:
         current_state = off;
         break;
   }
   // State Actions
   switch(current_state) {
     case INIT:
      break;
    case off:
      turnoff();
      break;
    case on:
      int potentiometer = analogRead(A0);
      int rgb = map(potentiometer, 0, 1023, 0, 5);
      int red[6] = {8, 36, 87, 99, 21, 75};
      int blue[6] = {98, 21, 04, 14, 12, 67};
      int green[6] = {76, 98, 78, 12, 87, 21};
      switch(rgb){
        case 0:
          analogWrite(red_pin, red[0]);
          analogWrite(blue_pin, blue[0]);
          analogWrite(green_pin, green[0]);
          break;
        case 1:
          analogWrite(red_pin, red[1]);
          analogWrite(blue_pin, blue[1]);
          analogWrite(green_pin, green[1]);
          break;
        case 2:
          analogWrite(red_pin, red[2]);
          analogWrite(blue_pin, blue[2]);
          analogWrite(green_pin, green[2]);
          break;
        case 3:
          analogWrite(red_pin, red[3]);
          analogWrite(blue_pin, blue[3]);
          analogWrite(green_pin, green[3]);
          break;
        case 4:
          analogWrite(red_pin, red[4]);
          analogWrite(blue_pin, blue[4]);
          analogWrite(green_pin, green[4]);
          break;
        case 5:
          analogWrite(red_pin, red[5]);
          analogWrite(blue_pin, blue[5]);
          analogWrite(green_pin, green[5]);
  }
    default:
      break;
}
}

void setup() {
  // put your setup code here, to run once:
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(light_level, INPUT);
  pinMode(potentiometer, INPUT);
  Serial.begin(9600);
  current_state = INIT;
}

void loop() {
  // put your main code here, to run repeatedly:
  int light_level = analogRead(A2);
  int potentiometer = analogRead(A0); 
  if (light_level > dark_threshold){
    current_state = on;
  }
  if (light_level <= dark_threshold){
    current_state = off;
  }
 SM0_Tick(); 
  //you can delete these prints if you want they are 
  //just here to show you the values of the 
  //potentiometer(A0) and the photoresistor(A1)
  Serial.print(analogRead(A0));
  Serial.print(' ');
  Serial.println(analogRead(A2));
  //DELETE THIS LINE OF CODE WHEN YOU MAKE YOUR SOLUTION
  //PLEASE DONT FORGET TO DELETE THE DELAY FUNCTION
  //its only there so you can see the transition from states
  //and so that your serial monitor is not spammed with values
}
