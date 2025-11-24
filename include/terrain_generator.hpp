#pragma once
#include <FastNoiseLite.h>
#include <procedural_mesh.hpp>
#include <functional>
struct terrainSettings
{
	int seed = 1337;
	int octaves = 1;
	float scale = 0.01f;
	float lacunarity = 1.0f;
	float persistence = 1.0f;
	std::function<float(float)> smoothingFunction = [](float x) {return x; };
};
class terrain_generator
{
public:
	procedural_mesh Generate(int width, int depth,const terrainSettings& settings) const;
private:

};