/**
 * ============================================================================
 * AAU CRANE SENSOR
 * ----------------
 * Optical, driven by a dedicated Arduino Due!
 * No use of digital write, instead a custom function
 * Use http://arduino.cc/en/Hacking/PinMappingSAM3X to see corresponding
 * pins and ports on SAM3X.
 * ============================================================================
 * STUFF THAT NEEEDS TO BE DONE:
 * - Blind angles - How should we react?
 * - How do we find the wire when only some of the "dip" is visible?
 * - Translate the found falling edge into an actual position of the wire. Maybe using the width?
 */
#define NR_SENSORS 3
#define NR_PIXELS  128
#define WIRE_WIDTH_PX 30
#define NR_PIXELS_W_DEADBAND 188
#define DEADBAND_WIDTH 60

const int MAGIC_THRESHOLD[] = {50, 50, 50};

int DEAD_PIXELS[NR_SENSORS][NR_PIXELS] = { {0} };
/**
 * ----------------------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------------------
 */
struct gpio_t {
  Pio *port;
  uint32_t mask;
  uint32_t arduino_pin;
};

typedef struct gpio_t gpio_t;

struct sensor_t {
  /* I/O info */
  gpio_t pin_SI;
  gpio_t pin_LED;
  uint8_t pin_AO;
  int pixels[NR_PIXELS] = { -1 }; /* Raw sensor data */
};

typedef struct sensor_t sensor_t;

struct wire_location_t {
  int pixel_value;
  int sensor_id;
  int pixel_id;
};

typedef struct wire_location_t wire_location_t;

/**
 * ----------------------------------------------------------------------------
 * PROTOTYPES
 * ----------------------------------------------------------------------------
 */

void enable_CLK();
void disable_CLK();
void run_CLK_n_times(uint8_t n);
void init_sensor(sensor_t *sensor, gpio_t si, gpio_t led, uint8_t ao);
void enable_sensor_LED(sensor_t *sensor);
void disable_sensor_LED(sensor_t *sensor);
void enable_sensor_SI(sensor_t *sensor);
void disable_sensor_SI(sensor_t *sensor);
uint16_t read_sensor(sensor_t sensor);
void dio(Pio *port, uint32_t mask, uint8_t state);
int get_wire_location(wire_location_t* wire_loc);

/**
 * ----------------------------------------------------------------------------
 * GLOBALS
 * ----------------------------------------------------------------------------
 */
sensor_t sensor_array[NR_SENSORS];

int normal[3] = {160, 155, 180};

gpio_t CLK   = {PIOB, (1 << 12), 20};

gpio_t TEST  = {PIOB, (1 << 13), 21};

/**
 * ----------------------------------------------------------------------------
 * SETUP...
 * ----------------------------------------------------------------------------
 */
void setup()
{
  DEAD_PIXELS[2][6] = 1;
  DEAD_PIXELS[2][7] = 1;
  DEAD_PIXELS[2][8] = 1;
  
  Serial.begin(9600);

  sensor_t sensor1, sensor2, sensor3;

  /* Initialize pins! */
  gpio_t SI2  = {PIOA, (1 << 12), 17};
  gpio_t LED1 = {PIOA, (1 << 13), 16};

  gpio_t SI1  = {PIOA, (1 << 11), 18};
  gpio_t LED2 = {PIOD, (1 <<  4), 14};

  gpio_t SI3  = {PIOA, (1 << 10), 19};
  gpio_t LED3 = {PIOD, (1 <<  5), 15};

  init_sensor(&sensor1, SI1, LED1,  A0);
  init_sensor(&sensor2, SI2, LED2,  A1);
  init_sensor(&sensor3, SI3, LED3,  A2);

  sensor_array[0] = sensor1;
  sensor_array[1] = sensor2;
  sensor_array[2] = sensor3;

  pinMode(CLK.arduino_pin, OUTPUT);
  pinMode(TEST.arduino_pin, OUTPUT);

  disable_sensor_LED(&sensor_array[0]);
  disable_sensor_LED(&sensor_array[1]);
  disable_sensor_LED(&sensor_array[2]);

  analogWriteResolution(10);
  
}

/**
 * ----------------------------------------------------------------------------
 * LOOP...
 * ----------------------------------------------------------------------------
 */
wire_location_t wire_loc;
void loop()
{
  /*Collect data...*/
  for (uint8_t sensor_nr = 0; sensor_nr < NR_SENSORS; sensor_nr++) {
    /*Lets read a sensor!*/
    enable_sensor_LED(&sensor_array[sensor_nr]);
    enable_sensor_SI(&sensor_array[sensor_nr]);
    run_CLK_n_times(1);

    /*Shift out two dummy pixels - run clock twice*/
    run_CLK_n_times(2);
    /*Now sample the entire sensor (128 pixels)*/
    for (uint8_t pixel_nr = 0; pixel_nr < NR_PIXELS; pixel_nr++) {
      if (pixel_nr == 17) {
        disable_sensor_SI(&sensor_array[sensor_nr]);
      }
      enable_CLK();
      delayMicroseconds(1);
      disable_CLK();
      delayMicroseconds(1);
    }
    /*Shift out two dummy pixels + one final shift to reset*/
    run_CLK_n_times(3);

    disable_sensor_LED(&sensor_array[sensor_nr]);
  }

  /*Collect data...*/
  for (uint8_t sensor_nr = 0; sensor_nr < NR_SENSORS; sensor_nr++) {
    /*Lets read a sensor!*/
    enable_sensor_LED(&sensor_array[sensor_nr]);
    enable_sensor_SI(&sensor_array[sensor_nr]);
    run_CLK_n_times(1);

    /*Shift out two dummy pixels - run clock twice*/
    run_CLK_n_times(2);
    /*Now sample the entire sensor (128 pixels)*/
    for (uint8_t pixel_nr = 0; pixel_nr < NR_PIXELS; pixel_nr++) {
      if (pixel_nr == 17) {
        disable_sensor_SI(&sensor_array[sensor_nr]);
      }
      enable_CLK();
      delayMicroseconds(1);
      disable_CLK();

      if (sensor_nr != 1) {
        sensor_array[sensor_nr].pixels[(NR_PIXELS - 1) - pixel_nr] =
          read_sensor(sensor_array[sensor_nr]);
      }
      else {
        sensor_array[sensor_nr].pixels[pixel_nr] =
          read_sensor(sensor_array[sensor_nr]);
      }

      delayMicroseconds(1);
    }
    /*Shift out two dummy pixels + one final shift to reset*/
    run_CLK_n_times(4); // Was 3

    disable_sensor_LED(&sensor_array[sensor_nr]);
  }
  
  /*for(int i = 0; i < NR_SENSORS; i++){
    for(int j = 5; j < NR_PIXELS; j++){
      Serial.print(i);
      Serial.print(',');
      Serial.println(sensor_array[i].pixels[j]);
    }
  }*/
  
  /*
  Serial.print(wire_loc.sensor_id);
  Serial.print(',');
  Serial.println(wire_loc.pixel_id);*/
  get_wire_location(&wire_loc);
  //Serial.println(wire_loc.pixel_id);

  //wire_loc.pixel_id = 285;
  
  analogWrite(DAC1, map(wire_loc.pixel_id, 0, 3 * NR_PIXELS_W_DEADBAND, 0, 1024));
  
}

/**
 * ----------------------------------------------------------------------------
 * FUNCTIONS
 * ----------------------------------------------------------------------------
 */

void enable_CLK()
{
  dio(&(*CLK.port), CLK.mask, 1);
}

void disable_CLK()
{
  dio(&(*CLK.port), CLK.mask, 0);
}

void run_CLK_n_times(uint8_t n)
{
  for (uint8_t i = 0; i < n; i++) {
    enable_CLK();
    delayMicroseconds(1);
    disable_CLK();
    delayMicroseconds(1);
  }
}

void enable_sensor_LED(sensor_t *sensor)
{
  dio(&(*(sensor->pin_LED).port), (sensor->pin_LED).mask, 1);
}

void disable_sensor_LED(sensor_t *sensor)
{
  dio(&(*(sensor->pin_LED).port), (sensor->pin_LED).mask, 0);
}

void enable_sensor_SI(sensor_t *sensor)
{
  dio(&(*(sensor->pin_SI).port), (sensor->pin_SI).mask, 1);
}

void disable_sensor_SI(sensor_t *sensor)
{
  dio(&(*(sensor->pin_SI).port), (sensor->pin_SI).mask, 0);
}

uint16_t read_sensor(sensor_t sensor)
{
  return analogRead(sensor.pin_AO);
}

void init_sensor(sensor_t *sensor, gpio_t si, gpio_t led, uint8_t ao)
{
  sensor->pin_SI  = si;
  sensor->pin_LED = led;
  sensor->pin_AO  = ao;
  pinMode(sensor->pin_SI.arduino_pin,  OUTPUT);
  pinMode(sensor->pin_LED.arduino_pin, OUTPUT);
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


int get_wire_location(wire_location_t* wire_loc) {
  int start = -1, wire_end = -1, diff;
  
  for(int i = 0; i < NR_SENSORS; i++){
    for(int j = 5; j < NR_PIXELS; j++){
      if(is_dead_pixel(i,j) || is_dead_pixel(i,j-5) )
        continue;

      diff  = sensor_array[i].pixels[j] - sensor_array[i].pixels[j - 5];

      if( (start == -1) && (diff < -MAGIC_THRESHOLD[i]) ){
        start = j;
      }
      else if( (wire_end == -1) && (diff > MAGIC_THRESHOLD[i]) ){
        wire_end = j;
      }
      
      if( (start != -1) && (wire_end != -1) ){ // Hvis vi har fundet begge flanker
        wire_loc->sensor_id = i;
        wire_loc->pixel_id = ( (start + wire_end) / 2 ) + (NR_PIXELS_W_DEADBAND * i) + DEADBAND_WIDTH/2;
        wire_loc->pixel_value = sensor_array[i].pixels[wire_loc->pixel_id];
        return wire_loc->pixel_value;
      } 
    }
    
    if( (start != -1) || (wire_end != -1) ){
      wire_loc->sensor_id = i;
      
      if(start != -1){
        wire_loc->pixel_id = start + (NR_PIXELS_W_DEADBAND * i) + WIRE_WIDTH_PX/2 + DEADBAND_WIDTH/2;
      }else if (wire_end != -1){
        wire_loc->pixel_id = wire_end + (NR_PIXELS_W_DEADBAND * i) - WIRE_WIDTH_PX/2 + DEADBAND_WIDTH/2;
      }
      
      wire_loc->pixel_value = sensor_array[i].pixels[wire_loc->pixel_id];
      return wire_loc->pixel_value;
    }
  }
  return -1;
}

bool is_dead_pixel(uint8_t sensor, uint8_t pixel){
  return DEAD_PIXELS[sensor][pixel] == 1;
}
