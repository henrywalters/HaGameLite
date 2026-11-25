//
// Created by henry on 11/22/26.
//
#include "../../include/hagame/core/assetWatcher.h"
#include "../../include/hagame/core/assets.h"
#include "../../include/hagame/utils/log.h"

namespace fs = std::filesystem;

namespace {
    // Scripts are read when they run, and scenes belong to the editor
    bool ignored(fs::path const& path) {
        auto ext = path.extension().string();
        return ext == ".lua" || ext == ".hg_scene" || ext.empty() || ext.back() == '~' ||
               path.filename().string().starts_with(".");
    }

    // Visits every file under root, as root + its relative path ("../assets/textures/crate.png")
    template <class Fn>
    void eachFile(std::string const& root, Fn fn) {
        std::error_code error;
        fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, error);
        for (; !error && it != fs::recursive_directory_iterator(); it.increment(error)) {
            std::error_code fileError;
            if (!it->is_regular_file(fileError) || ignored(it->path())) {
                continue;
            }
            auto time = it->last_write_time(fileError);
            auto size = it->file_size(fileError);
            if (fileError) {
                continue; // Gone, or being replaced, between listing it and looking at it
            }
            auto relative = fs::relative(it->path(), root, fileError).generic_string();
            fn(root + relative, time, size);
        }
    }
}

hg::AssetWatcher::AssetWatcher(std::string root, double interval):
    m_root(std::move(root)),
    m_interval(interval)
{
    if (!m_root.empty() && m_root.back() != '/') {
        m_root += '/';
    }
    snapshot();
}

void hg::AssetWatcher::snapshot() {
    m_files.clear();
    eachFile(m_root, [&](std::string const& path, fs::file_time_type time, std::uintmax_t size) {
        m_files[path] = FileState{time, size, false};
    });
}

void hg::AssetWatcher::update(double dt) {
    m_elapsed += dt;
    if (m_elapsed < m_interval) {
        return;
    }
    m_elapsed = 0.0;
    poll();
}

void hg::AssetWatcher::poll() {
    std::vector<std::string> ready;

    eachFile(m_root, [&](std::string const& path, fs::file_time_type time, std::uintmax_t size) {
        auto it = m_files.find(path);
        if (it == m_files.end()) {
            m_files[path] = FileState{time, size, true};
            return;
        }
        auto& state = it->second;
        if (state.time != time || state.size != size) {
            state.time = time;
            state.size = size;
            state.pending = true;
        } else if (state.pending) {
            state.pending = false;
            ready.push_back(path);
        }
    });

    // Textures before the sheets, atlases and prefabs that use them
    std::stable_sort(ready.begin(), ready.end(), [](auto const& a, auto const& b) {
        return a.ends_with(".png") && !b.ends_with(".png");
    });

    for (auto const& path : ready) {
        bool changed = reloadAsset(path);
        if (changed) {
            log::info("Reloaded {}", path.substr(m_root.size()));
        }
        if (onReload) {
            onReload(path, changed);
        }
    }
}
