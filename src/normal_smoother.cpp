#include "normal_smoother.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

normalSmoother::normalSmoother(const std::string& shaderPath)
{
    std::string src = LoadSource(shaderPath);
    const char* csrc = src.c_str();

    unsigned int cs = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(cs, 1, &csrc, nullptr);
    glCompileShader(cs);
    CheckShader(cs);

    programID = glCreateProgram();
    glAttachShader(programID, cs);
    glLinkProgram(programID);
    CheckProgram(programID);
    glDeleteShader(cs);

    loc_borderLength = glGetUniformLocation(programID, "u_borderLength");
    loc_edgeType     = glGetUniformLocation(programID, "u_edgeType");
    loc_lineVerts    = glGetUniformLocation(programID, "u_lineVerts");
}

normalSmoother::~normalSmoother()
{
    glDeleteProgram(programID);
}

void normalSmoother::Dispatch(unsigned int vboA, unsigned int vboB,
                               ChunkEdge edge, int lineVerts,
                               std::ptrdiff_t vboSizeA, std::ptrdiff_t vboSizeB)
{
    glUseProgram(programID);
    glUniform1i(loc_borderLength, lineVerts);
    glUniform1i(loc_edgeType,     static_cast<int>(edge));
    glUniform1i(loc_lineVerts,    lineVerts);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, vboA, 0, static_cast<GLsizeiptr>(vboSizeA));
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, vboB, 0, static_cast<GLsizeiptr>(vboSizeB));

    int groups = (lineVerts + 63) / 64;
    glDispatchCompute(groups, 1, 1);

    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

std::string normalSmoother::LoadSource(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        std::cerr << "[normalSmoother] Cannot open: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void normalSmoother::CheckShader(unsigned int id)
{
    int ok; char log[1024];
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        glGetShaderInfoLog(id, 1024, nullptr, log);
        std::cerr << "Failed to compile Compute Shader! [normalSmoother]\n" << log << "\n";
    }
}

void normalSmoother::CheckProgram(unsigned int id)
{
    int ok; char log[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        glGetProgramInfoLog(id, 1024, nullptr, log);
        std::cerr << "Failed to link Compute Shader! [normalSmoother]\n" << log << "\n";
    }
}
