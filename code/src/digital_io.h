#pragma once

#include <avr/io.h>
#include <stdbool.h>

void init_digital_io(void);
void set_detection_out(bool detected);

#define FREQ_SELECTION_MASK 0x3F
#define FREQUENCY_SELECTION_IN (PINC & FREQ_SELECTION_MASK)

#define EN_SWITCH_IN (PIND & _BV(0))
#define TX_SWITCH_IN (PIND & _BV(1))
