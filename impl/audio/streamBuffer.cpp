//
// Created by henry on 11/20/23.
//

#include "../../../include/hagame/audio/streamBuffer.h"

hg::audio::StreamBuffer::StreamBuffer() {
    alGenBuffers(1, &m_buffer);
}

void hg::audio::StreamBuffer::load(ALenum format, int size, uint16_t *data, int sampleRate) {

    alBufferData(
        m_buffer,
        format,
        data,
        size,
        sampleRate
    );
}

void hg::audio::StreamBuffer::load(hg::audio::Stream *stream) {
    load(stream->getFormat(), stream->bytes(), stream->m_stream.data(), stream->m_sampleRate);
}
