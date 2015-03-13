/**
 * DEBUG DEFINES
 */
//#define DEBUG

/**
 * ============================================================================
 * AAU CRANE SENSOR
 * ----------------
 * Optical, driven by a dedicated Arduino Due!
 * No use of digital write, instead a custom function
 * Use http://arduino.cc/en/Hacking/PinMappingSAM3X to see corresponding
 * pins and ports on SAM3X.
 * ============================================================================
 */
#define NR_SENSORS 3
#define NR_PIXELS  128

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
  gpio_t pin_SI;
  gpio_t pin_LED;
  uint8_t pin_AO;
  uint16_t pixels[NR_PIXELS] = { -1 }; /* Initialize to 0xFFFF */
  uint16_t min_value = -1;             /* Initialize to 0xFFFF */ 
  uint8_t min_index = -1;              /* Initialize to 0xFFFF */
  uint16_t max_value = 0;              /* Initialize to 0x0000 - since we are measureing max values */
  uint8_t max_index = -1;              /* Initialize to 0xFFFF */
  uint16_t normalized_min_value = -1;  /* Initialize to 0xFFFF */ 
  
};

typedef struct sensor_t sensor_t;

uint32_t t1, t2;

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

/**
 * ----------------------------------------------------------------------------
 * GLOBALS
 * ----------------------------------------------------------------------------
 */
sensor_t sensor_array[NR_SENSORS];

gpio_t CLK   = {PIOB, (1 << 12), 20};

gpio_t TEST  = {PIOB, (1 << 13), 21};

/**
 * ----------------------------------------------------------------------------
 * SETUP...
 * ----------------------------------------------------------------------------
 */
void setup()
{
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
}

/**
 * ----------------------------------------------------------------------------
 * LOOP...
 * ----------------------------------------------------------------------------
 */
void loop()
{   
  uint16_t data[NR_SENSORS][NR_PIXELS];
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
 
      if (sensor_nr != 1){
	sensor_array[sensor_nr].pixels[pixel_nr] =
	  read_sensor(sensor_array[sensor_nr]);
      }
      else{
	sensor_array[sensor_nr].pixels[pixel_nr] =
	  read_sensor(sensor_array[sensor_nr]);
      }
 
      delayMicroseconds(1);
    }
    /*Shift out two dummy pixels + one final shift to reset*/
    run_CLK_n_times(3);

    disable_sensor_LED(&sensor_array[sensor_nr]);
  }

  for(uint8_t i = 0; i < NR_SENSORS; i++){
    for(uint8_t j = 0; j < NR_PIXELS; j++){
      if(sensor_array[i].pixels[j] < sensor_array[i].min_value){
	sensor_array[i].min_value = sensor_array[i].pixels[j];
	sensor_array[i].min_index = j;
      }
      if(sensor_array[i].pixels[j] > sensor_array[i].max_value){
	sensor_array[i].max_value = sensor_array[i].pixels[j];
	sensor_array[i].max_index = j;
      }
    }
  }
  
  Serial.println(sensor_array[0].min_value);
  Serial.println(sensor_array[1].min_value);
  Serial.println(sensor_array[2].min_value);
  while(1);

  /**
   * Normalize each minimum value for the sensors, and compare these
   * to determind the position.
   */
  uint8_t sensor_index = -1;
  uint8_t pixel_index = -1;
  uint16_t global_min = -1;
  for(uint8_t i = 0; i < NR_SENSORS; i++){
    sensor_array[i].normalized_min_value =
      map(sensor_array[i].min_value,
	  sensor_array[i].min_value, sensor_array[i].max_value,
	  0, 1024);
    if(sensor_array[i].normalized_min_value < global_min){
      global_min = sensor_array[i].normalized_min_value;
      pixel_index = sensor_array[i].min_index;
      sensor_index = i;
    }
  }
    
  /* #ifdef DEBUG */
  /*     Serial.print("Wire determined to be at: \n"); */
  /*     Serial.print("Sensor index: "); Serial.print(sensor_index); Serial.print("\n"); */
  /*     Serial.print("Pixel index: "); Serial.print(pixel_index); Serial.print("\n"); */
  /*     Serial.print("With value: "); Serial.print(min); Serial.print("\n"); */
  /* #endif     */
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
