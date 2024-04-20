#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl/filesystem.h>

#include <audio_base.h>
#include <wav.h>

PYBIND11_MODULE(musik_box, m) {
    pybind11::enum_<AudioType>(m, "AudioType")
        .value("MONO", AudioType::MONO)
        .value("STEREO", AudioType::STEREO)
        .export_values();
    
    pybind11::enum_<AudioFormat>(m, "AudioFormat")
        .value("BIT_8", AudioFormat::BIT_8)
        .value("BIT_16", AudioFormat::BIT_16)
        .value("BIT_24", AudioFormat::BIT_24)
        .value("BIT_32", AudioFormat::BIT_32)
        .export_values();

    pybind11::class_<AudioSample>(m, "AudioSample")
        .def(pybind11::init<int16_t, int16_t>())
        .def_readwrite("left", &AudioSample::left)
        .def_readwrite("right", &AudioSample::right);
    
    pybind11::class_<WAVHeader>(m, "WAVHeader")
        .def(pybind11::init<
             uint32_t,
             uint32_t,
             uint32_t,
             uint32_t,
             uint32_t,
             uint16_t,
             uint16_t,
             uint32_t,
             uint32_t,
             uint16_t,
             uint16_t,
             uint32_t,
             uint16_t>())
        .def_readwrite("riff", &WAVHeader::riff)
        .def_readwrite("chunkSize", &WAVHeader::chunkSize)
        .def_readwrite("format", &WAVHeader::format)
        .def_readwrite("subChunk1ID", &WAVHeader::subChunk1ID)
        .def_readwrite("subChunk1Size", &WAVHeader::subChunk1Size)
        .def_readwrite("audioFormat", &WAVHeader::audioFormat)
        .def_readwrite("numChannels", &WAVHeader::numChannels)
        .def_readwrite("sampleRate", &WAVHeader::sampleRate)
        .def_readwrite("byteRate", &WAVHeader::byteRate)
        .def_readwrite("blockAlign", &WAVHeader::blockAlign)
        .def_readwrite("bitsPerSample", &WAVHeader::bitsPerSample)
        .def_readwrite("data", &WAVHeader::data)
        .def_readwrite("dataSize", &WAVHeader::dataSize);

    pybind11::class_<WAV>(m, "WAV")
        .def(pybind11::init<std::filesystem::path>())
        .def("loadAudio", &WAV::loadAudio)
        .def("getSampleRate", &WAV::getSampleRate);
}
