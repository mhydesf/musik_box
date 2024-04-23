#include <vector>
#include <fft.h>
#include <wav.h>
#include <gnu_plot.h>

std::vector<float> generateSinWave(float amplitude,
                                   float frequency,
                                   uint32_t sample_rate,
                                   float duration) {
    std::vector<float> ret;
    int totalSamples = static_cast<int>(sample_rate * duration);
    for (int n = 0; n < totalSamples; ++n) {
        double value = amplitude * sin(n * 2 * M_PI * frequency / sample_rate);
        ret.push_back(value);
    }

    return ret;
}

int main() {
    const uint32_t sample_rate = 44100;
    const float freq = 10.0;
    auto wave = generateSinWave(1.0, freq, sample_rate, 1000.0);
    auto wave_d = MusikBox::Audio::WAV::convertAudioData<float, double>(wave);

    auto fft = MusikBox::DSP::FFTManager();
    auto result = fft.computeFFT(wave_d);
    fft.reduceFFT(result, sample_rate, 10*freq);

    std::vector<std::pair<float, std::complex<double>>> pl(result.size());
    for (size_t i = 0; i < result.size(); i++) {
        float x = i * fft.getFreqencyStep(sample_rate, result.size());
        std::complex<double> y = result[i];
        pl[i] = std::pair(x, y);
    }
    
    MusikBox::Tools::Plotter plot;
    plot.Plot1D(pl);

    return 0;
}
