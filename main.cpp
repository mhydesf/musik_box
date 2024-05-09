#include <filesystem>

#include <wav.h>
#include <fft.h>
#include <gnu_plot.h>

using namespace MusikBox;

int main(int argc, const char** argv) {
    // load + parse wav file
    auto path = std::filesystem::absolute("../sample/sample_audio.wav");
    Audio::WAV wav{path};
    auto data = wav.loadAudio();
    auto mono = Audio::WAV::convertStereoToMono_AVG(data);

    // convert to std::vector<double>
    std::vector<double> mono_d = Audio::WAV::convertAudioData<int16_t, double>(mono);

    // run fft + reduce size
    auto fft = DSP::FFTManager();
    std::vector<std::complex<double>> result = fft.computeFFT(mono_d);

    // prepare vector for plotting
    const auto pl = fft.convertToPairVector(result, wav.getSampleRate());

    // plot
    Tools::Plotter plot;
    plot.SetBounds(0, 10'000, std::nullopt, std::nullopt);
    plot.Plot1D(pl);

    return 0;
}
