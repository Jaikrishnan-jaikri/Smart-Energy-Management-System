#include <ACS712_E.h>
#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"



/*
  Make sure your Firebase project's '.read' and '.write' rules are set to 'true'. 
  Ignoring this will prevent the MCU from communicating with the database. 
  For more details- https://github.com/Rupakpoddar/ESP32Firebase 
*/
#include <ArduinoJson.h>  
#include <ESP32Firebase.h>

RTC_DS3231 rtc;
DateTime now;


#define _SSID "jai"          // Your WiFi SSID
#define _PASSWORD "12345678"      // Your WiFi Password
#define REFERENCE_URL "https://energy-monitoring-de9c7-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

#define FIREBASE_AUTH "kXGh3JUmeQOgUE5A5EHrSCk60Z2KfzotaZ0rlry6"  

Firebase firebase(REFERENCE_URL);

int S5=33;
int S6=32;
int S7=27;
int S8=39;
float totalEnergy=0;

int Status_SW1;
int Status_SW2;
//int Status_SW3;

#define R5 18
#define R7 4
#define R6 19


void setup() {
  Serial.begin(115200);
  pinMode(18,OUTPUT);
  digitalWrite(18,HIGH);
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  // digitalWrite(LED_BUILTIN, HIGH);
  
  //pinMode(13,INPUT);
  
  
//================================================================//
//================================================================//

  /*// Examples of setting String, integer and float values.
  firebase.setString("Example/setString", "It's Working");
  firebase.setInt("Example/setInt", 123);
  firebase.setFloat("Example/setFloat", 45.32);*/

  // Examples of pushing String, integer and float values.
  firebase.pushString("bedroom1_AC_consumption", "30000 wh 21/03/2024");
  firebase.pushString("bedroom1_fan_consumption", "450 wh 21/03/2024");
  firebase.pushString("motor_consumption_perday", "550 wh  wh 21/03/2024");

  firebase.pushInt("push", 789);
  firebase.pushFloat("push", 89.54);

  /*// Example of getting a String.
  String data1 = firebase.getString("Example/setString");
  Serial.print("Received String:\t");
  Serial.println(data1);

  // Example of getting an int.
  int data2 = firebase.getInt("Example/setInt");
  Serial.print("Received Int:\t\t");
  Serial.println(data2);

  // Example of getting a float.
  float data3 = firebase.getFloat("Example/setFloat");
  Serial.print("Received Float:\t\t");
  Serial.println(data3);

  // Example of data deletion.
  firebase.deleteData("Example");*/

    /*firebase.setString("BedRoom1/light", "OFF");
  firebase.setString("BedRoom1/fan", "OFF");
  firebase.setString("BedRoom1/AC", "OFF");
  firebase.setString("BedRoom1/socket_point", "OFF");*/
  //firebase.setFloat("BedRoom1/light_consumption",0);
  Serial.println("Initializing...");
  if (!rtc.begin()) {
    Serial.println("RTC Module not Present");
  }
  firebase.pushString("bedroom1_light_consumption","350 wh 21/03/2024");

  /*if (rtc.lostPower()) {
    Serial.println("RTC power failure, reset the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }*/

 

  Serial.println("Initialization complete.");
  
}



void Data_from_firebase(String getroom)
{
  String data = firebase.getString(String(getroom)+"/light");
   Serial.print("Device 1:\t");
  Serial.println(data);
  String data1 = firebase.getString("BedRoom1/fan");
  Serial.print("Device 2:\t");
  Serial.println(data1);
  String data2 = firebase.getString("BedRoom1/AC");
  Serial.print("Device 3:\t");
  Serial.println(data2);
  String data3 = firebase.getString("BedRoom1/socket1");
  Serial.print("Device 4:\t");
  Serial.println(data3);
  //firebase.setString("BedRoom1/DEV", "OFF");



  /*const size_t capacity = JSON_OBJECT_SIZE(5) + 60;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, data);

  // Store the deserialized data.
  String received_String1 = doc["appliance1"]; // "It's Working"
  String received_String2 = doc["appliance2"];               // 123
  String received_String3 = doc["appliance3"];         // 45.32

  // Print data
  Serial.print("Received String:\t");
  Serial.println(received_String1);

  Serial.print("Received Int:\t\t");
  Serial.println(received_String2);

  Serial.print("Received Float:\t\t");
  Serial.println(received_String3);*/

}

void displayTime() {
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
  Serial.println();
}

void displayDate() {
  Serial.print("Date: ");
  Serial.print(now.day());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.year());
  Serial.println();
}

void controlFun(int stat , int pin,String device)
{
   if(stat==1)
  {
   String control_update = firebase.getString("BedRoom1/"+String(device)+"_control");
   Serial.println(control_update);
  
    if (control_update=="OFF")
    {
      digitalWrite(pin,HIGH);
    }
    else
    {
      digitalWrite(pin,LOW);
      firebase.setString("BedRoom1/"+String(device),"OFF");
    }
    }
    
   delay(1000);

  }


float getaccurrent(int pin,String device)
{
  ACS712 Sensor(ACS712_30A,pin);
  Sensor.calibrate();
  float I=Sensor.getCurrentAC();
  /*if(I<20)
  {
    I=0;
  }*/
  I=I-0.16;
  Serial.println(I);
  float instantaneousPower = I * 220;
  float energyPerSecond = instantaneousPower / 3600; // Energy consumed per second in watt-hours
  float totalEnergybackup=firebase.getFloat("BedRoom1/"+String(device)+"_consumption");
  totalEnergy = totalEnergybackup+energyPerSecond;
  SendDataPerDay(totalEnergy,device);
  Serial.println("totenergy : "+String(totalEnergy)+" Wh"); // Accumulate energy consumption
  // Reset total energy consumption every hour
  firebase.setFloat("BedRoom1/"+String(device)+"_consumption",totalEnergy);
  Serial.println("Wh- of light sent");
  return I;


}

int getStatus(float A,int stat, String device)
{
  if(A>=0.08)
  {
    stat=1;
    firebase.setString("BedRoom1/"+String(device),"ON");
  }
  else
  {
    stat=0;
    firebase.setString("BedRoom1/"+String(device),"OFF");
  }
  Serial.println(stat);
  return stat;
}





void timeanddate()
{
  now = rtc.now();
  displayDate();
  displayTime();
}

void SendDataPerDay(float wh , String device)
{
  Serial.println("datapushed");
  now = rtc.now();
  if((now.hour())==00 && (now.minute())==00 && (now.minute())==00)
  {
    firebase.pushString("bedroom1_"+String(device)+"_consumption", String(wh)+"Wh"+"  "+String(now.day())+"/"+String(now.month())+"/"+String(now.year()));
    
  }
}




void bedroom1()
{
  float current = getaccurrent(S5,"light");

  Serial.println(current);
  //Serial.println(energy);
  int status =  getStatus(current,Status_SW1,"light");

  Data_from_firebase("BedRoom1");

  controlFun(status,R5,"Light");

 Serial.println(" ");
 Serial.println(" ");
 Serial.println("second device ...... ");
 Serial.println(" ");

  float current2 = getaccurrent(S6,"fan");

  //Serial.println(current2);
  //Serial.println(energy);
  int status2 =  getStatus(current2,Status_SW2,"fan");

  
  controlFun(status2,R6,"fan");


  //float current = getaccurrent(S6);

 // Serial.println(current);
  //Serial.println(energy);
  //int status =  getStatus(current,Status_SW3,"socket");

  //Data_from_firebase("BedRoom1");

 // controlFun(status,R7);



  
  timeanddate();

}




void loop() 
{
  delay(1000);
  bedroom1();



}
