#pragma once

#include <cstdint>

enum class AudioType {
    MONO = 1,
    STEREO = 2
};

enum class AudioFormat {
    BIT_8 = 8,
    BIT_16 = 16,
    BIT_24 = 24,
    BIT_32 = 32
};

struct __attribute__((packed)) AudioSample {
    int16_t left;
    int16_t right;
};

