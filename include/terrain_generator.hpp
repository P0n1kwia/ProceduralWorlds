#pragma once
#include <procedural_mesh.hpp>
#include <functional>
struct terrainSettings
{
	int seed = 1337;
	int octaves = 1;
	int chunkSize = 241;
	float scale = 0.01f;
	float lacunarity = 1.0f;
	float persistence = 1.0f;
	float frequency = 1.0f;
	float maxMeshHeight = 2.0;
	std::function<float(float)> smoothingFunction = [](float x) {return x; };
	int levelOfDetail = 2; //1, 2, 4, 6, 8, 10 ,12 
};
class terrainGenerator
{
public:
	meshData Generate(int chunkX, int chunkZ,const terrainSettings& settings) const;
private:
	
};