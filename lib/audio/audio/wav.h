#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

#include <audio_base.h>

struct __attribute__((packed)) WAVHeader {
public:
    uint32_t riff;
    uint32_t chunkSize;
    uint32_t format;
    uint32_t subChunk1ID;
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    uint32_t data;
    uint32_t dataSize;
};

class WAV {
public:
    WAV(std::filesystem::path filepath) { loadFromFile(filepath); }
    ~WAV(){}
    
    std::vector<AudioSample> loadAudio() const {
        const size_t bytesPerSample = m_Header.bitsPerSample / 8;
        const size_t numSamples = m_Header.dataSize / (bytesPerSample * m_Header.numChannels);

        std::vector<AudioSample> samples;
        samples.reserve(numSamples);

        for (size_t i = 0; i < m_Header.dataSize; i += bytesPerSample * m_Header.numChannels) {
            AudioSample sample = {};
            switch (m_Header.numChannels) {
            case static_cast<uint16_t>(AudioType::MONO):
                sample.left = sample.right = getSampleValue(&m_AudioData[i], m_Header.bitsPerSample);
                break;
            case static_cast<uint16_t>(AudioType::STEREO):
                sample.left = getSampleValue(&m_AudioData[i], m_Header.bitsPerSample);
                sample.right = getSampleValue(&m_AudioData[i + bytesPerSample], m_Header.bitsPerSample);
                break;
            default:
                throw std::runtime_error("Unsupported number of channels");
            }
            samples.push_back(sample);
        }

        return samples;
    }

private:
    void loadFromFile(std::filesystem::path filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&m_Header, sizeof(WAVHeader));
            m_AudioData.resize(m_Header.dataSize);
            file.read(m_AudioData.data(), m_Header.dataSize);
            file.close();
        } else {
            throw std::runtime_error("Could not open WAV file");
        }
    }

    int32_t getSampleValue(const char* buffer, uint16_t bitsPerSample) const {
        switch (bitsPerSample) {
        case static_cast<uint16_t>(AudioFormat::BIT_8):
            return static_cast<int32_t>(*reinterpret_cast<const uint8_t*>(buffer)) - 128;
        case static_cast<uint16_t>(AudioFormat::BIT_16):
            return static_cast<int32_t>(*reinterpret_cast<const int16_t*>(buffer));
        case static_cast<uint16_t>(AudioFormat::BIT_24): {
            int32_t value = (static_cast<int32_t>(buffer[2]) << 16) | 
                            (static_cast<int32_t>(buffer[1]) << 8) | 
                            (static_cast<int32_t>(buffer[0]));
            if (value & 0x800000) value |= 0xFF000000;
            return value;
        }
        case static_cast<uint16_t>(AudioFormat::BIT_32):
            return *reinterpret_cast<const int32_t*>(buffer);
        default:
            throw std::runtime_error("Unsupported bits per sample");
        }
    }

private:
    WAVHeader m_Header;
    std::vector<char> m_AudioData;
};
