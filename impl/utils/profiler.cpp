//
// Created by henry on 7/18/23.
//

#include "../../../include/hagame/utils/profiler.h"
#include "../../../include/hagame/utils/clock.h"
#include "../../../include/hagame/graphics/debug.h"
#include "../../../include/hagame/utils/file.h"

using namespace std;
using namespace std::experimental;

void hg::utils::Profiler::Start(std::string name, source_location source) {
    if (s_profiles.find(name) == s_profiles.end()) {
        Profile profile;
        profile.source = source;
        profile.name = name;
        s_profiles.insert(std::make_pair(name, profile));
        s_started.insert(std::make_pair(name, false));
    }

    if (s_started[name]) {
        return;
    }

    s_started[name] = true;

    ProfileFrame frame;
    frame.start = utils::Clock::Now();

    if (s_profiles[name].frames.size() >= MAX_PROFILER_FRAMES) {
        s_profiles[name].sum -= Clock::ToSeconds(s_profiles[name].frames[0].end - s_profiles[name].frames[0].start);
        s_profiles[name].frames.pop_front();
    }

    s_profiles[name].start = utils::Clock::Now();
    s_profiles[name].frames.push_back(frame);
}

void hg::utils::Profiler::End(std::string name, source_location source) {
    if (s_started.find(name) == s_started.end() || !s_started[name]) {
        throw std::runtime_error(name + " has not been started by profiler");
    }

    s_started[name] = false;
    s_profiles[name].end = utils::Clock::Now();
    s_profiles[name].frames[s_profiles[name].frames.size() - 1].end = utils::Clock::Now();
    s_profiles[name].sum += Clock::ToSeconds(s_profiles[name].end - s_profiles[name].start);
}

void hg::utils::Profiler::Render() {
    hg::Vec3 pos;
    for (auto &[name, profile] : s_profiles) {
        double dur = utils::Clock::ToSeconds(profile.end - profile.start) * 1000;
        hg::graphics::Debug::DrawText(pos, name + ": " + std::to_string(dur) + "ms", hg::graphics::Color::blue());
        pos[1] += 16;
    }
}

unordered_map<std::string, hg::utils::Profile> &hg::utils::Profiler::Profiles() {
    return s_profiles;
}
