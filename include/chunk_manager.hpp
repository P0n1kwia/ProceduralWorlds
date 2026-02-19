#pragma once
#include <map>
#include <memory>
#include <chrono>
#include <future>
#include "procedural_mesh.hpp"
#include "terrain_generator.hpp"
#include "normal_smoother.hpp"
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
class chunkManager
{
public:
	chunkManager(int viewDistance, const terrainSettings& settings);
	void Update(const glm::vec3& worldPos);
	void Draw(shader& shader);
private:
	int viewDistance;
	terrainSettings settings;
	terrainGenerator generator;
	//chunks that we can draw
	std::map<chunkCoord, std::unique_ptr<proceduralMesh>> activeChunks;
	//chunks that will be calculated on threads
	std::map<chunkCoord, std::future<meshData>> pendingChunks;
	normalSmoother smooth{ "shaders/normalSmooth.glsl" };

	chunkCoord GetChunkCoordFromPosition(glm::vec3 pos);
	void SmoothBorders(const chunkCoord& c, proceduralMesh& newChunk);
};