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
				auto job = std::make_unique<chunk_job>();
				meshData* resultPtr = &job->result;
				std::atomic<bool>* donePtr = &job->isDone;
				job->worker = std::thread([this, targetCoord, resultPtr, donePtr]()
					{
						*resultPtr = generator.Generate(targetCoord.first, targetCoord.second, settings);
						*donePtr = true;
					});
				pendingChunks[targetCoord] = std::move(job);

			}
			for (auto it = pendingChunks.begin(); it != pendingChunks.end();)
			{
				if (it->second->isDone)
				{
					if (it->second->worker.joinable())
					{
						it->second->worker.join();
					}
					meshData data = std::move(it->second->result);
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
