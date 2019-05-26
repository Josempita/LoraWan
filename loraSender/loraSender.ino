#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <SSD1306.h>  
#include "images.h"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    433E6

#define TOUTCH_PIN T2 // ESP32 Pin D2
int touch_value = 0;
unsigned int counter = 0;

SSD1306 display(0x3c, 4, 15);
String rssi = "RSSI --";
String packSize = "--";
String packet ;
String sendMessage = "";



void setup() {
  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);   // while OLED is running, must set GPIO16 in high
  
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
  delay(1500);
}

/***************************************************** 
* ESP32 Touch Test and LED Ctrl
* Touch pin ==> Touch0 is T0 which is on GPIO 4 (D4).
*****************************************************/

void loop() {

  touch_value = touchRead(TOUTCH_PIN);
  if (touch_value > 0)
  {
     sendMessage = "Touch ";
     counter++;
  }
  else
  {
    // send packet
    sendMessage = "No Touch ";

  }
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0, 0, "Sending packet: ");
  display.drawString(90, 0, String(touch_value));
  display.display();
  // send packet
  LoRa.beginPacket();
  LoRa.print(sendMessage);
  LoRa.print(" ");
  LoRa.print(counter);
  LoRa.endPacket();

  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(500); 
                       
}
