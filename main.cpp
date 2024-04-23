#include <filesystem>

#include <wav.h>
#include <fft.h>
#include <gnu_plot.h>

using namespace MusikBox;

int main(int argc, const char** argv) {
    // load + parse wav file
    auto path = "/home/mikhail-hyde/Documents/sources/musik_box/sample/M4_major.wav";
    Audio::WAV wav{path};
    auto data = wav.loadAudio();
    auto mono = Audio::WAV::convertStereoToMono_AVG(data);

    // convert to std::vector<double>
    std::vector<double> mono_d = Audio::WAV::convertAudioData<int16_t, double>(mono);

    // run fft + reduce size
    auto fft = DSP::FFTManager();
    std::vector<std::complex<double>> result = fft.computeFFT(mono_d);
    fft.reduceFFT(result, wav.getSampleRate(), 20'000);

    // prepare vector for plotting
    std::vector<std::pair<float, std::complex<double>>> pl(result.size());
    for (size_t i = 0; i < result.size(); i++) {
        float x = i * fft.getFreqencyStep(wav.getSampleRate(), result.size());
        std::complex<double> y = result[i];
        pl[i] = std::pair(x, y);
    }

    // plot
    Tools::Plotter plot;
    plot.Plot1D(pl);

    return 0;
}
