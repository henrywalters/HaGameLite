//
// Created by henry on 11/10/26.
//

#ifndef HAGAME2_LOG_H
#define HAGAME2_LOG_H

#include <cstdint>
#include <deque>
#include <format>
#include <functional>
#include <mutex>
#include <source_location>
#include <string>

// The engine's log, like Godot's print / push_warning / push_error. Every message records where it came from, a
// message repeated straight after itself is counted rather than added again (so something warning every frame is one
// line, not thousands), and it goes to stdout and to whatever listens: the editor's Output panel, the game's console.
//
//   hg::log::info("Loaded {}", path);
//   hg::log::warn("AudioSource on '{}' has no sound set", entity->name);
//   hg::log::error("Prefab {} isn't loaded", id);
//
// Safe to call from any thread, e.g. the audio thread
namespace hg::log {

    enum class Level {
        Info,
        Warning,
        Error,
    };

    struct Entry {
        Level level;
        std::string message;
        std::string file; // Just the file's name, not its path
        int line = 0;
        int count = 1; // How many times in a row it's been logged
        uint64_t id = 0; // Increases with every new entry, so a view can tell what it's already seen
    };

    // Adds an entry. Usually reached through info / warn / error, which fill in where it came from
    void write(Level level, std::string message, std::source_location where);

    // Copies of the entries kept, oldest first. Only the most recent MAX_ENTRIES are kept
    std::deque<Entry> entries();
    constexpr size_t MAX_ENTRIES = 2000;

    // Changes every time an entry is added, counted or cleared, so a view only copies entries() when there's news
    uint64_t version();

    void clear();

    // Called for every write, a repeat included (its count is the new total). Returns an id to unsubscribe with.
    // Listeners are called one at a time, but on whichever thread logged, so a listener mustn't touch graphics or
    // the scene. To show entries in UI, poll entries() on the main thread when version() changes instead
    using Listener = std::function<void(Entry const&)>;
    int subscribe(Listener listener);
    void unsubscribe(int id);

    // Whether stdout gets every entry. On by default; the colours are ANSI, so off when stdout isn't a terminal
    void setPrintToStdout(bool print);

    // The message with its format arguments, and where it was logged from. The location has to be a default argument
    // of a constructor for it to be the caller's, which is why the format string is wrapped in this
    template <typename... Args>
    struct Format {
        std::format_string<Args...> format;
        std::source_location where;

        template <typename T>
        consteval Format(T const& string, std::source_location location = std::source_location::current()):
            format(string),
            where(location)
        {}
    };

    template <typename... Args>
    void info(Format<std::type_identity_t<Args>...> format, Args&&... args) {
        write(Level::Info, std::format(format.format, std::forward<Args>(args)...), format.where);
    }

    template <typename... Args>
    void warn(Format<std::type_identity_t<Args>...> format, Args&&... args) {
        write(Level::Warning, std::format(format.format, std::forward<Args>(args)...), format.where);
    }

    template <typename... Args>
    void error(Format<std::type_identity_t<Args>...> format, Args&&... args) {
        write(Level::Error, std::format(format.format, std::forward<Args>(args)...), format.where);
    }

    std::string levelName(Level level);
}

#endif //HAGAME2_LOG_H
