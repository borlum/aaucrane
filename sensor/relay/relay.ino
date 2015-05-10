int buf;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if( (buf = Serial.read()) > 0)
    Serial.write(buf);
}
