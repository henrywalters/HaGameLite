//
// Created by henry on 7/18/23.
//

#ifndef HAGAME2_PROFILER_H
#define HAGAME2_PROFILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <experimental/source_location>
#include <stdexcept>

namespace hg::utils {

    struct ProfileFrame {
        long long start;
        long long end;
    };

    struct Profile {
        std::string name;
        std::experimental::source_location source;
        std::vector<ProfileFrame> frames;
        long long start, end;
    };

    class Profiler {
    public:

        static std::string Name(std::experimental::source_location source);

        static bool ENABLED;

        static void Start(std::string name = Name(std::experimental::source_location::current()), std::experimental::source_location source = std::experimental::source_location::current());
        static void End(std::string name = Name(std::experimental::source_location::current()), std::experimental::source_location source = std::experimental::source_location::current());

        static void Render();

    private:

        static inline std::unordered_map<std::string, Profile> s_profiles;
        static inline std::unordered_map<std::string, bool> s_started;

    };
}

#endif //HAGAME2_PROFILER_H
