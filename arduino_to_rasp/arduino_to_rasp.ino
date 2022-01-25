#include "DHT.h"
#define PIR_PIN_INPUT 3     //PIR = Passive InfraRed sensor
#define DHTPIN 2            // Digital pin connected to the DHT sensor
#define LED_LIGHT 4

#define LED_BLUE 9
#define LED_GREEN 10
#define LED_RED 11

#define DHTTYPE DHT11       // DHT 11
DHT dht(DHTPIN, DHTTYPE);

bool alarm; //TRUE: Alarm is on, FALSE: Alarm is off
bool alarmBreach; //TRUE: someone has breached the Alarm, FALSE: Alarm is on and everything is fine

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino has been activated. Starting Serial communication...");
  pinMode(LED_LIGHT, OUTPUT);
  pinMode(LED_RED, OUTPUT); 
  pinMode(LED_GREEN, OUTPUT); 
  pinMode(LED_BLUE, OUTPUT); 
  pinMode(PIR_PIN_INPUT, INPUT);     
  dht.begin();
  alarm = false;
  alarmBreach = false;

  led_color(0, 255, 0);
}

void loop() {
  delay(500);
  sendData();
  
  if(alarm==true && digitalRead(PIR_PIN_INPUT) == HIGH && alarmBreach == false){
    Serial.println("MOTION_DETECTED");
    alarmBreach = true;
    delay(500);
  }
  
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n'); //Reading the first line until there is a backspace
    
    if(data.equals("LED_ON")){
      digitalWrite(LED_LIGHT, HIGH);
      
    }else if(data.equals("LED_OFF")){
      digitalWrite(LED_LIGHT, LOW);
      
    }else if(data.equals("ALARM_ON")){
      alarm=true;
      led_color(255, 0, 0);
    }else if(data.equals("ALARM_OFF")){
      alarm=false;
      alarmBreach = false;
      digitalWrite(LED_LIGHT, LOW);
      led_color(0, 255, 0);
    }
  }
  
  if(alarmBreach == true){
    digitalWrite(LED_LIGHT, HIGH);
    led_color(255, 0, 0);
    delay(500);
    led_color(0, 0, 0);
  } else {
    delay(800);
  }
}

void sendData(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.println((String) "HUM"+h);
  Serial.println((String) "TEM"+t);
  Serial.println((String) "HIC"+hic);
}

void led_color(unsigned char red, unsigned char green, unsigned char blue)
{
 analogWrite(LED_RED, red); 
 analogWrite(LED_GREEN, green); 
 analogWrite(LED_BLUE, blue); 
}
