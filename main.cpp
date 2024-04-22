#include <cstdint>
#include <filesystem>

#include <wav.h>
#include <gnu_plot.h>

int main(int argc, const char** argv) {
    auto path1 = "/home/mikhail-hyde/Documents/sources/musik_box/sample/sample_audio.wav";
    auto path2 = "/home/mikhail-hyde/Documents/sources/musik_box/sample/M4_major.wav";
    WAV wav1{path1};
    WAV wav2{path2};
    auto data1 = wav1.loadAudio();
    auto data2 = wav2.loadAudio();

    auto mono1 = wav1.convertStereoToMono(data1);
    auto mono2 = wav2.convertStereoToMono(data2);

    Plotter<int16_t> plot;

    plot.Plot(mono1);

    return 0;
}
