#!/bin/env python3

import math

BUFFER_LENGTH = 201
F_CPU = 16_000_000

ctcss_tones = [
    None,  None,  None,  None,  None,  None,  None,  None,
    None,  None,  None,  None,  None,  None,  None,  None, 
    None,  None,  None,  None,  None,  91.5,  88.5,  85.4,
    82.5,  79.7,  77.0,  74.4,  71.9,  67.0,  250.3, 241.8,
    233.6, 225.7, 218.1, 210.7, 203.5, 192.8, 186.2, 179.9,
    173.8, 167.9, 162.2, 156.7, 151.4, 146.2, 141.3, 136.5,
    131.8, 127.3, 123.0, 118.8, 114.8, 110.9, 107.2, 103.5,
    100.0, 94.8
]

with open("src/sample_data.c", "w") as f:
    f.write('#include "sample_data.h"\n\n')

    f.write('const uint16_t tone_periods[] PROGMEM = { ')
    tone_periods = ["0" if tone is None else str(round(F_CPU / BUFFER_LENGTH / tone)) for tone in ctcss_tones]
    f.write(", ".join(tone_periods))
    f.write(' };\n\n')

    f.write('const uint8_t sine_samples[][256] PROGMEM = {\n')
    for period in tone_periods:
        period = int(period)
        sine_wave = [str(period * round(255 * (0.5 + 0.5 * math.sin(2 * math.pi * i / BUFFER_LENGTH))) >> 12) for i in range(BUFFER_LENGTH)]
        f.write('\t{ ')
        f.write(", ".join(sine_wave))
        f.write(' },\n')
    f.write('};\n\n')
