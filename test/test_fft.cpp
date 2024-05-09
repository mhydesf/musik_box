#include <optional>
#include <vector>
#include <fft.h>
#include <wav.h>
#include <gnu_plot.h>

int main() {
    // Signal
    const uint32_t sample_rate = 44100;
    float frequency = 40.0;
    std::vector<float> wave;
    wave.resize(sample_rate);
    for (int i = 0; i < sample_rate; ++i) {
        wave[i] = 1.0 * sin(2.0 * M_PI * frequency * i / sample_rate);
    }

    // FFT Test
    auto wave_d = MusikBox::Audio::WAV::convertAudioData<float, double>(wave);
    auto fft = MusikBox::DSP::FFTManager();
    auto result = fft.computeFFT(wave_d);
    auto pl = fft.convertToPairVector(result, sample_rate, 100);

    // Plot
    MusikBox::Tools::Plotter plot;
    // plot.SetBounds(0, 100, std::nullopt, std::nullopt);
    plot.Plot1D(pl);

    return 0;
}
