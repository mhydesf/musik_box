#pragma once

#include <cstdint>

enum class AudioType {
    MONO = 1,
    STEREO = 2
};

struct __attribute__((packed)) AudioSample {
    int16_t left;
    int16_t right;
};

