#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <wav.h>

PYBIND11_MODULE(musik_box, m) {
    pybind11::enum_<AudioType>(m, "AudioType")
        .value("MONO", AudioType::MONO)
        .value("STEREO", AudioType::STEREO)
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
        .def("loadFromFile", &WAVHeader::loadFromFile)
        .def_readwrite("riff", &WAVHeader::riff)
        .def_readwrite("fileSize", &WAVHeader::fileSize)
        .def_readwrite("fileHeader", &WAVHeader::fileHeader)
        .def_readwrite("format", &WAVHeader::format)
        .def_readwrite("dataLength", &WAVHeader::dataLength)
        .def_readwrite("formatType", &WAVHeader::formatType)
        .def_readwrite("numberChannels", &WAVHeader::numberChannels)
        .def_readwrite("sampleRate", &WAVHeader::sampleRate)
        .def_readwrite("byteRate", &WAVHeader::byteRate)
        .def_readwrite("blockAlign", &WAVHeader::blockAlign)
        .def_readwrite("bitsPerSample", &WAVHeader::bitsPerSample)
        .def_readwrite("data", &WAVHeader::data)
        .def_readwrite("dataSize", &WAVHeader::dataSize);

    pybind11::class_<WAV>(m, "WAV")
        .def(pybind11::init<std::filesystem::path>())
        .def("loadAudio", &WAV::loadAudio);
}
