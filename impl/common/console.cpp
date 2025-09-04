//
// Created by henry on 10/29/23.
//
#include "../../../include/hagame/common/console.h"
#include "../../../include/hagame/core/assets.h"
#include "../../../include/hagame/utils/string.h"

using namespace hg;
using namespace hg::graphics;

const std::vector<std::string> ARG_TYPE_NAMES {
    "string",
    "int",
    "float"
};

Console::Console(Font* font, hg::Vec2i windowSize, hg::Vec2i consoleSize):
        m_elapsedTime(0),
        m_windowSize(windowSize),
        m_consoleSize(consoleSize),
        m_consoleQuad(m_consoleSize.cast<float>(), Vec2((m_windowSize[0] - m_consoleSize[0]) / 2.0, -m_consoleSize[1])),
        m_cursorQuad(Vec2(10, 24)),
        m_scrollBarQuad(Vec2(settings.scrollWidth, consoleSize[1]), Vec2(m_consoleSize[0] - settings.scrollWidth, -m_consoleSize[1])),
        m_scrollQuad(Vec2(settings.scrollWidth, 20), Vec2(m_consoleSize[0] - settings.scrollWidth, -m_consoleSize[1]))
{
    m_scrollBarQuad.centered(false);
    m_scrollQuad.centered(false);
    m_consoleQuad.centered(false);
    m_console = std::make_unique<MeshInstance>(&m_consoleQuad);
    m_cursor = std::make_unique<MeshInstance>(&m_cursorQuad);
    m_scrollBar = std::make_unique<MeshInstance>(&m_scrollBarQuad);
    m_scroll = std::make_unique<MeshInstance>(&m_scrollQuad);
    m_commandBuffer = std::make_unique<TextBuffer>(font, "", Vec3(0, -50), TextHAlignment::Left);
    m_historyBuffer = std::make_unique<TextBuffer>(font, "", Vec3(0, -50), TextHAlignment::Left, TextVAlignment::Bottom);

    m_commandBuffer->text(settings.prefix);
    m_outputSize = 0;

    m_buffer = utils::f_readLines(settings.historyFile);
    m_historyIdx = m_buffer.size();

    setSize(m_windowSize, m_consoleSize);
}


void Console::submit() {

    putLine(m_command);

    std::vector<std::string> parts = utils::s_split(m_command, ' ');

    m_commandBuffer->text(settings.prefix);

    if (utils::s_trim(m_command) == "") {
        return;
    }

    m_buffer.push_back(m_command);
    m_historyIdx = m_buffer.size();
    utils::f_append(settings.historyFile, m_command + "\n");
    m_command = "";

    if (m_commands.find(parts[0]) == m_commands.end()) {
        putLine("Command not found: " + parts[0]);
        return;
    }

    Command cmd = m_commands.at(parts[0]);

    if (parts.size() - 1 != cmd.args.size()) {
        putLine("Invalid arguments. Usage: " + parts[0] + " " + cmd.help());
        return;
    }

    std::vector<CommandArg> args;

    for (int i = 0; i < cmd.args.size(); i++) {
        std::string raw = parts[i + 1];
        CommandArg arg;
        arg.type = cmd.args[i];
        if (!tryParse(arg.type, raw, arg.value)) {
            putLine("Invalid arguments. Usage: " + parts[0] + " " + cmd.help());
            return;
        }
        args.push_back(arg);
    }

    int returnCode = cmd.fn(this, args);

    if (returnCode != 0) {
        putLine("Error code: " + std::to_string(returnCode));
    }
}

void Console::newChar(char ch) {
    m_command += ch;
    m_commandBuffer->text(settings.prefix + m_command);
}

void Console::setCommand(const std::string& command) {
    m_command = command;
    m_commandBuffer->text(settings.prefix + command);
}

void Console::toggle() {
    if (m_status == Status::Open || m_status == Status::Opening) {
        m_status = Status::Closing;
    } else {
        m_status = Status::Opening;
    }
}

void Console::update(double dt) {

    if (m_status == Status::Closed) {
        return;
    } else if (m_status == Status::Open) {
        handleScroll();
        return;
    }

    if (m_status == Status::Closing) {
        m_t = math::clamp<double>((m_t - dt / settings.slideDur), 0, 1);
        if (m_t == 0) {
            m_status = Status::Closed;
        }
    }

    if (m_status == Status::Opening) {
        m_t = math::clamp<double>(m_t + dt / settings.slideDur, 0, 1);
        if (m_t == 1.0) {
            m_status = Status::Open;
        }
    }

    updateConsoleGfx();
    updateScrollGfx();

}

void Console::render() {

    Mat4 proj = Mat4::Orthographic(0, m_windowSize[0], 0, m_windowSize[1], -100, 100);

    auto shader = hg::getShader("color");
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", proj);
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("color", settings.bgColor);

    m_console->render();

    // TODO: Get cursor working
    shader->setVec4("color", settings.fgColor);
    // m_cursor->render();

    shader->setVec4("color", settings.scrollBarColor);
    m_scrollBar->render();
    shader->setVec4("color", settings.scrollColor);
    m_scroll->render();
    
    shader = hg::getShader("text_buffer");
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", proj);
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("textColor", settings.fgColor);

    m_commandBuffer->render();
    m_historyBuffer->render();

}

void Console::backspace() {
    if (m_command.length() > 0) {
        m_command.pop_back();
        m_commandBuffer->text(settings.prefix + m_command);
    }
}

void Console::registerCommand(std::string command, Console::command_fn fn, std::vector<ArgType> args) {
    m_commands.insert(std::make_pair(command, Command{args, fn}));
}

void Console::registerCommand(Console::NamedCommand command) {
    m_commands.insert(std::make_pair(std::get<0>(command), std::get<1>(command)));
}

void Console::handleScroll() {

}

void Console::putLine(const std::string& line) {

    m_output.push_back(line);
    if (m_output.size() >= settings.maxOutputSize) {
        m_output.pop_front();
    }

    m_outputStartIdx = m_output.size() - std::min<size_t>(m_output.size(), m_outputSize);
    m_outputEndIdx = m_output.size();

    float scrollPercent = m_output.size() < m_outputSize ? 1.0 : (float) m_outputSize / m_output.size();

    m_scrollHeight = m_consoleSize[1] * scrollPercent;

    updateScrollGfx();

    updateOutput();

}

void Console::updateOutput() {
    std::string output = "";

    for (int i = m_outputStartIdx; i < m_outputEndIdx; i++) {
        output += (i > m_outputStartIdx ? "\n" : "") + m_output[i - m_scrollIdx];
    }

    m_historyBuffer->text(output);

}

bool Console::tryParse(ArgType type, std::string raw, Console::arg_value_t &value) {
    if (type == ArgType::String) {
        value = raw;
        return true;
    }

    try {
        if (type == ArgType::Int) {
            value = std::stoi(raw);
        }
        if (type == ArgType::Float) {
            value = std::stof(raw);
        }
        return true;
    } catch (...) {
        return false;
    }
}

void Console::prevHistory() {

    if (m_buffer.size() == 0 || m_historyIdx == 0) {
        return;
    }

    m_historyIdx--;

    setCommand(m_buffer[m_historyIdx]);
}

void Console::nextHistory() {

    if (m_buffer.size() == 0) {
        return;
    }

    m_historyIdx++;

    if (m_historyIdx >= m_buffer.size()) {
        m_historyIdx = m_buffer.size();
        setCommand("");
        return;
    }

    setCommand(m_buffer[m_historyIdx]);
}

void Console::setSize(hg::Vec2i windowSize, hg::Vec2i consoleSize) {
    m_outputSize = consoleSize.y() / settings.lineHeight - 1;
    m_consoleSize = consoleSize;
    m_windowSize = windowSize;
    updateConsoleGfx();
    updateScrollGfx();
}

void Console::updateConsoleGfx() {

    auto font = m_historyBuffer->font();

    m_consoleQuad.offset(Vec2((m_windowSize[0] - m_consoleSize[0]) / 2.0, consolePosY()));
    m_console->update(&m_consoleQuad);

    float cursorOffset = font->calcMessageSize(m_command)[0];
    m_cursorQuad.offset(Vec2((m_windowSize[0] - m_consoleSize[0]) / 2.0 + cursorOffset, consolePosY()));
    m_cursor->update(&m_cursorQuad);

    m_commandBuffer->size(Vec3(m_consoleSize[0], settings.lineHeight, 0));
    m_commandBuffer->pos(Vec3((m_windowSize[0] - m_consoleSize[0]) / 2.0, consolePosY(), 0));

    // float historySize = font->calcMessageSize(m_historyBuffer->text())[1];
    m_historyBuffer->size(Vec3(m_consoleSize[0], m_consoleSize[1] - settings.lineHeight, 0));
    m_historyBuffer->pos(Vec3(0, consolePosY() + settings.lineHeight, 0));
}

void Console::updateScrollGfx() {
    m_scrollBarQuad.offset(Vec2((m_consoleSize[0] - settings.scrollWidth), consolePosY()));
    m_scrollBar->update(&m_scrollBarQuad);

    m_scrollQuad.size(Vec2(settings.scrollWidth, m_scrollHeight));

    float scrollPos = m_output.size() < m_outputSize ? 0. : (float) m_scrollIdx / m_output.size() * m_consoleSize[1];

    m_scrollQuad.offset(Vec2((m_consoleSize[0] - settings.scrollWidth), consolePosY() + scrollPos));
    m_scroll->update(&m_scrollQuad);
}

void Console::scroll(int amt) {
    if (amt == 0) return;
    m_scrollIdx += amt;
    m_scrollIdx = math::clamp<int>(m_scrollIdx, 0, std::max<int>(0, m_output.size() - m_outputSize));
    updateScrollGfx();
    updateOutput();
}

void Console::clear() {
    m_scrollIdx = 0;
    m_outputStartIdx = 0;
    m_outputEndIdx = 0;
    m_scrollHeight = m_consoleSize[1];
    m_output.clear();
    m_buffer.clear();
    updateOutput();
    updateScrollGfx();
}

float Console::consolePosY() const {
    return m_consoleSize[1] * (m_t - 1.0);
}


std::string Console::Command::help() const {
    std::string params;
    for (const auto& arg : args) {
        params += "[" + ARG_TYPE_NAMES[(int)arg] + "] ";
    }
    return params;
}
