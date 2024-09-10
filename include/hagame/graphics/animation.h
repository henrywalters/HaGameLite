//
// Created by henry on 11/7/23.
//

#ifndef HAGAME2_ANIMATION_H
#define HAGAME2_ANIMATION_H

#include <deque>
#include <iostream>
#include <functional>

#include "../structures/graph.h"
#include "../utils/pubsub.h"

namespace hg::graphics {

    // Override this class to allow it to be added into the animation player
    class Animation {
    public:

        // Call this event in your override class to allow the animation player to move onwards.
        EventEmitter<Animation*> onFinish;

        virtual void play() = 0;
        virtual void update(double dt) = 0;
        virtual void stop() = 0;
        virtual void reset() = 0;

    };

    template <typename T>
    concept IsAnimation = std::is_base_of<Animation, T>::value;

    template <typename KeyType = std::string>
    class AnimationPlayer {
    public:

        ~AnimationPlayer() {
            for (const auto& [key, animation] : m_animationMap) {
                animation->onFinish.unsubscribe(m_onFinish[key]);
            }
        }

        Animation* addAnimation(KeyType key, Animation* animation) {
            m_onFinish.insert(std::make_pair(key, animation->onFinish.subscribe([&](auto anim){
                anim->reset();
                if (!m_animationQueue.empty()) {
                    auto nextInQueue = m_animationQueue[0];
                    m_animationQueue.pop_front();
                    m_animations.setCurrent(nextInQueue);
                } else {
                    auto node = m_animations.get(anim);
                    if (node->children.size() > 0) {
                        m_animations.setCurrent(node->children[0]->value);
                        trigger(node->children[0]->value);
                    }
                }
            })));
            m_animations.insert(animation);
            m_animationMap.insert(std::make_pair(key, animation));
            return animation;
        }

        // If a is connected to b, then b will play after a is played.
        void connect (Animation* a, Animation* b) {
            if (m_animations.get(a)->children.size() > 0) {
                throw std::runtime_error("Animation can only be connected to one child");
            }
            m_animations.connect(a, b);
        }

        // If a is connected to b, then b will play after a is played.
        void connect(KeyType a, KeyType b) {
            connect(m_animationMap[a], m_animationMap[b]);
        }

        // Adds this animation to the queue. It will be played after the current animation finishes
        void trigger(Animation* animation) {
            if (animation != m_animations.get()->value && std::find(m_animationQueue.begin(), m_animationQueue.end(), animation) == m_animationQueue.end()) {
                m_animationQueue.push_back(animation);
            }
        }

        // Adds this animation to the queue. It will be played after the current animation finishes
        void trigger(KeyType animation) {
            return trigger(m_animationMap[animation]);
        }

        // Skip the queue and play and animation instantly
        void triggerImmediately(Animation* animation) {
            m_animationQueue.clear();
            if (animation != m_animations.get()->value) {
                m_animations.setCurrent(animation);
                animation->reset();
                animation->play();
            }
        }

        // Skip the queue and play and animation instantly
        void triggerImmediately(KeyType key) {
            triggerImmediately(m_animationMap[key]);
        }

        void update(double dt) {
            auto animation = m_animations.get();
            if (!animation) {
                return;
            }
            animation->value->update(dt);
        }

        Animation* get() {
            auto animation = m_animations.get();
            if (!animation) {
                return nullptr;
            }

            return animation->value;
        }

    private:

        std::deque<Animation*> m_animationQueue;

        std::unordered_map<KeyType, Animation*> m_animationMap;
        structures::Graph<Animation*> m_animations;

        std::unordered_map<KeyType, std::shared_ptr<hg::EventListener<Animation*>>> m_onFinish;
    };
}

#endif //HAGAME2_ANIMATION_H
