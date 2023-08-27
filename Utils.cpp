#include "Utils.h"

int8_t clipValue(int8_t value, int8_t min, int8_t max, bool wrap = false) {
    // If we're wrapping around; set values below min to max, set values above max to min
    if (wrap) {
        if (value > max) { 
            value = min; 
        } else if (value < min) {
            value = max;  
        }
    } else {
    // Clip values above max to max, values below min to min
      value = constrain(value, min, max);
    }

    return value;
}