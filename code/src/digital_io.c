#include "digital_io.h"

void init_digital_io(void)
{
    DDRD |= _BV(2) | _BV(3);
}

void set_detection_out(bool detected)
{
    if (detected) {
        PORTD |= _BV(3);
        PORTD |= _BV(2);
    } else {
        PORTD &= ~_BV(3);
        PORTD &= ~_BV(2);
    }
}
