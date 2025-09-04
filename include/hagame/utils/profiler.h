//
// Created by henry on 7/18/23.
//

#ifndef HAGAME2_PROFILER_H
#define HAGAME2_PROFILER_H

#ifdef __UNIX__
    #define USE_EXPERIMENTAL 1;
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <deque>
#include <functional>
#include "file.h"
#include "clock.h"

#ifdef USE_EXPERIMENTAL
#include <experimental/source_location>
#endif

namespace hg::utils {

#ifdef USE_EXPERIMENTAL
    using source_t = std::experimental::source_location;

    inline std::string name_source(source_t source) {
        utils::FileParts parts = utils::f_getParts(source.file_name());
        return parts.name + ": " + std::string(source.function_name());
    }

    inline constexpr source_t default_source() {
        return std::experimental::source_location::current();
    }
#else

    using source_t = std::string;

    inline std::string name_source(source_t source) {
        return source;
    }

    inline source_t default_source() {
        return "SOURCE NOT SUPPORTED";
    }

#endif

    struct ProfileFrame {
        long long start;
        long long end;

        long long duration() const {
            return end - start;
        }

        double durationInSeconds() const {
            return hg::utils::Clock::ToSeconds(duration());
        }
    };

    const int MAX_PROFILER_FRAMES = 100;

    class Profiler {
    public:

        struct Profile {
            std::string name;
            source_t source;
            std::deque<ProfileFrame> frames;
            long long sum = 0;

            long long average() const {
                return sum / frames.size();
            }

            double averageInSeconds() const {
                return hg::utils::Clock::ToSeconds(average());
            }
        };

        static bool ENABLED;

        static void Start(std::string name = name_source(default_source()), source_t source = default_source());
        static void End(std::string name = name_source(default_source()), source_t source = default_source());

        static void Render();

        static std::unordered_map<std::string, Profile>& Profiles();

    private:

        static inline std::unordered_map<std::string, Profile> s_profiles;
        static inline std::unordered_map<std::string, bool> s_started;

    };


    template <typename Fn, typename... Args>
    void Profile(std::string name, Fn&& fn, Args &&... args) {
        Profiler::Start(name);
        if constexpr (std::is_member_function_pointer_v<Fn>) {
            // Member function pointer requires an instance (first arg)
            std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
        } else {
            // Non-member function or callable
            std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
        }
        Profiler::End(name);
    }
}

#endif //HAGAME2_PROFILER_H
