#include "forest_generator.hpp"
#include <FastNoiseLite.h>
#include <cmath>

float forestGenerator::Hash(int a, int b, int seed) const
{
    unsigned int h = static_cast<unsigned int>(a * 73856093 ^ b * 19349663 ^ seed * 83492791);
    h = (h ^ (h >> 16)) * 0x45d9f3b;
    h = (h ^ (h >> 16)) * 0x45d9f3b;
    h = h ^ (h >> 16);
    return static_cast<float>(h & 0xFFFFFF) / static_cast<float>(0xFFFFFF);
}

float forestGenerator::SampleHeight(float worldX, float worldZ,
    const terrainSettings& settings) const
{
    FastNoiseLite noise;
    noise.SetSeed(settings.seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalLacunarity(settings.lacunarity);
    noise.SetFractalOctaves(settings.octaves);
    noise.SetFractalGain(settings.persistence);
    noise.SetFrequency(settings.frequency);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);

    float h = noise.GetNoise(worldX * settings.scale, worldZ * settings.scale);
    h = (h + 1.0f) * 0.5f;
    h = settings.smoothingFunction(h);
    return h;
}
std::optional<forestData> forestGenerator::GenerateData(int chunkX, int chunkZ, const terrainSettings& terrain, const forestSettings& forest) const
{
    LSystemsGenerator lGen(forest.lsystem);
    std::string lString = lGen.Generate(forest.lsystemIterations);

    turtleInterpreter turtle(25.0f, 0.05f, 0.03f);
    std::vector<glm::mat4> branchTransforms = turtle.Translate(lString);

    if (branchTransforms.empty())
        return std::nullopt;

    float chunkOriginX = chunkX * static_cast<float>(terrain.chunkSize);
    float chunkOriginZ = chunkZ * static_cast<float>(terrain.chunkSize);

    forestData result;
    result.cylinder = forest.cylinder;
    result.instanceTransforms.reserve(forest.treesPerChunk * branchTransforms.size());

    for (int i = 0; i < forest.treesPerChunk; i++)
    {
        float tx = Hash(chunkX * 1000 + i, chunkZ, terrain.seed) * terrain.chunkSize;
        float tz = Hash(chunkX, chunkZ * 1000 + i, terrain.seed) * terrain.chunkSize;

        float worldX = chunkOriginX + tx;
        float worldZ = chunkOriginZ + tz;

        float normalisedHeight = SampleHeight(worldX, worldZ, terrain);

        if (normalisedHeight < forest.minHeight || normalisedHeight > forest.maxHeight)
            continue;

        float eps = 1.0f;
        float hR = SampleHeight(worldX + eps, worldZ, terrain) * terrain.maxMeshHeight;
        float hL = SampleHeight(worldX - eps, worldZ, terrain) * terrain.maxMeshHeight;
        float hU = SampleHeight(worldX, worldZ + eps, terrain) * terrain.maxMeshHeight;
        float hD = SampleHeight(worldX, worldZ - eps, terrain) * terrain.maxMeshHeight;

        glm::vec3 approxNormal = glm::normalize(glm::vec3(hL - hR, 2.0f * eps, hD - hU));
        float flatness = glm::dot(approxNormal, glm::vec3(0.0f, 1.0f, 0.0f));

        if (flatness < forest.maxSlope)
            continue;

        float worldY = normalisedHeight * terrain.maxMeshHeight;
        float scale = forest.minScale + Hash(i, chunkX ^ chunkZ, terrain.seed) * (forest.maxScale - forest.minScale);
        float rotY = Hash(i, chunkX + chunkZ * 7, terrain.seed + 1) * 360.0f;

        glm::mat4 root = glm::mat4(1.0f);
        root = glm::translate(root, glm::vec3(worldX, worldY, worldZ));
        root = glm::rotate(root, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        root = glm::scale(root, glm::vec3(scale));

        for (const auto& branch : branchTransforms)
            result.instanceTransforms.push_back(root * branch);
    }

    if (result.instanceTransforms.empty())
        return std::nullopt;

    return result;
}
std::optional<instancedMesh> forestGenerator::BuildMesh(forestData&& data)
{
    if (data.instanceTransforms.empty())
        return std::nullopt;

    plant_generator pGen;
    return pGen.Generate(data.instanceTransforms, data.cylinder);
}