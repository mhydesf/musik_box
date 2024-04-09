#pragma once

#include <stdexcept>
#include <functional>
#include <audio_base.h>

struct __attribute__((packed)) WAVHeader {
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

class WAV : public I_Audio {
public:
    WAV(std::filesystem::path filepath) {
        loadFromFile(filepath);
        setAudioFormat(m_Header.bitsPerSample);
        setAudioType(m_Header.numChannels);
    }

    ~WAV(){}
    
    std::vector<AudioSample> loadAudio() const override {
        const size_t bytesPerSample = m_Header.bitsPerSample / 8;
        const size_t numSamples = m_Header.dataSize / (bytesPerSample * m_Header.numChannels);

        std::vector<AudioSample> samples;
        samples.reserve(numSamples);

        for (size_t i = 0; i < m_Header.dataSize; i += bytesPerSample * m_Header.numChannels) {
            AudioSample sample = m_LoadSample(i, bytesPerSample);
            samples.push_back(sample);
        }

        return samples;
    }

protected:
    void loadFromFile(std::filesystem::path filepath) override {
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

private:
    AudioSample loadSample_MONO(const size_t& index, const size_t& bytesPerSample) const {
        AudioSample sample = {};
        sample.left = sample.right = m_GetSampleValue(&m_AudioData[index]);
        return sample;
    }
    
    AudioSample loadSample_STEREO(const size_t& index, const size_t& bytesPerSample) const {
        AudioSample sample = {};
        sample.left = m_GetSampleValue(&m_AudioData[index]);
        sample.right = m_GetSampleValue(&m_AudioData[index + bytesPerSample]);
        return sample;
    }

    void setAudioType(uint16_t numChannels) {
        switch (numChannels) {
        case (static_cast<uint16_t>(AudioType::MONO)):
            m_AudioType = AudioType::MONO;
            m_LoadSample = [this](const size_t& i, const size_t& bps) {
                return this->loadSample_MONO(i, bps);
            };
            break;
        case (static_cast<uint16_t>(AudioType::STEREO)):
            m_AudioType = AudioType::STEREO;
            m_LoadSample = [this](const size_t& i, const size_t& bps) {
                return this->loadSample_STEREO(i, bps);
            };
            break;
        default:
            throw std::runtime_error("Unsupported audio type");
        }
    }
    
    int32_t getSampleValue_8_bit(const char* buffer) const {
        return static_cast<int32_t>(*reinterpret_cast<const uint8_t*>(buffer)) - 128;
    }
    
    int32_t getSampleValue_16_bit(const char* buffer) const {
        return static_cast<int32_t>(*reinterpret_cast<const int16_t*>(buffer));
    }

    int32_t getSampleValue_24_bit(const char* buffer) const {
        int32_t value = (static_cast<int32_t>(buffer[2]) << 16) | 
                        (static_cast<int32_t>(buffer[1]) << 8) | 
                        (static_cast<int32_t>(buffer[0]));
        if (value & 0x800000) value |= 0xFF000000;
        return value;
    }

    int32_t getSampleValue_32_bit(const char* buffer) const {
        return *reinterpret_cast<const int32_t*>(buffer);
    }

    void setAudioFormat(uint16_t format) {
        switch(format) {
        case (static_cast<uint16_t>(AudioFormat::BIT_8)):
            m_BitsPerSample = AudioFormat::BIT_8;
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_8_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_16)):
            m_BitsPerSample = AudioFormat::BIT_16;
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_16_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_24)):
            m_BitsPerSample = AudioFormat::BIT_24;
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_24_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_32)):
            m_BitsPerSample = AudioFormat::BIT_32;
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_32_bit(buffer);
            };
            break;
        default:
            throw std::runtime_error("Unsupported bits per sample");
        }
    }

private:
    WAVHeader m_Header;
    AudioFormat m_BitsPerSample;
    AudioType m_AudioType;

    std::function<int32_t(const char*)> m_GetSampleValue;
    std::function<AudioSample(const size_t&, const size_t&)> m_LoadSample;
};
