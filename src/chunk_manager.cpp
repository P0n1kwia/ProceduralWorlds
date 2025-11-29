#include "chunk_manager.hpp"

chunk_manager::chunk_manager(int viewDistance, const terrainSettings& settings)
{
	this->viewDistance = viewDistance;
	this->settings = settings;
}

void chunk_manager::Update(const glm::vec3& worldPos)
{
	chunkCoord coords = GetChunkCoordFromPosition(worldPos);


	for (int x = -viewDistance; x <= viewDistance; x++)
	{
		for (int z = -viewDistance; z <= viewDistance; z++)
		{
			chunkCoord targetCoord = { coords.first + x,coords.second + z };

			if (activeChunks.find(targetCoord) == activeChunks.end())
			{
				auto meshData = generator.Generate(targetCoord.first, targetCoord.second, settings);
				activeChunks[targetCoord] = std::make_unique<procedural_mesh>(meshData);

			}
		}
	}
	for (auto it = activeChunks.begin(); it != activeChunks.end();)
	{
		chunkCoord chunkPos = it->first;
		int distX = std::fabs(chunkPos.first - worldPos.x);
		int distZ = std::fabs(chunkPos.second - worldPos.z);

		int dist = std::max(distX, distZ);


		if (dist > viewDistance + 1)
		{
			it = activeChunks.erase(it);
		}
		else
		{
			it++;
		}
	}
	
	
	

	
}

void chunk_manager::Draw(shader& shader)
{
	for (const auto& [coord, mesh] : activeChunks)
	{
		mesh->Draw(shader);
	}
}

chunkCoord chunk_manager::GetChunkCoordFromPosition(glm::vec3 pos)
{
	int x = std::floor(pos.x / (float)settings.chunkSize);
	int z = std::floor(pos.z / (float)settings.chunkSize);
	return { x,z };
}
