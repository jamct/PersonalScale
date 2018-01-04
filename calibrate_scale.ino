#include "HX711.h"
#include "soc/rtc.h"
HX711 scale;

void setup(){
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  Serial.begin(115200);
  delay(2000);
  scale.begin(25,26);
  scale.set_scale();
  scale.tare();
  Serial.println("Legen Sie in den nächsten 20 Sekunden ein bekanntes Gewicht von mindestens 10kg auf.");
  delay(20000);
  Serial.println(scale.get_units(10));
  Serial.println("Notieren Sie den Wert und teilen Sie ihn durch das bekannte Gewicht. Anschließend können Sie die Waage in Betrieb nehmen.");
  }

void loop(){
   //
  }
