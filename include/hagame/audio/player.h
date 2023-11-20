//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_PLAYER_H
#define HAGAME2_PLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include "../utils/loopingThread.h"
#include "../utils/uuid.h"
#include "../structures/tsqueue.h"
#include "source.h"

namespace hg::audio {

    using source_t = utils::uuid_t;
    using stream_t = utils::uuid_t;

    class Player : public utils::LoopingThread {
    public:

        source_t addSource(Vec3 position = Vec3::Zero());
        void updateSource(source_t source, Vec3 position, Vec3 velocity = Vec3::Zero());

        stream_t addStream(std::string file);
        void playStream(source_t source, stream_t stream);

    protected:

        void onStart() override;
        void onTick() override;
        void onAbort() override;

    private:

        enum class EventType {
            AddSource,
            UpdateSource,
            AddStream,
            PlayStream,
        };

        struct Event {
            EventType type;
            std::string file;
            Vec3 position;
            Vec3 velocity;
            source_t source;
            stream_t stream;
        };

        bool m_valid = false;

        ALCdevice* m_device;
        ALCcontext* m_context;

        std::unordered_map<source_t, std::unique_ptr<Source>> m_sources;
        std::unordered_map<stream_t, std::unique_ptr<Stream>> m_streams;
        net::TSQueue<Event> m_events;

        void checkError();
    };
}

#endif //HAGAME2_PLAYER_H
