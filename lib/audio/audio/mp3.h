#pragma once

#include <filesystem>
#include <audio_base.h>

struct __attribute__((packed)) MP3Header {
};

class MP3 : public I_Audio {
public:
    MP3(std::filesystem::path filepath) {}
    ~MP3(){}

    std::vector<AudioSample> loadAudio() const override {
        std::vector<AudioSample> samples;
        return samples;
    }

protected:
    void loadFromFile(std::filesystem::path filepath) override {}

private:
    MP3Header m_Header;
};
