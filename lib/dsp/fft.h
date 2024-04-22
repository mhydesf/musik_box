#pragma once

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

        for (size_t i = 0; i < inputSize; ++i) {
            in[i] = input[i];
        }

        fftw_execute(plan);

        std::vector<std::complex<double>> result(inputSize / 2 + 1);
        for (size_t i = 0; i < inputSize / 2 + 1; ++i) {
            result[i] = std::complex<double>(out[i][0], out[i][1]);
        }

        return result;
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

private:
    double* in;
    fftw_complex* out;
    fftw_plan plan;
    size_t N;
};

} // namespace MusikBox::DSP
