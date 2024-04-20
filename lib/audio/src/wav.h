#pragma once

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
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
    }

    ~WAV(){}
    
    std::vector<AudioSample> loadAudio() const override {
        const size_t bytesPerSample = m_Header.bitsPerSample / 8;
        const size_t numSamples = m_Header.dataSize / (bytesPerSample * m_Header.numChannels);

        std::vector<AudioSample> samples;
        samples.reserve(numSamples);

        bool isStereo = false;
        switch (m_Header.numChannels) {
        case (static_cast<uint16_t>(AudioType::MONO)):
            break;
        case (static_cast<uint16_t>(AudioType::STEREO)):
            isStereo = true;
            break;
        default:
            throw std::runtime_error("Unsupported audio type");
        }

        for (size_t i = 0; i < m_Header.dataSize; i += bytesPerSample * m_Header.numChannels) {
            AudioSample sample = {};
            sample.left = m_GetSampleValue(&m_AudioData[i]);
            if (isStereo) {
                sample.right = m_GetSampleValue(&m_AudioData[i + bytesPerSample]);
            } else {
                sample.right = sample.left;
            }
            samples.push_back(sample);
        }
        return samples;
    }
    
    uint16_t getNumChannels() { return m_Header.numChannels; }

    uint32_t getSampleRate() { return m_Header.sampleRate; }

    uint32_t getByteRate() { return m_Header.byteRate; }

    uint16_t getBitsPerSample() { return m_Header.bitsPerSample; }

protected:
    void loadFromFile(std::filesystem::path filepath) override {
        int fd = open(filepath.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Could not open wav file");
        }
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            close(fd);
            throw std::runtime_error("Could not get file size");
        }
        char* addr = static_cast<char*>(
            mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)
        );
        if (addr == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("Memory mapping failed");
        }
        std::memcpy(&m_Header, addr, sizeof(WAVHeader));
        m_AudioData = std::vector<char>(addr+sizeof(WAVHeader), addr + sb.st_size);
        
        munmap(addr, sb.st_size);
        close(fd);
    }

private:
    inline int32_t getSampleValue_8_bit(const char* buffer) const {
        return static_cast<int32_t>(*reinterpret_cast<const uint8_t*>(buffer)) - 128;
    }
    
    inline int32_t getSampleValue_16_bit(const char* buffer) const {
        return static_cast<int32_t>(*reinterpret_cast<const int16_t*>(buffer));
    }

    inline int32_t getSampleValue_24_bit(const char* buffer) const {
        int32_t value = (static_cast<int32_t>(buffer[2]) << 16) | 
                        (static_cast<int32_t>(buffer[1]) << 8) | 
                        (static_cast<int32_t>(buffer[0]));
        if (value & 0x800000) value |= 0xFF000000;
        return value;
    }

    inline int32_t getSampleValue_32_bit(const char* buffer) const {
        return *reinterpret_cast<const int32_t*>(buffer);
    }

    void setAudioFormat(uint16_t format) {
        switch(format) {
        case (static_cast<uint16_t>(AudioFormat::BIT_8)):
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_8_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_16)):
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_16_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_24)):
            m_GetSampleValue = [this](const char* buffer) {
                return this->getSampleValue_24_bit(buffer);
            };
            break;
        case (static_cast<uint16_t>(AudioFormat::BIT_32)):
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
    std::function<int32_t(const char*)> m_GetSampleValue;
};
