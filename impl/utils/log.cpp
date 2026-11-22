//
// Created by henry on 11/10/26.
//
#include "../../include/hagame/utils/log.h"

#include <cstdio>
#include <iostream>
#include <map>
#include <unistd.h>

using namespace hg::log;

namespace {
    struct State {
        std::mutex mutex;
        std::mutex listenerMutex; // Listeners are called one at a time, but outside mutex so they can log themselves
        std::deque<Entry> entries;
        uint64_t version = 0;
        uint64_t nextId = 1;
        std::map<int, Listener> listeners;
        int nextListener = 1;
        bool printToStdout = true;
        bool colour = isatty(fileno(stdout));
    };

    // Made on first use, so logging from another static's constructor still works
    State& state() {
        static State s;
        return s;
    }

    std::string fileName(const char* path) {
        std::string file(path);
        auto slash = file.find_last_of("/\\");
        return slash == std::string::npos ? file : file.substr(slash + 1);
    }

    void print(Entry const& entry, bool colour) {
        // Repeats only every so often, so a warning every frame doesn't flood the terminal either
        int count = entry.count;
        if (count > 1 && count != 2 && count != 10 && count % 100 != 0) {
            return;
        }
        const char* start = "";
        const char* end = colour ? "\033[0m" : "";
        if (colour) {
            start = entry.level == Level::Error ? "\033[31m" : entry.level == Level::Warning ? "\033[33m" : "";
        }
        auto& out = entry.level == Level::Info ? std::cout : std::cerr;
        out << start;
        if (entry.level != Level::Info) {
            out << levelName(entry.level) << ": ";
        }
        out << entry.message;
        if (entry.level != Level::Info) {
            out << "  (" << entry.file << ":" << entry.line << ")";
        }
        if (count > 1) {
            out << "  x" << count;
        }
        out << end << "\n";
    }
}

std::string hg::log::levelName(Level level) {
    switch (level) {
        case Level::Info: return "INFO";
        case Level::Warning: return "WARNING";
        case Level::Error: return "ERROR";
    }
    return "";
}

void hg::log::write(Level level, std::string message, std::source_location where) {
    auto& s = state();
    Entry written;
    std::vector<Listener> listeners;
    bool printIt;
    bool colour;
    {
        std::lock_guard lock(s.mutex);
        auto file = fileName(where.file_name());
        int line = (int) where.line();

        Entry* last = s.entries.empty() ? nullptr : &s.entries.back();
        if (last && last->level == level && last->line == line && last->file == file && last->message == message) {
            last->count++;
            written = *last;
        } else {
            s.entries.push_back(Entry{level, std::move(message), std::move(file), line, 1, s.nextId++});
            if (s.entries.size() > MAX_ENTRIES) {
                s.entries.pop_front();
            }
            written = s.entries.back();
        }
        s.version++;
        for (const auto& [id, listener] : s.listeners) {
            listeners.push_back(listener);
        }
        printIt = s.printToStdout;
        colour = s.colour;
    }

    // Outside the lock, so a listener can log too without deadlocking
    if (printIt) {
        print(written, colour);
    }
    if (!listeners.empty()) {
        std::lock_guard lock(s.listenerMutex);
        for (const auto& listener : listeners) {
            listener(written);
        }
    }
}

std::deque<Entry> hg::log::entries() {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    return s.entries;
}

uint64_t hg::log::version() {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    return s.version;
}

void hg::log::clear() {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    s.entries.clear();
    s.version++;
}

int hg::log::subscribe(Listener listener) {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    int id = s.nextListener++;
    s.listeners[id] = std::move(listener);
    return id;
}

void hg::log::unsubscribe(int id) {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    s.listeners.erase(id);
}

void hg::log::setPrintToStdout(bool print) {
    auto& s = state();
    std::lock_guard lock(s.mutex);
    s.printToStdout = print;
}
