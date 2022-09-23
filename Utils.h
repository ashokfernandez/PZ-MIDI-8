
#include <Arduino.h>
#ifndef UTILS_H
#define UTILS_H

#define CLIP_WRAP_AROUND true
#define CLIP_CLAMP_VALUE false

int8_t clipValue (int8_t value, int8_t min, int8_t max, bool wrap);

#endif