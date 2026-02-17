#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum class ChunkEdge : int
{
    PosX = 0, 
    NegX = 1, 
    PosZ = 2, 
    NegZ = 3, 
};



//This is just a compute shader
class normalSmoother
{
public:
    normalSmoother(const std::string& shaderPath)
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
        loc_edgeType = glGetUniformLocation(programID, "u_edgeType");
        loc_lineVerts = glGetUniformLocation(programID, "u_lineVerts");
    }

    ~normalSmoother()
    {
        glDeleteProgram(programID);
    }

    void Dispatch(unsigned int vboA, unsigned int vboB, ChunkEdge edge, int lineVerts, GLsizeiptr vboSizeA, GLsizeiptr vboSizeB)
    {
        glUseProgram(programID);
        glUniform1i(loc_borderLength, lineVerts);
        glUniform1i(loc_edgeType, static_cast<int>(edge));
        glUniform1i(loc_lineVerts, lineVerts);

        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, vboA, 0, vboSizeA);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, vboB, 0, vboSizeB);
        int groups = (lineVerts + 63) / 64;
        glDispatchCompute(groups, 1, 1);


        glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }

private:
    unsigned int programID = 0;
    int loc_borderLength = -1;
    int loc_edgeType = -1;
    int loc_lineVerts = -1;

    static std::string LoadSource(const std::string& path)
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

    static void CheckShader(unsigned int id)
    {
        int ok; 
        char log[1024];
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            glGetShaderInfoLog(id, 1024, nullptr, log);
            std::cerr << "Failed to compile Compute Shader![normalSmoother]" << log << "\n";
        }
    }

    static void CheckProgram(unsigned int id)
    {
        int ok; char log[1024];
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glGetProgramInfoLog(id, 1024, nullptr, log);
            std::cerr << "Failed to link Compute Shader![normalSmoother]\n" << log << "\n";
        }
    }
};