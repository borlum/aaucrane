/**
 * ============================================================================
 * AAU CRANE SENSOR
 * ============================================================================
 */

#define NR_SENSORS 3
#define NR_PIXELS  8

typedef struct gpio_ {
  Pio *port;
  uint32_t mask;
  uint32_t arduino_pin;
} gpio;

struct sensor_t {
    uint8_t pin_CLK;
    uint8_t pin_SI;
    uint8_t pin_LED;
    uint8_t pin_AO;
};

//gpio pin_example = {PIOD, (1 << 27), 13};
//dio(pin_example->port, pin_example->mask, 1);

typedef struct sensor_t sensor_t;

void init_sensor(sensor_t *sensor, uint8_t clk, uint8_t si, uint8_t led, uint8_t ao);
void enable_sensor_LED(sensor_t sensor);
void disable_sensor_LED(sensor_t sensor);
void enable_sensor_CLK(sensor_t sensor);
void disable_sensor_CLK(sensor_t sensor);
void enable_sensor_SI(sensor_t sensor);
void disable_sensor_SI(sensor_t sensor);
int read_sensor(sensor_t sensor);
void dio(Pio *port, uint32_t mask, uint8_t state);

sensor_t sensor1, sensor2, sensor3;
sensor_t sensor_array[NR_SENSORS] = {sensor1, sensor2, sensor3};

void setup()
{
    Serial.begin(9600);
    init_sensor(&sensor1, 5, 6,  13,  0);
    init_sensor(&sensor2, 5, 6,  13,  0);
    init_sensor(&sensor3, 5, 6,  13,  0);
}

void loop()
{   
    disable_sensor_LED(sensor1);
    delay(1000);
    enable_sensor_LED(sensor1);
    delay(1000);
    /*for (uint8_t sensor_nr = 0; sensor_nr < NR_SENSORS; sensor_nr++) {
        enable_sensor_LED(sensor_array[sensor_nr]);
    }*/
}

void enable_sensor_LED(sensor_t sensor)
{
    digitalWrite(sensor.pin_LED, HIGH);
}

void disable_sensor_LED(sensor_t sensor)
{
    digitalWrite(sensor.pin_LED, LOW);
}

void enable_sensor_CLK(sensor_t sensor)
{
    digitalWrite(sensor.pin_CLK, HIGH);
}

void disable_sensor_CLK(sensor_t sensor)
{
    digitalWrite(sensor.pin_CLK, LOW);
}

void enable_sensor_SI(sensor_t sensor)
{
    digitalWrite(sensor.pin_SI, HIGH);
}

void disable_sensor_SI(sensor_t sensor)
{
    digitalWrite(sensor.pin_SI, LOW);
}

int read_sensor(sensor_t sensor)
{
    return analogRead(sensor.pin_AO);
}

void init_sensor(sensor_t *sensor, uint8_t clk, uint8_t si, uint8_t led, uint8_t ao)
{
    pinMode(clk, OUTPUT);
    pinMode(si,  OUTPUT);
    pinMode(led, OUTPUT);
    sensor->pin_CLK = clk;
    sensor->pin_SI  = si;
    sensor->pin_LED = led;
    sensor->pin_AO  = ao;
}

void dio(Pio *port, uint32_t mask, uint8_t state)
{
  if (state) {
    port->PIO_SODR |= mask;
  }
  else {
    port->PIO_CODR |= mask;
  }
}