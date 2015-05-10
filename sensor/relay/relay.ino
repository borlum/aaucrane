#include "TimerOne.h"

uint16_t buf = 0;

void setup() {
  Serial.begin(9600);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(callback);
}

void loop() {
  char tmp[2];
  if(Serial.readBytes(tmp, sizeof(tmp)) != 0){
    memcpy(&buf, tmp, sizeof(buf));
    Serial.print(buf, DEC);
    Serial.println();
  }
}

void callback(){;
  //Serial.println(atoi(buf));
}
