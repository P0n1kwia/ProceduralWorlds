#include "terrain_generator.hpp"
#include <FastNoiseLite.h>
#include <thread>
#include <iostream>
meshData terrain_generator::Generate(int chunkX, int chunkZ, const terrainSettings& settings) const
{

	std::cout << "[THREAD " << std::this_thread::get_id() << "] generates chunk: " << chunkX << ", " << chunkZ << "\n";
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalLacunarity(settings.lacunarity);
	noise.SetFractalOctaves(settings.octaves);
	noise.SetFractalGain(settings.persistence);
	noise.SetFrequency(settings.frequency);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);

	

	//generating verticies for our mesh
	std::vector<vertex> verticies;
	verticies.reserve((settings.chunkSize + 1) * (settings.chunkSize + 1));
	for (int z = 0; z <= settings.chunkSize; z++)
	{
		for (int x = 0; x <= settings.chunkSize; x++)
		{
			float worldX = chunkX * settings.chunkSize + x;
			float worldZ = chunkZ * settings.chunkSize + z;
			float heightValue  = noise.GetNoise(worldX * settings.scale, worldZ * settings.scale);
			heightValue = (heightValue + 1.0f) * 0.5f;
			heightValue = settings.smoothingFunction(heightValue);
			
			glm::vec3 pos = glm::vec3(worldX, heightValue*settings.maxMeshHeight , worldZ);
			glm::vec3 normals = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec2 texCoords = glm::vec2((float)x / settings.chunkSize, (float)z / settings.chunkSize);
			vertex vert{ pos,normals,texCoords };
			verticies.push_back(vert);
		}
	}
	std::vector<unsigned int> indices;
	indices.reserve(6 * settings.chunkSize * settings.chunkSize);
	int rowSize = settings.chunkSize + 1;

	for (int z = 0; z < settings.chunkSize; z++)
	{
		for (int x = 0; x < settings.chunkSize; x++)
		{
			unsigned int lowerLeft = z * rowSize + x;
			unsigned int lowerRight = lowerLeft + 1;
			unsigned int upperLeft = (z + 1) * rowSize + x;
			unsigned int upperRight = upperLeft + 1;

			// triangle 1
			indices.push_back(lowerLeft);
			indices.push_back(upperLeft);
			indices.push_back(lowerRight);

			// triangle 2
			indices.push_back(lowerRight);
			indices.push_back(upperLeft);
			indices.push_back(upperRight);
		}
	}
	//calculation triangle normals
	for (int i = 0; i < indices.size(); i += 3)
	{
		unsigned int i0 = indices[i];
		unsigned int i1 = indices[i+1];
		unsigned int i2 = indices[i+2];

		glm::vec3 v0 = verticies[i0].position;
		glm::vec3 v1 = verticies[i1].position;
		glm::vec3 v2 = verticies[i2].position;
		

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 faceNormal = glm::cross(edge1, edge2);
		verticies[i0].normal += faceNormal;
		verticies[i1].normal += faceNormal;
		verticies[i2].normal += faceNormal;
	}
	for (auto& vert : verticies)
	{
		vert.normal = glm::normalize(vert.normal);
	}
	

	return { verticies, indices };
}
