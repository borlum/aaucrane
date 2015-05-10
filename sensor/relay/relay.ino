void setup() {
  Serial.begin(9600);
}

void loop() {
  char tmp[5] = {0};
  if(Serial.readBytes(tmp, 4) != 0){
    if(tmp[3] == ';')
      Serial.print(tmp);
  }
}
