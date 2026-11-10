//
// Created by henry on 11/18/23.
//

#include "../../include/hagame/audio/stream.h"
#include "AudioFile.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <algorithm>

#include "hagame/core/assets.h"

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

    // Float files can overshoot [-1, 1]; clamp so loud peaks clip instead of wrapping around
    auto toInt16 = [](double sample) {
        return static_cast<int16_t>(std::clamp(sample, -1.0, 1.0) * INT16_MAX);
    };

    if (file.isMono()) {
        m_stream.resize(m_channels * m_samples);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            m_stream[i] = toInt16(file.samples[0][i]);
        }
    } else {
        m_stream.resize(m_samples * m_channels);
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            for (int ch = 0; ch < file.getNumChannels(); ch++) {
                m_stream[2 * i + ch] = toInt16(file.samples[ch][i]);
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
    // Samples are always stored as 16 bit, whatever the file's bit depth
    return m_channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
}

void StreamAsset::reload() {
    m_asset = getAudioStream(m_path);
}
