#include <filesystem>

#include <wav.h>
#include <fft.h>
#include <gnu_plot.h>

using namespace MusikBox;

int main(int argc, const char** argv) {
    auto path = "/home/mikhail-hyde/Documents/sources/musik_box/sample/sample_audio.wav";
    Audio::WAV wav{path};
    auto data = wav.loadAudio();

    auto mono = Audio::I_Audio::convertStereoToMono_AVG(data);
    auto fft = DSP::FFTManager();

    std::vector<double> mono_d;
    mono_d.resize(mono.size());

    for (size_t i = 0; i < mono.size(); i++) {
        mono_d[i] = static_cast<double>(mono[i]);
    }

    std::vector<std::complex<double>> result = fft.computeFFT(mono_d);

    volatile bool stop = true;

    Tools::Plotter plot;
    plot.Plot1D(result);

    return 0;
}
