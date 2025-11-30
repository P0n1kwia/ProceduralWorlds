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

			//we check whether we have that chunk in our map, or whether our chunk is waiting to be generated
			bool isReady = activeChunks.find(targetCoord) != activeChunks.end();
			bool isGenerating = pendingChunks.find(targetCoord) != pendingChunks.end();

			if (!isReady && !isGenerating)
			{
				pendingChunks[targetCoord] = std::async(std::launch::async, &terrain_generator::Generate, &generator, targetCoord.first
				,targetCoord.second, settings);
			}
			for (auto it = pendingChunks.begin(); it != pendingChunks.end();)
			{
				if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				{
					meshData data = it->second.get();
					activeChunks[it->first] = std::make_unique<procedural_mesh>(data);
					it = pendingChunks.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
	}
	for (auto it = activeChunks.begin(); it != activeChunks.end();)
	{
		chunkCoord chunkPos = it->first;
		int distX = std::fabs(chunkPos.first - coords.first);
		int distZ = std::fabs(chunkPos.second - coords.second);

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
