//
// Created by henry on 11/18/23.
//

#include "../../include/hagame/audio/stream.h"

using namespace hg::audio;

Stream Stream::FromFile(std::string filepath)
{
    Stream stream;

    AudioFile<double> file;

    file.load(filepath);
    file.printSummary();

    stream.m_bitDepth = file.getBitDepth();
    stream.m_channels = file.getNumChannels();
    stream.m_sampleRate = file.getSampleRate();
    stream.m_samples = file.getNumSamplesPerChannel();

    if (file.isMono()) {
        stream.m_stream.resize(stream.m_channels);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            stream.m_stream[i] = static_cast<int16_t>(file.samples[0][i] * INT16_MAX);
        }
    } else {
        stream.m_stream.resize(stream.m_samples * stream.m_channels);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            for (int ch = 0; ch < file.getNumChannels(); ch++) {
                stream.m_stream[2 * i + ch] = static_cast<int16_t>(file.samples[ch][i] * INT16_MAX);
            }
        }
    }

    return stream;
}

SampleType *Stream::stream() {
    return m_stream.data();
}

SampleType &Stream::at(int channel, int idx) {
    return m_stream[2 * idx + channel];
}

ALCenum Stream::getFormat() const {
    int bits = m_bitDepth / m_channels;
    bool isMono = m_channels == 1;

    return isMono ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    if (isMono && bits == 8) {
        return AL_FORMAT_MONO8;
    } else if (isMono && bits == 16) {
        return AL_FORMAT_MONO16;
    } else if (!isMono && bits == 8) {
        return AL_FORMAT_STEREO8;
    } else {
        return AL_FORMAT_STEREO16;
    }
}
