#include <utility>
#include <vector>

#include <fftw3.h>
#include <audio_base.h>
#include <gnu_plot.h>

int main() {
    // Singal Preparation
    size_t N = 44100;
    size_t N_padded = 1 << 16;

    std::vector<double> input(N_padded, 0.0);
    std::vector<std::complex<double>> output(N_padded/2 + 1);

    // Signal
    float frequency = 40.0;
    for (int i = 0; i < N; ++i) {
        input[i] = 1.0 * sin(2.0 * M_PI * frequency * i / N);
    }
    
    // Hanning Window
    for (size_t i = 0; i < N; ++i) {
        input[i] *= 0.5 * (1 - cos(2 * M_PI * i / (N - 1)));
    }

    // FFTW Execution
    fftw_plan plan = fftw_plan_dft_r2c_1d(N_padded, input.data(), reinterpret_cast<fftw_complex*>(output.data()), FFTW_ESTIMATE);
    fftw_execute(plan);

    // Pair Vector for Plotting
    std::vector<std::pair<float, std::complex<double>>> pl;
    pl.resize(N_padded/2+1);

    auto resolution = static_cast<float>(N) / N_padded;
    for (size_t i = 0; i < N_padded/2 + 1; i++) {
        std::pair<float, std::complex<double>> point;
        point.first = static_cast<float>(i * resolution);
        point.second = output[i];
        pl.push_back(point);
    }
    
    // Plotting
    MusikBox::Tools::Plotter plot;
    plot.Plot1D(pl);

    fftw_destroy_plan(plan);
    return 0;
}
