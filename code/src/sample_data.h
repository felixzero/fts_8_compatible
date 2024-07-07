#include <avr/pgmspace.h>
#include <stdint.h>

#define TONE_PERIOD_ARRAY_LENGTH        0x39
#define BUFFER_LENGTH                   201

extern const uint8_t sine_samples[][256] PROGMEM;

extern const uint16_t tone_periods[] PROGMEM;
