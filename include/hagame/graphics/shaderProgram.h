//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_SHADERPROGRAM_H
#define HAGAME2_SHADERPROGRAM_H

#include <string>
#include "shader.h"
#include "../math/aliases.h"

namespace hg::graphics {
    class ShaderProgram {
    public:
        unsigned int id;
        std::string name;
        Shader vertexShader;
        Shader fragmentShader;

        ShaderProgram(std::string _name, Shader vertex, Shader fragment);

        std::string getProgramLog();

        void use();

        unsigned int getUniform(std::string var);
        int getAttrib(std::string var);
        void setBool(std::string var, bool val);
        void setInt(std::string var, int val);
        void setFloat(std::string var, float val);
        void setDouble(std::string var, double val);
        void setVec2(std::string var, Vec2 vect);
        void setVec3(std::string var, Vec3 vect);
        void setVec4(std::string var, Vec4 vect);
        void setMat4(std::string var, Mat4 mat);

        //void setMaterial(std::string var, Material mat);
        //void setLight(std::string var, Vec3 pos, Light light);
        //void setLight(std::string var, DirLight light);
        //void setLight(std::string var, Vec3 pos, PointLight light);

        void setMVP(Mat4 model, Mat4 view, Mat4 projection);
        void setVP(Mat4 view, Mat4 projection);
    };
}

#endif //HAGAME2_SHADERPROGRAM_H
