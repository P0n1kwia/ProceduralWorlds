#include "chunk_manager.hpp"



chunkManager::chunkManager(int viewDistance, const terrainSettings& terrainS, const forestSettings& forestS)
{
	this->viewDistance = viewDistance;
	this->settings = terrainS;
	this->forest = forestS;
}
static generatedChunk GenerateChunkAsync(int chunkX, int chunkZ, terrainSettings tCfg, forestSettings  fCfg)
{
	terrainGenerator tGen;
	forestGenerator  fGen;

	generatedChunk result;
	result.terrainData = tGen.Generate(chunkX, chunkZ, tCfg);
	//result.forestData = fGen.GenerateData(chunkX, chunkZ, tCfg, fCfg); uncomment this to generate forests, currently disabled for performance reasons
	return result;
}
void chunkManager::Update(const glm::vec3& worldPos)
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
				pendingChunks[targetCoord] = std::async(
					std::launch::async,
					GenerateChunkAsync,
					targetCoord.first, targetCoord.second,
					settings, forest);
			}
			
		}
	}
	for (auto it = pendingChunks.begin(); it != pendingChunks.end();)
	{
		if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			chunkCoord coord = it->first;
			generatedChunk gen = it->second.get();

			chunkData cd;
			cd.terrain = std::make_unique<proceduralMesh>(gen.terrainData);

			// BuildMesh does the GL upload - safe here because we're on the main thread
			if (gen.forestData.has_value())
				cd.forest = forestGenerator::BuildMesh(std::move(*gen.forestData));

			SmoothBorders(coord, *cd.terrain);
			activeChunks[coord] = std::move(cd);
			it = pendingChunks.erase(it);
		}
		else
		{
			it++;
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

void chunkManager::Draw(shader& terrainShader, shader& plantShader)
{
	for (const auto& [coord, cd] : activeChunks)
	{
		cd.terrain->Draw(terrainShader);
	}
	plantShader.use();
	for (auto& [coord, cd] : activeChunks)
	{
		if (cd.forest.has_value())
			cd.forest->Draw(plantShader);
	}
}

chunkCoord chunkManager::GetChunkCoordFromPosition(glm::vec3 pos)
{
	int x = std::floor(pos.x / (float)settings.chunkSize);
	int z = std::floor(pos.z / (float)settings.chunkSize);
	return { x,z };
}

void chunkManager::SmoothBorders(const chunkCoord& c, proceduralMesh& newChunk)
{
	struct Edges
	{
		chunkCoord offset;
		ChunkEdge edgeOfNewChunk;
		ChunkEdge edgeOfNeighbour;
	};
	const Edges neighbors[4] =
	{
		{ { +1,  0 }, ChunkEdge::PosX, ChunkEdge::NegX },
		{ { -1,  0 }, ChunkEdge::NegX, ChunkEdge::PosX },
		{ {  0, +1 }, ChunkEdge::PosZ, ChunkEdge::NegZ },
		{ {  0, -1 }, ChunkEdge::NegZ, ChunkEdge::PosZ },
	};
	for (const auto& e : neighbors)
	{
		chunkCoord neighCoord = { c.first + e.offset.first,c.second + e.offset.second };
		auto it = activeChunks.find(neighCoord);
		if (it == activeChunks.end()) continue; // neighbour not loaded yet
		proceduralMesh& neighb = *it->second.terrain;
		int lineVerts = newChunk.GetLineVerts();

		smooth.Dispatch(newChunk.GetVBO(), neighb.GetVBO(), e.edgeOfNewChunk, lineVerts, newChunk.GetVBOSize(), neighb.GetVBOSize());
	}
}
