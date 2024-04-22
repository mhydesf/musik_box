#include <wav.h>
#include <filesystem>

int main(int argc, const char** argv) {
    auto path = "/home/mikhail-hyde/Documents/sources/musik_box/sample/sample_audio.wav";
    WAV wav{path};
    auto data = wav.loadAudio();

    return 0;
}
