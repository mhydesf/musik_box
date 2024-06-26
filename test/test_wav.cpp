#include <iostream>
#include <wav.h>

using namespace MusikBox;

int main() {
    try {
        Audio::WAV wav{"../../../../sample/sample_audio.wav"};
        std::cout << "Successfully generated WAV obj" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception raised " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unexpected exception raised " << std::endl;
        return -2;
    }
    
    try {
        Audio::WAV wav{"../../../sample/fake.wav"};
        return -1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Appropriate exception raised " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected exception raised " << std::endl;
        return -2;
    }

    return 0;
}
