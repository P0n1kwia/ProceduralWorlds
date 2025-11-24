#include "terrain_generator.hpp"

procedural_mesh terrain_generator::Generate(int width, int depth, const terrainSettings& settings) const
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFractalLacunarity(settings.lacunarity);
	noise.SetFractalOctaves(settings.octaves);
	noise.SetFractalGain(settings.persistence);
	//generating verticies for our mesh
	std::vector<vertex> verticies;
	verticies.reserve((width + 1) * (depth * 1));
	for (int z = 0; z <= depth; z++)
	{
		for (int x = 0; x <= width; x++)
		{

			float y = 0;
			glm::vec3 pos = glm::vec3(x, y, z);
			glm::vec3 normals = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec2 texCoords = glm::vec2((float)x / width, (float)z / depth);
			vertex vert{ pos,normals,texCoords };
			verticies.push_back(vert);
		}
	}
	std::vector<unsigned int> indices;
	indices.reserve(6 * width * depth);
	int rowSize = width + 1;

	for (int z = 0; z < depth; z++)
	{
		for (int x = 0; x < width; x++)
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
	

	return { verticies, indices };
}
