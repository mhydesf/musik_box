#include <wav.h>
#include <filesystem>

int main(int argc, const char** argv) {
    auto path1 = "/home/mikhail-hyde/Documents/sources/musik_box/sample/sample_audio.wav";
    auto path2 = "/home/mikhail-hyde/Documents/sources/musik_box/sample/M4_major.wav";
    WAV wav1{path1};
    WAV wav2{path2};
    auto data1 = wav1.loadAudio();
    auto data2 = wav1.loadAudio();

    return 0;
}
