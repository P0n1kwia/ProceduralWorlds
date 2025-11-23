#pragma once
#include <procedural_mesh.hpp>

class terrain_generator
{
public:
	procedural_mesh GenerateFlatGrid(int width, int depth,int octaves, float persistence, float lacunarity);
private:

};