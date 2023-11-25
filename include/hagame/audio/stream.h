//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_STREAM_H
#define HAGAME2_STREAM_H

#include <memory>
#include <string>
#include "AudioFile.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace hg::audio {

    using SampleType = uint16_t;

    class Stream {
    public:

        friend class StreamBuffer;

        Stream() {}
        Stream(std::string filepath);

        SampleType* stream();
        SampleType& at(int channel, int idx);

        int channels() const { return m_channels; }
        int sampleRate() const { return m_sampleRate; }
        int bitDepth() const { return m_bitDepth; }
        int samples() const { return m_samples; }
        ALCenum getFormat() const;
        size_t bytes() const { return m_channels * m_samples * (m_bitDepth / 8); }

    private:

        int m_channels;
        int m_sampleRate;
        int m_bitDepth;
        int m_samples;
        std::vector<SampleType> m_stream;

    };
}

#endif //HAGAME2_STREAM_H
