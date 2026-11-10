//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_STREAM_H
#define HAGAME2_STREAM_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "hagame/core/asset.h"

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
        // Bit depth of the source file. Samples are always converted to 16 bit on load
        int bitDepth() const { return m_bitDepth; }
        int samples() const { return m_samples; }
        int getFormat() const;
        size_t bytes() const { return m_stream.size() * sizeof(SampleType); }

    private:

        int m_channels;
        int m_sampleRate;
        int m_bitDepth;
        int m_samples;
        std::vector<SampleType> m_stream;

    };

    class StreamAsset : public Asset<Stream> {
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_STREAM_H
