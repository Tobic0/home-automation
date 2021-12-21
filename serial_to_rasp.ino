//LED_BUILTIN to select the LED on the arduino UNO board
#define PIR_PIN_INPUT 2 //PIR = Passive InfraRed sensor

bool alarm; //TRUE: Alarm is on, FALSE: Alarm is off
bool alarmBreach; //TRUE: someone has breached the Alarm, FALSE: Alarm is on and everything is fine

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino has been activated. Starting Serial communication...");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_PIN_INPUT, INPUT);     
  alarm = false;
  alarmBreach = false;
}

void loop() {
  if(alarm==true && digitalRead(PIR_PIN_INPUT) == HIGH && alarmBreach == false){
    Serial.println("MOTION_DETECTED");
    alarmBreach = true;
    delay(500);
  }
  
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n'); //Reading the first line until there is a backspace
    
    if(data.equals("LED_ON")){
      digitalWrite(LED_BUILTIN, HIGH);
      
    }else if(data.equals("LED_OFF")){
      digitalWrite(LED_BUILTIN, LOW);
      
    }else if(data.equals("ALARM_ON")){
      alarm=true;
    }else if(data.equals("ALARM_OFF")){
      alarm=false;
      alarmBreach = false;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
  if(alarmBreach == true){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(400);
    digitalWrite(LED_BUILTIN, LOW);
    delay(400);
  } else {
    delay(800);
  }
}

void sendData(){
  Serial.println("");
}
