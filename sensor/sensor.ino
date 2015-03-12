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
#define NR_PIXELS  8

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
};

typedef struct sensor_t sensor_t;

/**
 * ----------------------------------------------------------------------------
 * PROTOTYPES
 * ----------------------------------------------------------------------------
 */

void enable_CLK();
void disable_CLK();
void init_sensor(sensor_t *sensor, gpio_t si, gpio_t led, uint8_t ao);
void enable_sensor_LED(sensor_t *sensor);
void disable_sensor_LED(sensor_t *sensor);
void enable_sensor_SI(sensor_t *sensor);
void disable_sensor_SI(sensor_t *sensor);
uint32_t read_sensor(sensor_t sensor);
void dio(Pio *port, uint32_t mask, uint8_t state);

/**
 * ----------------------------------------------------------------------------
 * GLOBALS
 * ----------------------------------------------------------------------------
 */
sensor_t sensor_array[NR_SENSORS];

gpio_t CLK  = {PIOB, (1 << 12), 20}; //20

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
    gpio_t LED1 = {PIOD, (1 <<  5), 15};

    gpio_t SI1  = {PIOA, (1 << 11), 18};
    gpio_t LED2 = {PIOD, (1 << 27), 13};

    gpio_t SI3  = {PIOA, (1 << 10), 19};
    gpio_t LED3 = {PIOD, (1 <<  4), 14};

    init_sensor(&sensor1, SI1, LED1,  0);
    init_sensor(&sensor2, SI2, LED2,  1);
    init_sensor(&sensor3, SI3, LED3,  2);
    
    sensor_array[0] = sensor1;
    sensor_array[1] = sensor2;
    sensor_array[2] = sensor3;

    pinMode(CLK.arduino_pin, OUTPUT);
    enable_sensor_SI(&sensor_array[0]);
}

/**
 * ----------------------------------------------------------------------------
 * LOOP...
 * ----------------------------------------------------------------------------
 */
void loop()
{   
    enable_CLK();
    delayMicroseconds(5);
    disable_CLK();


    /*enable_sensor_SI(&sensor_array[1]);
    enable_sensor_SI(&sensor_array[2]);
    enable_sensor_LED(&sensor_array[0]);
    enable_sensor_LED(&sensor_array[1]);
    enable_sensor_LED(&sensor_array[2]);
    delayMicroseconds(5);
    disable_CLK();
    disable_sensor_SI(&sensor_array[0]);
    disable_sensor_SI(&sensor_array[1]);
    disable_sensor_SI(&sensor_array[2]);
    disable_sensor_LED(&sensor_array[0]);
    disable_sensor_LED(&sensor_array[1]);
    disable_sensor_LED(&sensor_array[2]);
    delayMicroseconds(5);*/

    //uint32_t data[NR_SENSORS][NR_PIXELS];
    /*Collect data...*/
    /*for (uint8_t sensor_nr = 0; sensor_nr < NR_SENSORS; sensor_nr++) {
        enable_sensor_LED(&sensor_array[sensor_nr]);
        enable_sensor_SI(&sensor_array[sensor_nr]);
        enable_CLK();
        delayMicroseconds(1);
        disable_sensor_SI(&sensor_array[sensor_nr]);
        disable_CLK();
        delayMicroseconds(1);
        for (uint8_t pixel_nr = 0; pixel_nr < NR_PIXELS; pixel_nr++) {
            data[sensor_nr][pixel_nr] = read_sensor(sensor_array[sensor_nr]);
            enable_CLK();
            delayMicroseconds(1);
            disable_CLK();
            delayMicroseconds(1);
        }
    }*/
    /*Process...*/
    /*???*/
    /*Output...*/
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

uint32_t read_sensor(sensor_t sensor)
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