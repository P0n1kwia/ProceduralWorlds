#include "terrain_generator.hpp"
#include <FastNoiseLite.h>
#include <iostream>
procedural_mesh terrain_generator::GenerateFlatGrid(int width, int depth,int octaves ,float persistence, float lacunarity)
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetSeed(2137);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm); 
	noise.SetFractalOctaves(5);      
	noise.SetFractalLacunarity(2.0f); 
	noise.SetFractalGain(0.5f);
	float scale = 0.0001f;
	float frequency = 1.0f;
	float amplitude = 1.0f;
	std::vector<vertex> verticies;
	verticies.reserve((width+1) * (depth+1));
	for (int z = 0; z <= depth; z++)
	{
		for (int x = 0; x <= width; x++)
		{
			float noiseValue = noise.GetNoise((float)x/scale * frequency, (float)z/scale * frequency);

			
			float y = (noiseValue+1.0)/2.0 * amplitude;
			
			
		

			glm::vec3 pos = glm::vec3(x, y, z);
			glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
			glm::vec2 texCoord = glm::vec2(float(x) / width, float(z) / depth);
			vertex vert{}; vert.normal = normal; vert.position = pos; vert.texCoord = texCoord;
			verticies.push_back(vert);
		}
	}


	std::vector<unsigned int> indices;
	indices.reserve(6 * width * depth);
	for (int z = 0; z < depth; z++)
	{
		for (int x = 0; x < width; x++)
		{
			int rowSize = width + 1;
			unsigned int lowerLeft = z * rowSize + x;
			unsigned int lowerRight = z * rowSize + (x + 1);
			unsigned int upperRight = (z + 1) * rowSize + x+1;
			unsigned int upperLeft = (z + 1) * rowSize + x;

			indices.push_back(lowerLeft);
			indices.push_back(upperLeft);
			indices.push_back(lowerRight);

			indices.push_back(lowerRight);
			indices.push_back(upperLeft);
			indices.push_back(upperRight);

		}
	}
	



	return { verticies,indices };
}

