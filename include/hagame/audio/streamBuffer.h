//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_STREAMBUFFER_H
#define HAGAME2_STREAMBUFFER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdint>
#include "stream.h"

namespace hg::audio {
    class StreamBuffer {
    public:

        StreamBuffer();

        void load(ALenum format, int size, uint16_t* data, int sampleRate);
        void load(Stream* stream);

        ALuint id() const { return m_buffer; }

    private:

        ALuint m_buffer;

    };
}

#endif //HAGAME2_STREAMBUFFER_H
