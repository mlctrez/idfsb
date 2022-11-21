/*----------------------------------------
  Sensory Bridge GLOBAL CONSTANTS
----------------------------------------*/

#define BUFFER_SIZE 256
#define BUFFER_COUNT 1

#define FRAME_SIZE (BUFFER_SIZE * BUFFER_COUNT)
#define FFT_SIZE FRAME_SIZE / 2

#define SAMPLE_RATE 18750
#define I2S_PORT I2S_NUM_0

// I2S pins
#define I2S_BCLK_PIN  GPIO_NUM_33
#define I2S_LRCLK_PIN GPIO_NUM_34
#define I2S_DOUT_PIN  GPIO_NUM_35

#define LED_DATA_PIN GPIO_NUM_36
#define LED_CLOCK_PIN GPIO_NUM_37

#define RNG_SEED_PIN GPIO_NUM_17

#define SWEET_SPOT_LEFT_PIN    GPIO_NUM_7
#define SWEET_SPOT_CENTER_PIN  GPIO_NUM_8
#define SWEET_SPOT_RIGHT_PIN   GPIO_NUM_9

#define NOISE_CAL_PIN  GPIO_NUM_11
#define MODE_PIN       GPIO_NUM_10

#define PHOTONS_PIN GPIO_NUM_1
#define CHROMA_PIN  GPIO_NUM_2
#define MOOD_PIN    GPIO_NUM_3