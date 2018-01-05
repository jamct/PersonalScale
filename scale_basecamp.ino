#define DEBUG 1
#include <Basecamp.hpp>
#include "HX711.h"
#include "soc/rtc.h"
#include <SPI.h>
#include <SSD_13XX.h>

#define _cs   17  // goes to TFT CS
#define _dc   16  // goes to TFT DC
#define _mosi 23  // goes to TFT MOSI
#define _sclk 18  // goes to TFT SCK/CLK
#define _rst  5   // ESP RST to TFT RESET
#define _miso     // Not connected

SSD_13XX disp = SSD_13XX(_cs,_dc);

HX711 scale;
Basecamp iot;

void setup(){ 
  iot.begin();
  Serial.begin(115200);

  scale.begin(25,26);
  iot.web.addInterfaceElement("ScaleFactor", "input", "Factor to calculate weight:", "#configform", "ScaleFactor");
  iot.web.addInterfaceElement("ScaleUnit", "input", "Unit to show (e.g. kg):", "#configform", "ScaleUnit");
  iot.web.addInterfaceElement("ScaleWakeUp", "input", "Threshold for Wakeup-Pin:", "#configform", "ScaleWakeUp");
  iot.web.addInterfaceElement("MQTTTopic", "input", "Topic for MQTT", "#configform", "MQTTTopic");
  
  float Factor = iot.configuration.get("ScaleFactor").toFloat();
  if(Factor != 0){
     rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    scale.set_scale(Factor);
    scale.tare();
    char  Weight[50];
    sprintf(Weight, "%f", scale.get_units(10)  );
     rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
  
    char * MQTTTopic = iot.configuration.getCString("ScaleFactor"); 
    if(strlen(MQTTTopic) >1){
       iot.mqtt.publish(MQTTTopic, 1, true, Weight );
    }
    disp.begin(false);
    disp.fillScreen(BLUE);
    disp.setCursor(10,10);
    disp.setTextColor(WHITE);
    disp.setTextScale(4);
    disp.println(Weight);
    disp.setTextScale(3);
    disp.setCursor(90,50);
    disp.println(iot.configuration.get("ScaleUnit"));
  
    Serial.println(Weight);
  
    delay(25000);
    disp.changeMode(DISP_OFF);
    int Threshold = iot.configuration.get("ScaleWakeUp").toInt(); 
    touchAttachInterrupt(T4, callback, Threshold);
    esp_sleep_enable_touchpad_wakeup();
    Serial.println("Sleep");
    esp_deep_sleep_start();
    }
  }

void callback(){
  //empty callback for wakeup
  }

void loop(){
  //no loop
  }