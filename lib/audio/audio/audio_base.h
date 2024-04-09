#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <filesystem>

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

class I_Audio {
public:
    virtual std::vector<AudioSample> loadAudio() const = 0;

protected:
    virtual void loadFromFile(std::filesystem::path filepath) = 0;

protected:
    std::vector<char> m_AudioData;
};
