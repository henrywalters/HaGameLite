//
// Created by henry on 7/18/23.
//

#ifndef HAGAME2_PROFILER_H
#define HAGAME2_PROFILER_H

#ifndef __EMSCRIPTEN__
    #define USE_EXPERIMENTAL 1;
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#ifdef USE_EXPERIMENTAL
#include <experimental/source_location>
#include "file.h"

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
    };

    struct Profile {
        std::string name;
        source_t source;
        std::vector<ProfileFrame> frames;
        long long start, end;
    };

    class Profiler {
    public:

        static bool ENABLED;

        static void Start(std::string name = name_source(default_source()), source_t source = default_source());
        static void End(std::string name = name_source(default_source()), source_t source = default_source());

        static void Render();

    private:

        static inline std::unordered_map<std::string, Profile> s_profiles;
        static inline std::unordered_map<std::string, bool> s_started;

    };
}

#endif //HAGAME2_PROFILER_H
