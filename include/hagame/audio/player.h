//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_PLAYER_H
#define HAGAME2_PLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <variant>
#include <mutex>
#include <optional>
#include <unordered_map>
#include "../utils/loopingThread.h"
#include "../utils/uuid.h"
#include "../structures/tsqueue.h"
#include "source.h"
#include "streamBuffer.h"

namespace hg::audio {

    using source_t = utils::uuid_t;
    using buffer_t = utils::uuid_t;

    struct ListenerSettings {
        Vec3 position;
        Vec3 velocity;
        float gain = 1.0f;

        bool operator==(const ListenerSettings&) const = default;
    };

    // Player owns an OpenAL device and context on its own thread. Everything that touches OpenAL happens on that
    // thread: the public methods only queue events, and source states are read back from a cache
    class Player : public utils::LoopingThread {
    public:

        ~Player();

        source_t addSource(Vec3 position = Vec3::Zero());
        // Replace a source's settings. The bound buffer is left alone; use bindBuffer to change it
        void updateSource(source_t source, const SourceSettings& settings);
        void removeSource(source_t source);

        // State as of the player thread's last tick, or nullopt if the source hasn't been created yet
        std::optional<SourceState> getState(source_t source);

        void playSource(source_t source);
        void pauseSource(source_t source);
        void stopSource(source_t source);

        buffer_t addBuffer(Stream* stream);
        void bindBuffer(buffer_t buffer, source_t source);

        void updateListener(const ListenerSettings& settings);

    protected:

        void onStart() override;
        void onTick() override;
        void onEnd() override;

    private:

        enum class EventType {
            AddSource,
            UpdateSource,
            AddBuffer,
            BindBuffer,
            ChangeState,
            RemoveSource,
            UpdateListener,
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

        struct ChangeStateEvent {
            EventType type = EventType::ChangeState;
            SourceState state;
            source_t source;
        };

        struct RemoveSourceEvent {
            EventType type = EventType::RemoveSource;
            source_t source;
        };

        struct ListenerEvent {
            EventType type = EventType::UpdateListener;
            ListenerSettings settings;
        };

        using Event = std::variant<SourceEvent, AddBufferEvent, BindBufferEvent, ChangeStateEvent, RemoveSourceEvent, ListenerEvent>;

        bool m_valid = false;

        ALCdevice* m_device = nullptr;
        ALCcontext* m_context = nullptr;

        std::unordered_map<source_t, std::unique_ptr<Source>> m_sources;
        std::unordered_map<buffer_t, std::unique_ptr<audio::StreamBuffer>> m_buffers;
        structures::TSQueue<Event> m_events;

        std::mutex m_stateMutex;
        std::unordered_map<source_t, SourceState> m_states;

        void checkError();

        void changeState(source_t source, SourceState state);
    };
}

#endif //HAGAME2_PLAYER_H
