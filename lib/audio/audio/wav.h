#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <fstream>
#include <filesystem>

enum class AudioType {
    MONO = 1,
    STEREO = 2
};

struct __attribute__((packed)) AudioSample {
    int16_t left;
    int16_t right;
};

struct __attribute__((packed)) WAVHeader {
public:
    uint32_t riff;
    uint32_t fileSize;
    uint32_t fileHeader;
    uint32_t format;
    uint32_t dataLength;
    uint16_t formatType;
    uint16_t numberChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    uint32_t data;
    uint16_t dataSize;

    static const WAVHeader loadFromFile(std::filesystem::path filepath) {
        WAVHeader header;
        std::ifstream file(filepath, std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&header, sizeof(WAVHeader));
            file.close();
        } else {
            throw std::runtime_error("Could not open WAV file");
        }
        return header;
    }
};

class WAV {
public:
    WAV(std::filesystem::path filepath)
        : m_Filepath(filepath)
        , m_Header(WAVHeader::loadFromFile(m_Filepath)){}

    ~WAV(){}
    
    std::vector<AudioSample> loadAudio() {
        std::vector<AudioSample> samples;

        std::ifstream file(m_Filepath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open WAV file to read audio data");
        }

        file.seekg(sizeof(WAV), std::ios::beg);

        uint16_t bytesPerSample = m_Header.bitsPerSample / 8;
        uint32_t numSamples = m_Header.dataSize / (bytesPerSample * m_Header.numberChannels);

        samples.reserve(numSamples);
        std::vector<char> buffer(bytesPerSample * m_Header.numberChannels);

        for (uint32_t i = 0; i < numSamples; ++i) {
            file.read(buffer.data(), buffer.size());

            AudioSample sample = {};

            switch (AudioType(m_Header.numberChannels)) {
            case (AudioType::MONO):
                sample.left = *reinterpret_cast<int16_t*>(buffer.data());
                sample.right = sample.left;
                break;
            case (AudioType::STEREO):
                sample.left = *reinterpret_cast<int16_t*>(buffer.data());
                sample.right = *reinterpret_cast<int16_t*>(buffer.data() + bytesPerSample);
                break;
            default:
                throw std::runtime_error("More than two channels not supported");
            }

            samples.push_back(sample);
        }

        file.close();
        return samples;
    }

private:
    std::filesystem::path m_Filepath;
    WAVHeader m_Header;
};
