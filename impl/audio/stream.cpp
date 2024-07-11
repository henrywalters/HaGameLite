//
// Created by henry on 11/18/23.
//

#include "../../include/hagame/audio/stream.h"
#include "AudioFile.h"
#include <AL/al.h>
#include <AL/alc.h>

using namespace hg::audio;

Stream::Stream(std::string filepath)
{

    AudioFile<double> file;

    file.load(filepath);
    file.printSummary();

    m_bitDepth = file.getBitDepth();
    m_channels = file.getNumChannels();
    m_sampleRate = file.getSampleRate();
    m_samples = file.getNumSamplesPerChannel();

    if (file.isMono()) {
        m_stream.resize(m_channels * m_samples);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            m_stream[i] = static_cast<int16_t>(file.samples[0][i] * INT16_MAX);
        }
    } else {
        m_stream.resize(m_samples * m_channels);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            for (int ch = 0; ch < file.getNumChannels(); ch++) {
                m_stream[2 * i + ch] = static_cast<int16_t>(file.samples[ch][i] * INT16_MAX);
            }
        }
    }
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
