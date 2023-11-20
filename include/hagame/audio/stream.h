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

        friend class Source;

        Stream(std::string filepath);

        std::vector<SampleType> data() const;

    private:

        ALCenum getFormat() const;

        ALuint m_buffer;
        std::unique_ptr<AudioFile<double>> m_file;

    };
}

#endif //HAGAME2_STREAM_H
