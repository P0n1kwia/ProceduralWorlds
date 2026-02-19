#pragma once
#include "instanced_mesh.hpp"
#include "terrain_generator.hpp"
#include "Lsystems_generator.hpp"
#include "turtle_interpreter.hpp"
#include "plant_generator.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <optional>
struct forestSettings
{
    int   treesPerChunk = 40;
    float minHeight = 0.05f;
    float maxHeight = 0.75f;
    float maxSlope = 0.7f;
    float minScale = 0.6f;
    float maxScale = 1.4f;
    int   lsystemIterations = 5;
    LSystemsSettings lsystem;
    cylinderSettings cylinder;
};
struct forestData
{
    std::vector<glm::mat4> instanceTransforms;  
    cylinderSettings       cylinder;            
};

class forestGenerator
{
public:
    forestGenerator() = default;
    std::optional<forestData> GenerateData( int chunkX, int chunkZ, const terrainSettings& terrain, const forestSettings& forest) const;


    static std::optional<instancedMesh> BuildMesh(forestData&& data);
private:
    float SampleHeight(float worldX, float worldZ, const terrainSettings& settings) const;
    float Hash(int a, int b, int seed) const;
};