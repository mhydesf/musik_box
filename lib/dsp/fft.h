#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>
#include <complex>
#include <fftw3.h>

namespace MusikBox::DSP {

class FFTManager {
public:
    FFTManager()
        : in(nullptr)
        , out(nullptr)
        , plan(nullptr)
        , N(0) {}

    ~FFTManager() {
        cleanup();
    }

    std::vector<std::complex<double>> computeFFT(const std::vector<double>& input) {
        size_t inputSize = input.size();
        preparePlan(inputSize);

        std::vector<double> windowedInput = input;
        hanningWindow(windowedInput);

        std::copy(input.begin(), input.end(), in);

        fftw_execute(plan);

        double scale = 1.0 / static_cast<double>(N);
        std::vector<std::complex<double>> result(inputSize / 2 + 1);
        for (size_t i = 0; i < inputSize / 2 + 1; ++i) {
            result[i] = std::complex<double>(out[i][0], out[i][1]) * scale;
        }

        return result;
    }

    void reduceFFT(std::vector<std::complex<double>>& fft,
                   uint32_t sampleRate,
                   float maxFrequency) {
        size_t maxBin = getMaxBin(sampleRate, fft.size(), maxFrequency);
        fft.erase(fft.begin() + maxBin + 1, fft.end());
    }
    
    float getFreqencyStep(uint32_t sampleRate, size_t fftSize) {
        return static_cast<float>(sampleRate) / fftSize;
    }

private:
    void preparePlan(size_t newSize) {
        if (newSize != N) {
            cleanup();
            N = newSize;
            in = fftw_alloc_real(N);
            out = fftw_alloc_complex(N/2+1);
            plan = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
        }
    }

    void cleanup() {
        if (plan) fftw_destroy_plan(plan);
        if (in) fftw_free(in);
        if (out) fftw_free(out);
        plan = nullptr;
        in = nullptr;
        out = nullptr;
        N = 0;
    }

    void hanningWindow(std::vector<double>& input) {
        for (size_t i = 0; i < input.size(); ++i) {
            input[i] *= 0.5 * (1 - cos(2 * M_PI * i / (input.size() - 1)));
        }
    }

    size_t getMaxBin(uint32_t sampleRate,
                     size_t fftSize,
                     float maxFrequency) {
        float freq_step = getFreqencyStep(sampleRate ,fftSize);
        return static_cast<size_t>(maxFrequency / freq_step);
    }

private:
    double* in;
    fftw_complex* out;
    fftw_plan plan;
    size_t N;
};

} // namespace MusikBox::DSP
