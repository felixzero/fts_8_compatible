#include "tone_generation.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "sample_data.h"

static volatile uint16_t downsampling_factor = 0;
static volatile const uint8_t *sine_sample_ptr = 0;
static volatile uint8_t current_sample = 0;
static volatile bool is_output_enabled = false;

ISR(TIMER1_OVF_vect) {
    ICR1 = downsampling_factor;
    OCR1A = pgm_read_byte(sine_sample_ptr + current_sample++);
    current_sample %= BUFFER_LENGTH;
}

void init_tone_generation(void)
{
    set_tone(0);

    // Set pin as output
    DDRB |= _BV(1);

    cli();

    // Phase correct PWM on OC1A, no prescaler
    TCCR1A = _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);

    ICR1 = 1;
    OCR1A = 0;

    // Enable overflow interrupt
    TIMSK1 = _BV(TOIE1);

    sei();
}

void set_tone(uint8_t tone_index)
{
    if (tone_index > TONE_PERIOD_ARRAY_LENGTH) {
        return;
    }

    downsampling_factor = pgm_read_word(tone_periods + tone_index);
    sine_sample_ptr = sine_samples[tone_index];
}

void enable_output_tone(void)
{
    TCCR1A |= _BV(COM1A1);
    is_output_enabled = true;
}

void disable_output_tone(void)
{
    TCCR1A &= ~_BV(COM1A1);
    is_output_enabled = false;
}
