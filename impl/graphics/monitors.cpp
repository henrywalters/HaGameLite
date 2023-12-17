//
// Created by henry on 12/16/23.
//
#include <iostream>
#include "../../../include/hagame/graphics/monitors.h"

using namespace hg::graphics;

std::vector<Monitor> Monitors::All() {
    std::vector<Monitor> out;

    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::cout << "MONITOR COUNT = " << count << "\n";

    for (int i = 0; i < count; i++) {
        Monitor monitor;
        monitor.monitor = monitors[i];
        monitor.name = glfwGetMonitorName(monitor.monitor);
        glfwGetMonitorPhysicalSize(monitor.monitor, &monitor.physicalSize[0], &monitor.physicalSize[1]);
        glfwGetMonitorContentScale(monitor.monitor, &monitor.scale[0], &monitor.scale[1]);
        glfwGetMonitorPos(monitor.monitor, &monitor.pos[0], &monitor.pos[1]);
        glfwGetMonitorWorkarea(monitor.monitor, &monitor.workArea.pos[0], &monitor.workArea.pos[1], &monitor.workArea.size[0], &monitor.workArea.size[1]);

        int vmCount;
        const GLFWvidmode* modes = glfwGetVideoModes(monitor.monitor, &vmCount);

        for (int j = 0; j < vmCount; j++) {
            VideoMode mode;
            mode.refreshRate = modes[i].refreshRate;
            mode.redBits = modes[i].redBits;
            mode.greenBits = modes[i].greenBits;
            mode.blueBits = modes[i].blueBits;
            mode.size = hg::Vec2i(modes[i].width, modes[i].height);
            monitor.videoModes.push_back(mode);
        }

        out.push_back(monitor);
    }

    return out;
}
