#pragma once
#include <string>
#include <cstddef>

enum class ChunkEdge : int
{
    PosX = 0,   // chunk's right column
    NegX = 1,   // chunk's left  column
    PosZ = 2,   // chunk's top   row
    NegZ = 3,   // chunk's bottom row
};

class normalSmoother
{
public:
    normalSmoother(const std::string& shaderPath);
    ~normalSmoother();

    void Dispatch(unsigned int vboA, unsigned int vboB,
        ChunkEdge edge, int lineVerts,
        std::ptrdiff_t vboSizeA, std::ptrdiff_t vboSizeB);

private:
    unsigned int programID = 0;
    int loc_borderLength = -1;
    int loc_edgeType = -1;
    int loc_lineVerts = -1;

    static std::string LoadSource(const std::string& path);
    static void CheckShader(unsigned int id);
    static void CheckProgram(unsigned int id);
};