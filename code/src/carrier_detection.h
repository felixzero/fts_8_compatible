#pragma once

#include <stdint.h>

void init_carrier_detection(void);

extern volatile uint16_t detected_carrier_strength;
