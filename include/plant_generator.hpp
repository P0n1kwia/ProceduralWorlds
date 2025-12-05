#pragma once
#include <instanced_mesh.hpp>
struct cylinderSettings
{
	int sectorCount;
	int sectorStep; // 2*PI / sectorCount
	float radiusTop;
	float radiusBottom;
	float height;
	const float PI = 3.1415926f;
	void CalculateSectorStep()
	{
		sectorStep = 2 * PI / sectorCount;
	}
	
};
class plant_generator
{
public:
	instancedMesh Generate(const std::vector<glm::mat4>& matricies,const cylinderSettings& settings) const;
private:
	std::vector<float> UnitCircleVerticies(const cylinderSettings& settings) const;

};