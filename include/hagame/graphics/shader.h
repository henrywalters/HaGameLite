//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_SHADER_H
#define HAGAME2_SHADER_H

#include <string>

namespace hg::graphics {

    class Shader {
        unsigned int id;

        // Static Members
        static Shader Load(unsigned int type, std::string source);
        static Shader LoadVertex(std::string source);
        static Shader LoadFragment(std::string source);

        // Regular members
        std::string getShaderLog();
    };
}

#endif //HAGAME2_SHADER_H
