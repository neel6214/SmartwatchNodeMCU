


#include <Adafruit_SSD1306.h>
#include<Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <JsonStreamingParser.h>
#include <Adafruit_GFX.h>
#include "logo.h"
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>

// All these libraries makes my work easier
#define FIREBASE_HOST "smart-watch-2836a.firebaseio.com"
#define FIREBASE_AUTH "JU4vDtH18as7ZzJpKguB3C4XyYgjNbD1NzZufkQS"


WiFiServer server(80);//notification server
WiFiClientSecure client;
int timezone = 5 * 3600;//timezone
int dst = 0;


Adafruit_SSD1306 display(4);//declaring oled screen using library

void setup() {
  // put your setup code here, to run once:
  Wire.begin(1, 3); //I2C setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.drawBitmap(0, 0,wd2 ,128, 64, 1,WHITE); //display team ash cover for oled
  display.display();
  delay(1000);

  pinMode(16, INPUT);//input pullup does not require 10k resistor
  

  WiFiManager wifiManager; // this automatically excecutes wifi initialization
  display.print("Starting Server: connect to 192.168.4.1");
  wifiManager.autoConnect("EspWatch");
  display.print("Connecting....."); display.display();
  display.println("connected...yeey :)");
  display.println(WiFi.localIP());
  display.display();
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //gets th time from web service
  display.print("\nWaiting for Internet time");
  display.display();
  while (!time(nullptr)) 
    delay(1000);
    
  server.begin();//notification server begin
  display.clearDisplay(); display.display();
  WiFi.mode(WIFI_STA);
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  
}




int api_mtbs = 60000; //mean time between api requests
long api_lasttime, clocklast;

String serorclient = "Client"; // Server or Client status
int showdisplay=2, notif, val;
bool call;
void loop() {

  if (digitalRead(16)==LOW){
    showdisplay++;
    display.clearDisplay(); display.display(); delay(200);
  }  
  if (showdisplay == 2)
  {
    
    display.display();
    display.setTextColor(WHITE);
    display.setCursor(1, 4);
    display.setTextSize(1);
    display.print("S0S"); 
    //Firebase.setInt("Variable",6);
  }

  if (showdisplay < 0) {
    showdisplay = 1;
  } //two lines to keep the logics in limit
  if (showdisplay == 3) {
    showdisplay = 0;
  }
    


  if (showdisplay == 0)
  {
    display.clearDisplay();
   display.display();
   delay(2000);

  }

  if (showdisplay == 1 && (clocklast - millis() > 1000)) //puts a little delay without stopping the code
  {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.setTextSize(1);
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    display.print(p_tm->tm_mday);
    display.print("/");
    display.print(p_tm->tm_mon + 1);
    display.print("/");
    display.print(p_tm->tm_year + 1900);
    int wifistr=WiFi.RSSI();
    display.print("   ");


    display.drawBitmap(100,1,IMG_indicator_msg, 8, 8, WHITE);
    display.setCursor(110, 1);
    display.print(notif);

    display.setCursor(1, 8);
    display.setTextSize(2);
    if((p_tm->tm_min)>=30){
        (p_tm->tm_hour)++;
        (p_tm->tm_min)-=60;
      }
    display.print(p_tm->tm_hour);
    display.print(":");
    display.print((p_tm->tm_min)+30);
    display.print(":");
    display.println(p_tm->tm_sec);
    display.display();
    clocklast = millis();
    if (p_tm->tm_sec == 0) {
      display.clearDisplay();
      display.display();
    }


  }




  
  if (showdisplay == 3)
  { display.drawBitmap(96,1,setting, 24, 24, 1,0);
    display.setCursor(1, 4);
    display.setTextSize(1);
    display.print(serorclient); 
    display.setCursor(1, 12);
    display.print(WiFi.localIP());
    display.display();
  }
  


  
}
