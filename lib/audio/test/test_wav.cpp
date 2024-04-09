#include <iostream>
#include <fstream>

#include <wav.h>

int main() {
    std::string path = "../../../sample/sample_audio.wav";
    WAV wav(path);
    std::vector<AudioSample> data = wav.loadAudio();
    return 0;
}
