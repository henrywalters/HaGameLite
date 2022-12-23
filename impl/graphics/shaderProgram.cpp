//
// Created by henry on 12/17/22.
//

#include "../../include/hagame/graphics/glfw.h"
#include "../../include/hagame/graphics/shaderProgram.h"

hg::graphics::ShaderProgram::ShaderProgram(std::string _name, hg::graphics::Shader vertex, hg::graphics::Shader fragment): vertexShader(vertex), fragmentShader(fragment) {
    name = _name;
    id = glCreateProgram();
    glAttachShader(id, vertexShader.id);
    glAttachShader(id, fragmentShader.id);
    glLinkProgram(id);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(id, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        getProgramLog();
        throw std::runtime_error("Failed to link shader program");
    }
}

std::string hg::graphics::ShaderProgram::getProgramLog() {
    if (glIsProgram(id)) {
        int logLength = 0;
        int maxLength = logLength;

        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(id, maxLength, &logLength, infoLog);

        printf("%s\n", infoLog);

        return infoLog;
    }
    else {
        return "Is not program";
    }
}

void hg::graphics::ShaderProgram::use()
{
    glUseProgram(id);
}

unsigned int hg::graphics::ShaderProgram::getUniform(std::string var)
{
    return glGetUniformLocation(id, var.c_str());
}

int hg::graphics::ShaderProgram::getAttrib(std::string var) {
    return glGetAttribLocation(id, var.c_str());
}

void hg::graphics::ShaderProgram::setInt(std::string var, int val)
{
    glUniform1i(getUniform(var), val);
}

void hg::graphics::ShaderProgram::setFloat(std::string var, float val)
{
    glUniform1f(getUniform(var), val);
}

void hg::graphics::ShaderProgram::setVec2(std::string var, Vec2 vec) {
    glUniform2fv(getUniform(var), 1, &vec[0]);
}

void hg::graphics::ShaderProgram::setVec3(std::string var, Vec3 vec) {
    glUniform3fv(getUniform(var), 1, &vec[0]);
}

void hg::graphics::ShaderProgram::setVec4(std::string var, Vec4 vec) {
    glUniform4fv(getUniform(var), 1, &vec[0]);
}

void hg::graphics::ShaderProgram::setMat4(std::string var, Mat4 mat) {
    glUniformMatrix4fv(getUniform(var), 1, GL_TRUE, &mat[0]);
}

/*
void hg::graphics::ShaderProgram::setMaterial(std::string var, Material mat)
{
    setVec3(var + ".ambient", mat.ambient);
    setVec3(var + ".diffuse", mat.diffuse);
    setVec3(var + ".specular", mat.specular);
    setFloat(var + ".shininess", mat.shininess);
}

void hg::graphics::ShaderProgram::setLight(std::string var, Vec3 pos, Light light)
{
    setVec3(var + ".ambient", light.ambient);
    setVec3(var + ".diffuse", light.diffuse);
    setVec3(var + ".specular", light.specular);
    setVec3(var + ".position", pos);
}

void hg::graphics::ShaderProgram::setLight(std::string var, DirLight light)
{
    setVec3(var + ".ambient", light.ambient);
    setVec3(var + ".diffuse", light.diffuse);
    setVec3(var + ".specular", light.specular);
    setVec3(var + ".direction", light.direction);
}

void hg::graphics::ShaderProgram::setLight(std::string var, Vec3 pos, PointLight light)
{
    setVec3(var + ".ambient", light.ambient);
    setVec3(var + ".diffuse", light.diffuse);
    setVec3(var + ".specular", light.specular);
    setVec3(var + ".position", pos);
    setFloat(var + ".constant", light.constant);
    setFloat(var + ".linear", light.linear);
    setFloat(var + ".quadratic", light.quadratic);
}
 */

void hg::graphics::ShaderProgram::setMVP(Mat4 model, Mat4 view, Mat4 projection)
{
    setMat4("model", model);
    setMat4("projection", projection);
    setMat4("view", view);
}

void hg::graphics::ShaderProgram::setVP(Mat4 view, Mat4 projection)
{
    setMat4("view", view);
    setMat4("projection", projection);
}