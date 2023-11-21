//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_PLAYER_H
#define HAGAME2_PLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <variant>
#include "../utils/loopingThread.h"
#include "../utils/uuid.h"
#include "../structures/tsqueue.h"
#include "source.h"
#include "streamBuffer.h"

namespace hg::audio {

    using source_t = utils::uuid_t;
    using buffer_t = utils::uuid_t;

    class Player : public utils::LoopingThread {
    public:

        source_t addSource(Vec3 position = Vec3::Zero());
        void updateSource(source_t source, Vec3 position, Vec3 velocity = Vec3::Zero());
        void updateSource(source_t source, float pitch, float gain);
        void updateSource(source_t source, const SourceSettings& settings);
        Source getSource(source_t source);

        buffer_t addBuffer(Stream* stream);
        void bindBuffer(buffer_t buffer, source_t source);
        void playSource(source_t source);

    protected:

        void onStart() override;
        void onTick() override;
        void onAbort() override;

    private:

        enum class EventType {
            AddSource,
            UpdateSource,
            AddBuffer,
            BindBuffer,
            PlaySource,
        };

        struct SourceEvent {
            EventType type = EventType::AddSource;
            source_t source;
            SourceSettings settings;
        };

        struct AddBufferEvent {
            EventType type = EventType::AddBuffer;
            buffer_t buffer;
            Stream* stream;
        };

        struct BindBufferEvent {
            EventType type = EventType::BindBuffer;
            source_t source;
            buffer_t buffer;
        };

        struct PlaySourceEvent {
            EventType type = EventType::PlaySource;
            source_t source;
        };

        using Event = std::variant<SourceEvent, AddBufferEvent, BindBufferEvent, PlaySourceEvent>;

        bool m_valid = false;

        ALCdevice* m_device;
        ALCcontext* m_context;

        std::unordered_map<source_t, std::unique_ptr<Source>> m_sources;
        std::unordered_map<buffer_t, std::unique_ptr<audio::StreamBuffer>> m_buffers;
        net::TSQueue<Event> m_events;

        void checkError();
    };
}

#endif //HAGAME2_PLAYER_H
