#pragma once
#include <map>
#include <memory>
#include "procedural_mesh.hpp"
#include "terrain_generator.hpp"
using chunkCoord = std::pair<int, int>;
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
	std::map<chunkCoord, std::unique_ptr<procedural_mesh>> activeChunks;


	chunkCoord GetChunkCoordFromPosition(glm::vec3 pos);
};