 /************************************************************
        *
        * @author <<< Hamza El Iraqy >>>
        */

int therm_pin = 18;
int data_pin = 14;
int latch_pin = 15;
int clock_pin = 16;
int temp;

#define MIN_TEMP 15.0
#define MAX_TEMP 33.0

byte symbols[] = {
  B10010000,  //HIGH
  B11010100,  //NORMAL
  B11100010   //LOW
};

int h = 0;
int l = 0;
elapsedMillis timer;

void setup(){
  Serial.begin(38400);

  pinMode(data_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);

  digitalWrite(data_pin, LOW);
  digitalWrite(clock_pin, LOW);
  digitalWrite(latch_pin, LOW);

  
  
}

void loop(){

  if(timer >= 10000 && timer <= 14000){
    lowTemp();
  }
  if((timer > 6000) && (timer < 10000)){
    highTemp();
  }
  if((timer <= 6000) || (timer >= 14000)){
    temp = analogRead(therm_pin);
    readTemp(temp);
  }
  
  delay(500);
  
}

void segmentControl(float realTemp){
  
  if(realTemp > MIN_TEMP && realTemp < MAX_TEMP){
    digitalWrite(latch_pin,LOW);
    shiftOut(data_pin, clock_pin, LSBFIRST, symbols[1]);
    digitalWrite(latch_pin, HIGH);
  }
  else if(realTemp < MIN_TEMP){
    digitalWrite(latch_pin, LOW);
    shiftOut(data_pin, clock_pin, LSBFIRST, symbols[2]);
    digitalWrite(latch_pin, HIGH);
  }
  else{
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, symbols[0]);
  digitalWrite(latch_pin, HIGH);
} 

}

void readTemp(int temp){
  
  float realTemp = 0.0F;
  realTemp = 25.0 + ((temp - 512.0) / 10.3); //512 11.3
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" (raw) ");
  Serial.print(realTemp);
  Serial.println(" Celsius");

  segmentControl(realTemp);

}
void highTemp(){
  
  if((h == 0)){
    Serial.println("INJECT : HIGH");
    h = 1;
  }
  int highTem = 800;
  readTemp(highTem);
}

void lowTemp(){
  
  if((l == 0)){
    Serial.println("INJECT : LOW");
    l = 1;
  }
  int lowTem = -80;
  readTemp(lowTem);
}
