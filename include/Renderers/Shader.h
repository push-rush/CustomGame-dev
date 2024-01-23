#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <glew.h>
#include <glfw3.h>

#include <SDL.h>

class Shader
{
private:
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;

    bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
    bool isCompiled(GLuint shader);
    bool isValidProgram();

public:
    Shader(/* args */);
    ~Shader();

    bool Load(const std::string& vertName, const std::string& fragName);
    void unLoad();
    void setActive();

    // setters
    void setMatrixUniform(const char* name, const class Matrix4& matrix);
    void setMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
    void setVectorUniform(const char* name, const class Vector3& vector);
    void setVector2Uniform(const char* name, const class Vector2& vector);
    void setFloatUniform(const char* name, float value);
    void setIntUniform(const char* name, int value);
};