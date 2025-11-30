#pragma once
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include "procedural_mesh.hpp"
#include "terrain_generator.hpp"
using chunkCoord = std::pair<int, int>;
class chunk_job
{
public:
	std::thread worker;
	std::atomic<bool> isDone;
	meshData result;
	chunk_job() : isDone(false) {}
private:

};
class chunk_manager
{
public:
	chunk_manager(int viewDistance, const terrainSettings& settings);
	void Update(const glm::vec3& worldPos);
	void Draw(shader& shader);
private:
	int viewDistance;
	terrainSettings settings;
	terrain_generator generator;
	//chunks that we can draw
	std::map<chunkCoord, std::unique_ptr<procedural_mesh>> activeChunks;
	//chunks that will be calculated on threads
	std::map<chunkCoord, std::unique_ptr<chunk_job>> pendingChunks;



	chunkCoord GetChunkCoordFromPosition(glm::vec3 pos);
};