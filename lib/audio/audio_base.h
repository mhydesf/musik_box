#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace MusikBox::Audio {

enum class FileType {
    WAV = 0,
    MP3 = 1
};

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

struct AudioSample {
    int16_t left;
    int16_t right;
};

class I_Audio {
public:
    virtual std::vector<AudioSample> loadAudio() const = 0;

    static std::vector<int16_t> convertStereoToMono_AVG(const std::vector<AudioSample>& audioData) {
        std::vector<int16_t> output;
        output.resize(audioData.size());

        size_t i = 0;
        for (const auto& sample : audioData) {
            int16_t val = (sample.left + sample.right) / 2;
            output[i] = val;
            i++;
        }
        return output;
    }

protected:
    virtual void loadFromFile(std::filesystem::path filepath) = 0;

protected:
    std::vector<char> m_AudioData;
};

} // namespace MusikBox::Audio
