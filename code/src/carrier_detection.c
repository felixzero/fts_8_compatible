#include "carrier_detection.h"
#include "sample_data.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define DEBUG_ANALOG_OUTPUT     OCR2A

// Timer is set such as its sampling frequency is such as there exists an integer OMEGA_FACTOR and an integer p
// such as cos(2pi / p) ~ 1 - 1/2**OMEGA_FACTOR
// This renders the Goertzel algorithm calculation trivial (solely adding, subing and power of two factors)
// In practice, OMEGA_FACTOR=11 and p=201 works well enough (p=201.054 is the exact answer) (timer running à x201 the analysed frequency)
#define OMEGA_FACTOR        11
#define P_FACTOR            201
#define NUMBER_OF_PERIODS   32
#define DESCALING_FACTOR    12

#define COEFF_MULT(x)       (((x) << 1) - ((x) >> (OMEGA_FACTOR - 1)))

static volatile int32_t sprev, sprev2;
static volatile uint16_t sample_no = 0;
volatile uint16_t detected_carrier_strength;
ISR(ADC_vect) {
    int32_t s = (int32_t)ADC + COEFF_MULT(sprev) - sprev2;
    sprev2 = sprev;
    sprev = s;
    sample_no++;

    if (sample_no >= NUMBER_OF_PERIODS * P_FACTOR) {
        sprev >>= DESCALING_FACTOR;
        sprev2 >>= DESCALING_FACTOR;
        int32_t cross_term = sprev * sprev2;
        uint16_t power = (sprev * sprev + sprev2 * sprev2 - COEFF_MULT(cross_term));
        detected_carrier_strength = power;
        DEBUG_ANALOG_OUTPUT = sprev * sprev + sprev2 * sprev2 - COEFF_MULT(cross_term);

        sprev = sprev2 = 0;
        sample_no = 0;
    }
}

void init_carrier_detection(void)
{
    // Init debug analog output on OC2A (PB3)
    // Fast PWM to OC2A (to 255), no prescaling
    TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20);
    DDRB |= _BV(3);
    OCR2A = 50;

    // Init ADC synchronized over TIMER1 (used also for tone generation)
    cli();
    // Capacitor at AREF pin and input on ADC6
    ADMUX = _BV(REFS0) | 0b0110;
    // ADC enabled, with autotrigger, interrupt enabled and /16 division factor (1MHz ADC clock)
    ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);
    // Sampling on timer 1 overflow
    ADCSRB = _BV(ADTS2) | _BV(ADTS1);
    sei();
}
