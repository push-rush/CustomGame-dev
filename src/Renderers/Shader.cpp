#include "../../include/Renderers/Shader.h"
#include "../../include/General/Cmath.h"

Shader::Shader(/* args */)
{
    this->mShaderProgram = 0;
    this->mVertexShader = 0;
    this->mFragShader = 0;
}

Shader::~Shader()
{
    
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open())
    {
        // 将所有文本读入一个字符串
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char* contentsChar = contents.c_str();

        // 创建指定类型的着色器
        outShader = glCreateShader(shaderType);
        // 设置着色器源代码的字符串
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!isCompiled(outShader))
        {
            SDL_Log("Failed to compile shader: %s", fileName.c_str());
            return false;
        }
    }
    else
    {
        SDL_Log("Shader file not found: %s", fileName.c_str());
        return false;
    }
    return true;
}

bool Shader::isCompiled(GLuint shader)
{
    GLint status;
    // 查询编译状态
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        SDL_Log("GLSL Compile Failed: \n %s", buffer);
        return false;
    }
    return true;
}   

bool Shader::isValidProgram()
{
    GLint status;
    // 查询编译状态
    glGetProgramiv(this->mShaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(this->mShaderProgram, 511, nullptr, buffer);
        SDL_Log("GLSL Compile Failed: \n %s", buffer);
        return false;
    }
    return true;
}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
    if (!compileShader(vertName, GL_VERTEX_SHADER, this->mVertexShader) ||
        !compileShader(fragName, GL_FRAGMENT_SHADER, this->mFragShader))
    {
        return false;
    }

    // 将上面两个着色器链接到着色器程序上
    this->mShaderProgram = glCreateProgram();
    glAttachShader(this->mShaderProgram, this->mVertexShader);
    glAttachShader(this->mShaderProgram, this->mFragShader);
    glLinkProgram(this->mShaderProgram);

    // 验证程序是否链接成功
    if (!isValidProgram())
    {
        return false;
    }
    return true;
}

void Shader::setActive()
{
    glUseProgram(this->mShaderProgram);
}

void Shader::unLoad()
{
    glDeleteProgram(this->mShaderProgram);
    glDeleteShader(this->mVertexShader);
    glDeleteShader(this->mFragShader);
}

void Shader::setMatrixUniform(const char* name, const Matrix4& matrix)
{
    // 通过name定位全局变量
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);

    // 将矩阵数据传给全局变量
    glUniformMatrix4fv(
        loc,
        1,
        GL_TRUE,
        matrix.GetAsFloatPtr() // 传入指向矩阵数据的指针
    );
}

void Shader::setVectorUniform(const char* name, const Vector3& vector)
{
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);
    glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::setFloatUniform(const char* name, float value)
{
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);
    glUniform1f(loc, value);
}

void Shader::setMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);
    glUniformMatrix4fv(
        loc,
        count,
        GL_TRUE,
        matrices->GetAsFloatPtr()
    );
}

void Shader::setIntUniform(const char* name, int value)
{
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);
    glUniform1i(loc, value);
}

void Shader::setVector2Uniform(const char* name, const Vector2& vector)
{
    GLuint loc = glGetUniformLocation(this->mShaderProgram, name);
    glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}