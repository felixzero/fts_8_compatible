#include "tone_generation.h"
#include "carrier_detection.h"
#include "digital_io.h"

#define DETECTION_THRESHOLD_START   32
#define DETECTION_THRESHOLD_END     8

static bool detected = false;

#include <avr/delay.h>

int main()
{
    init_digital_io();
    init_tone_generation();
    init_carrier_detection();

    while (1) {
        set_tone(FREQUENCY_SELECTION_IN);

        if (!EN_SWITCH_IN) {
            disable_output_tone();
            set_detection_out(true);
            continue;
        }

        if (TX_SWITCH_IN) {
            enable_output_tone();
        } else {
            disable_output_tone();
        }

        if (detected_carrier_strength > DETECTION_THRESHOLD_START) {
            detected = true;
        } else if (detected_carrier_strength <= DETECTION_THRESHOLD_END) {
            detected = false;
        }
        set_detection_out(detected);
    }
    return 0;
}
