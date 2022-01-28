#include "DHT.h"
#define PIR_PIN_INPUT 3     // PIR = Passive InfraRed sensor
#define DHTPIN 2            // Digital pin connected to the DHT sensor
#define LED_LIGHT 4         // Classic yellow led

#define LED_BLUE 9          // Setting the pin for b of rgb led
#define LED_GREEN 10        // Setting the pin for g of rgb led
#define LED_RED 11          // Setting the pin for r of rgb led

#define DHTTYPE DHT11       // Definining the type of DHT sensor used
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
  sendData();   // Calls function to get data from DHT sensor
  
  // Checking if someone has entered the room
  if(alarm==true && digitalRead(PIR_PIN_INPUT) == HIGH && alarmBreach == false){
    Serial.println("MOTION_DETECTED");
    alarmBreach = true;
    delay(500);
  }
  
  // Check for possible available data comming from raspberry
  if(Serial.available()>0){
    String data = Serial.readStringUntil('\n');     // Reading the first line until there is a backspace
    
    // Checking for particular string
    if(data.equals("LED_ON")){                      
      digitalWrite(LED_LIGHT, HIGH);
      
    }else if(data.equals("LED_OFF")){
      digitalWrite(LED_LIGHT, LOW);
      
    }else if(data.equals("ALARM_ON")){
      alarm = true;
      led_color(255, 0, 0);                         // Calls rgb led function to set led to red
      
    }else if(data.equals("ALARM_OFF")){
      alarm = false;
      alarmBreach = false;
      digitalWrite(LED_LIGHT, LOW);
      led_color(0, 255, 0);                         // Calls rgb led function to set led to green
    }
  }
  
  if(alarmBreach == true){
    digitalWrite(LED_LIGHT, HIGH);                  // Turn on light for taking a better picture
    led_color(255, 0, 0);
    delay(500);                                     // Delay permitting to turn on and off rgb light to simulate alarm
    led_color(0, 0, 0);
  } else {
    delay(800);
  }
}

void sendData(){
  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early
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

// rgb led function for setting desired colors
void led_color(unsigned char red, unsigned char green, unsigned char blue){
 analogWrite(LED_RED, red); 
 analogWrite(LED_GREEN, green); 
 analogWrite(LED_BLUE, blue); 
}
