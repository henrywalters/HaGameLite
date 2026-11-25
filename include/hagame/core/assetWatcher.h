//
// Created by henry on 11/22/26.
//

#ifndef HAGAME2_ASSETWATCHER_H
#define HAGAME2_ASSETWATCHER_H

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

namespace hg {

    // Watches a folder of assets and reloads each file that's changed or been added (see hg::reloadAsset), so an
    // edited texture, sheet, rig, sound, font, shader or prefab shows up in the running game.
    // Polls: call update every frame from the thread that owns the GL context, as reloading uploads textures and
    // compiles shaders. A file is reloaded once it has stopped changing for a poll, so one an editor is still writing
    // isn't read half done
    class AssetWatcher {
    public:

        // root as the asset paths are written, e.g. hg::ASSET_DIR ("../assets/"), so new files get the same names the
        // loading scene would give them
        explicit AssetWatcher(std::string root, double interval = 0.5);

        // Called after a file is reloaded, with its path and whether anything changed
        std::function<void(std::string const& path, bool changed)> onReload;

        void update(double dt);

        // Looks at every file now, without reloading anything: whatever's there counts as loaded already
        void snapshot();

    private:

        struct FileState {
            std::filesystem::file_time_type time;
            std::uintmax_t size = 0;
            bool pending = false; // Changed at the last poll: reload it if it's the same at the next
        };

        std::string m_root;
        double m_interval;
        double m_elapsed = 0.0;
        std::unordered_map<std::string, FileState> m_files;

        void poll();
    };
}

#endif //HAGAME2_ASSETWATCHER_H
