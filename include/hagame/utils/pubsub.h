//
// Created by henry on 12/28/22.
//

#ifndef HAGAME2_PUBSUB_H
#define HAGAME2_PUBSUB_H

#include <functional>
#include <memory>

#include "uuid.h"

namespace hg {

    /*
     *
     * Exposed in pubsub.h are two APIs for event-driven systems. The simpler of which is the EventEmitter / Listener.
     * These are used when there will only be one type of event with the same payload.
     *
     * The pub/sub classes allow multiple event ids to send the same payload.
     *
     */

    template <class EventType>
    struct EventListener {
        utils::UUID id;
        std::function<void(EventType)> onEvent;
    };

    template <class EventType>
    class EventEmitter {
    private:
        std::unordered_map<uint32_t, std::shared_ptr<EventListener<EventType>>> subscribers;
        uint32_t subCount = 0;
        uint32_t activeSubCount = 0;
    public:

        std::shared_ptr<EventListener<EventType>> subscribe(std::function<void(EventType)> onEventFn) {
            auto subscriber = std::make_shared<EventListener< EventType>>();
            subscriber->onEvent = onEventFn;
            subscriber->id = subCount;
            subCount++;
            activeSubCount++;
            subscribers.insert(std::make_pair(subscriber->id, subscriber));
            return subscriber;
        }

        void unsubscribe(std::shared_ptr<EventListener<EventType>> subscriber) {
            subscribers.erase(subscriber->id);
            activeSubCount--;
            subscriber.reset();
        }

        void emit(EventType e) {
            for (auto& [id, subscriber] : subscribers) {
                subscriber->onEvent(e);
            }
        }
    };

    template <class EventId, class EventType>
    struct Subscriber {
        utils::UUID id;
        EventId eventId;
        std::function<void(EventType)> onEvent;
    };

    template <class EventId, class EventType>
    class Publisher {
        std::unordered_map<uint32_t, std::shared_ptr<Subscriber<EventId, EventType>>> subscribers;
        uint32_t subCount = 0;
        uint32_t activeSubCount = 0;
    public:

        std::shared_ptr<Subscriber<EventId, EventType>> subscribe(EventId eventId, std::function<void(EventType)> onEventFn) {
            auto subscriber = std::make_shared<Subscriber<EventId, EventType>>();
            subscriber->onEvent = onEventFn;
            subscriber->eventId = eventId;
            subscriber->id = subCount;
            subCount++;
            activeSubCount++;
            subscribers.insert(std::make_pair(subscriber->id, subscriber));
            return subscriber;
        }

        void unsubscribe(std::shared_ptr<Subscriber<EventId, EventType>> subscriber) {
            subscribers.erase(subscriber->id);
            activeSubCount--;
            subscriber.reset();
        }

        void emit(EventId eventId, EventType e) {
            for (auto& [id, subscriber] : subscribers) {
                if (subscriber->eventId == eventId) {
                    subscriber->onEvent(e);
                }
            }
        }
    };
}

#endif //HAGAME2_PUBSUB_H
