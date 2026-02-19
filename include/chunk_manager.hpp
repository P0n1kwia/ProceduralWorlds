#pragma once
#include <map>
#include <memory>
#include <chrono>
#include <future>
#include <optional>
#include "procedural_mesh.hpp"
#include "terrain_generator.hpp"
#include "normal_smoother.hpp"
#include "forest_generator.hpp"
using chunkCoord = std::pair<int, int>;
struct PairHash //hash function
{
	size_t operator() (const chunkCoord& p) const
	{
		size_t h1 = std::hash<int>{}(p.first);
		size_t h2 = std::hash<int>{}(p.second);
		return h1 ^ (h2 << 32 | h2 >> 32);
	}
};
struct chunkData
{
	std::unique_ptr<proceduralMesh>terrain;
	std::optional<instancedMesh> forest;
};
struct generatedChunk
{
	meshData terrainData;
	std::optional<forestData> forestData;    
};
class chunkManager
{
public:
	chunkManager(int viewDistance,const terrainSettings& terrainS,const forestSettings& forestS);
	void Update(const glm::vec3& worldPos);
	void Draw(shader& terrainShader, shader& plantShader);
private:
	int viewDistance;
	terrainSettings settings;
	terrainGenerator terrainGen;
	forestSettings forest;
	forestGenerator forestGen;
	//chunks that we can draw
	std::map<chunkCoord, chunkData> activeChunks;
	//chunks that will be calculated on threads
	std::map<chunkCoord, std::future<generatedChunk>> pendingChunks;
	normalSmoother smooth{ "shaders/normalSmooth.glsl" };

	chunkCoord GetChunkCoordFromPosition(glm::vec3 pos);
	void SmoothBorders(const chunkCoord& c, proceduralMesh& newChunk);
};