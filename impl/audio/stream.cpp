//
// Created by henry on 11/18/23.
//

#include "../../include/hagame/audio/stream.h"

using namespace hg::audio;

hg::audio::Stream::Stream(std::string filepath):
    m_file(std::make_unique<AudioFile<double>>())
{
    m_file->load(filepath);
    m_file->printSummary();

    alGenBuffers(1, &m_buffer);

    std::vector<uint16_t> data;

    if (m_file->isMono()) {
        data.resize(m_file->getNumSamplesPerChannel());
        for (int i = 0; i < m_file->getNumSamplesPerChannel(); i++) {
            data[i] = static_cast<int16_t>(m_file->samples[0][i] * INT16_MAX);
        }
    } else {
        data.resize(m_file->getNumSamplesPerChannel() * m_file->getNumChannels());
        for (int i = 0; i < m_file->getNumSamplesPerChannel(); i++) {
            for (int ch = 0; ch < m_file->getNumChannels(); ch++) {
                data[2 * i + ch] = static_cast<int16_t>(m_file->samples[ch][i] * INT16_MAX);
            }
        }
    }

    ALenum format = m_file->isMono() ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    ALuint size = m_file->getNumChannels() * m_file->getNumSamplesPerChannel() * (m_file->getBitDepth() / 8);
    alBufferData(
            m_buffer,
            format,
            data.data(),
            size,
            m_file->getSampleRate()
    );

    ALCenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cout << "OpenAL ERROR: " << error << "\n";
    }
}

ALCenum Stream::getFormat() const {
    int bits = m_file->getBitDepth() / m_file->getNumChannels();
    if (m_file->isMono() && bits == 8) {
        return AL_FORMAT_MONO8;
    } else if (m_file->isMono() && bits == 16) {
        return AL_FORMAT_MONO16;
    } else if (m_file->isStereo() && bits == 8) {
        return AL_FORMAT_STEREO8;
    } else {
        return AL_FORMAT_STEREO16;
    }
}
