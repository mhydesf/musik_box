#include <cstdint>
#include <filesystem>

#include <wav.h>
#include <gnu_plot.h>

using namespace MusikBox;

int main(int argc, const char** argv) {
    auto path = "/home/mikhail-hyde/Documents/sources/musik_box/sample/sample_audio.wav";
    Audio::WAV wav{path};
    auto data = wav.loadAudio();

    auto mono = Audio::I_Audio::convertStereoToMono_AVG(data);

    Tools::Plotter<int16_t> plot;
    plot.Plot1D(mono);

    return 0;
}
