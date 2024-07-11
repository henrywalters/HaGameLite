//
// Created by henry on 10/29/23.
//

#ifndef SCIFISHOOTER_CONSOLE_H
#define SCIFISHOOTER_CONSOLE_H

#include <vector>
#include <variant>
#include <deque>

#include "../graphics/primitives/quad.h"
#include "../graphics/mesh.h"
#include "../graphics/textBuffer.h"
#include "../graphics/resolution.h"
#include "../graphics/shaderProgram.h"

namespace hg {

    // The console provides an interactive terminal. This should be placed at the game level so it can access globals.
    // Usage is simple, just create an instance and use the API to control it!

    class Console {
    public:

        enum class ArgType {
            String,
            Int,
            Float,
        };

        enum class Status {
            Open,
            Opening,
            Closing,
            Closed,
        };

        using arg_value_t = std::variant<std::string, int, float>;

        bool tryParse(ArgType type, std::string raw, arg_value_t &value);

        struct CommandArg {
            ArgType type;
            arg_value_t value;
        };

        using command_fn = std::function<int(Console*, std::vector<CommandArg>)>;

        struct Command {
            std::vector<ArgType> args;
            command_fn fn;

            Command(std::vector<ArgType> _args, command_fn _fn):
                args(_args),
                fn(_fn)
            {}

            std::string help() const;
        };

        using NamedCommand = std::tuple<std::string, Command>;

        struct Settings {
            std::string historyFile = ".history";
            std::string prefix = "> ";

            int maxOutputSize = 1000;
            float slideDur = 0.125f;
            int lineHeight = 26.0f;

            float scrollWidth = 15;

            hg::graphics::Color bgColor = hg::graphics::Color(4, 30, 135, 150);
            hg::graphics::Color scrollBarColor = hg::graphics::Color(4, 30, 120, 150);
            hg::graphics::Color scrollColor = hg::graphics::Color(4, 30, 200, 150);
            hg::graphics::Color fgColor = hg::graphics::Color::white();
        } settings;

        Console(hg::graphics::Font *font, hg::Vec2i windowSize, hg::Vec2i consoleSize);

        // Add a new command to the console
        void registerCommand(std::string command, command_fn fn, std::vector<ArgType> args = {});

        // Add a new command to the console
        void registerCommand(NamedCommand command);

        // Update the size of the console on window resize for example
        void setSize(hg::Vec2i windowSize, hg::Vec2i consoleSize);

        // Close / open the console
        void toggle();

        // Enter current command
        void submit();

        // Clear out the console buffer
        void clear();

        // Scroll up by a certain amount
        void scroll(int amt);

        // Add the latest typed character
        void newChar(char ch);

        // Update the current command
        void setCommand(const std::string &command);

        // Add a line to the buffer
        void putLine(const std::string &line);

        // Delete
        void backspace();

        void prevHistory();

        void nextHistory();

        Status status() const { return m_status; }

        void update(double dt);

        void render();

        bool isOpen() const { return m_status == Status::Opening || m_status == Status::Open; }

        std::unordered_map<std::string, Command> m_commands;

    private:

        hg::Vec2i m_windowSize;
        hg::Vec2i m_consoleSize;

        int m_historyIdx = 0;

        double m_elapsedTime;

        Status m_status = Status::Closed;
        float m_t = 0;

        std::string m_command;
        std::vector<std::string> m_buffer;

        float m_scrollHeight;

        int m_scrollIdx = 0;
        size_t m_outputStartIdx = 0;
        size_t m_outputEndIdx = 0;
        size_t m_outputSize = 0;
        std::deque<std::string> m_output;

        hg::graphics::primitives::Quad m_consoleQuad;
        hg::graphics::primitives::Quad m_cursorQuad;
        hg::graphics::primitives::Quad m_scrollBarQuad;
        hg::graphics::primitives::Quad m_scrollQuad;
        std::unique_ptr<hg::graphics::MeshInstance> m_console;
        std::unique_ptr<hg::graphics::MeshInstance> m_cursor;
        std::unique_ptr<hg::graphics::MeshInstance> m_scrollBar;
        std::unique_ptr<hg::graphics::MeshInstance> m_scroll;
        std::unique_ptr<hg::graphics::TextBuffer> m_commandBuffer;
        std::unique_ptr<hg::graphics::TextBuffer> m_historyBuffer;

        void handleScroll();

        void updateOutput();

        void updateConsoleGfx();

        void updateScrollGfx();

    };
}

#endif //SCIFISHOOTER_CONSOLE_H
