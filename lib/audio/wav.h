#pragma once

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdexcept>
#include <functional>
#include <audio_base.h>

struct WAVHeader {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
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
        const size_t numSamples = m_Header.subchunk2Size / (bytesPerSample * m_Header.numChannels);

        bool isStereo = (m_Header.numChannels == static_cast<uint16_t>(AudioType::STEREO));

        const int NUM_THREADS = std::thread::hardware_concurrency();
        const int SUB_ARRAY_SIZE = numSamples / NUM_THREADS;

        std::vector<std::thread> threads;
        std::vector<std::vector<AudioSample>> results(NUM_THREADS);

        for (int i = 0; i < NUM_THREADS; i++) {
            int start = i * SUB_ARRAY_SIZE;
            int end = (i + 1) * SUB_ARRAY_SIZE;
            if (i == NUM_THREADS - 1) end = numSamples;

            results[i].reserve(SUB_ARRAY_SIZE);
            threads.emplace_back(&WAV::processSubArray,
                                 this,
                                 std::ref(results[i]),
                                 bytesPerSample,
                                 isStereo,
                                 start * bytesPerSample * m_Header.numChannels,
                                 end * bytesPerSample * m_Header.numChannels);
        }

        for (auto& t : threads) {
            t.join();
        }

        std::vector<AudioSample> samples;
        samples.reserve(numSamples);
        for (auto& res : results) {
            samples.insert(samples.end(), res.begin(), res.end());
        }

        return samples;
    }

    void processSubArray(std::vector<AudioSample>& target,
                         size_t bytesPerSample,
                         bool isStereo,
                         int start,
                         int end) const {
        for (size_t i = start; i < end; i += bytesPerSample * m_Header.numChannels) {
            AudioSample sample = {};
            sample.left = m_GetSampleValue(&m_AudioData[i]);
            if (isStereo) {
                sample.right = m_GetSampleValue(&m_AudioData[i + bytesPerSample]);
            } else {
                sample.right = sample.left;
            }
            target.push_back(sample);
        }
    }

    AudioType getNumChannels() { return static_cast<AudioType>(m_Header.numChannels); }

    uint32_t getSampleRate() { return m_Header.sampleRate; }

    uint32_t getByteRate() { return m_Header.byteRate; }

    uint16_t getBitsPerSample() { return m_Header.bitsPerSample; }

protected:
    void loadFromFile(std::filesystem::path filepath) override {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open WAV file");
        }
        file.read(reinterpret_cast<char*>(&m_Header), sizeof(WAVHeader));
        if (std::strncmp(m_Header.chunkID, "RIFF", 4) != 0 || std::strncmp(m_Header.format, "WAVE", 4) != 0) {
            throw std::runtime_error("Invalid WAV file");
        }
        m_AudioData.resize(m_Header.subchunk2Size);
        file.read(m_AudioData.data(), m_Header.subchunk2Size);
        if (!file) {
            throw std::runtime_error("Error reading WAV data");
        }
        file.close();
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
