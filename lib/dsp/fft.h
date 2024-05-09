#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
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
        std::copy(windowedInput.begin(), windowedInput.end(), in);

        fftw_execute(plan);

        std::vector<std::complex<double>> result(inputSize / 2 + 1);
        for (size_t i = 0; i < inputSize / 2 + 1; ++i) {
            result[i] = std::complex<double>(out[i][0], out[i][1]);
        }

        return result;
    }

    std::vector<std::pair<float, std::complex<double>>> convertToPairVector(std::vector<std::complex<double>> data,
                                                                            const uint32_t sample_rate,
                                                                            std::optional<float> cutoff_freq = std::nullopt) {
        std::vector<std::pair<float, std::complex<double>>> result(data.size());
        const auto resolution = static_cast<float>(sample_rate) / m_NPad;
        size_t last_valid_idx = 0;
        
        size_t cutoff_index = cutoff_freq.has_value() ? static_cast<size_t>(cutoff_freq.value() / resolution) : data.size();
        for (size_t i = 0; i < data.size() && i <= cutoff_index; i++) {
            float x = i * resolution;
            std::complex<double> y = data[i];
            result[last_valid_idx] = std::pair(x, y);
            last_valid_idx++;
        }
        result.resize(last_valid_idx);
        return result;
    }

private:
    void preparePlan(size_t newSize) {
        if (newSize != N) {
            cleanup();
            N = newSize;
            while (m_NPad < N) { m_NPad *= 2; }
            in = fftw_alloc_real(m_NPad);
            out = fftw_alloc_complex(m_NPad/2+1);
            plan = fftw_plan_dft_r2c_1d(m_NPad, in, out, FFTW_ESTIMATE);
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

private:
    double* in;
    fftw_complex* out;
    fftw_plan plan;
    size_t N;

    size_t m_NPad = 1 << 16;
};

} // namespace MusikBox::DSP
