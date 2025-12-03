#include "plant_generator.hpp"



instancedMesh plant_generator::Generate(const std::vector<glm::mat4>& matricies, const cylinderSettings& settings) const
{
	meshData data;
	std::vector<float> circleVerticies = UnitCircleVerticies(settings);
	float a = settings.radiusTop;
	float b = settings.radiusBottom;
	//side verticies
	for (int i = 0; i < 2; i++)
	{
		float h = -settings.height * 0.5f + i * settings.height;
		float t = 1.0f - i;
		int k = 0;
		float radius = (1 - i) * a + i * b;
		
		for (int j = 0; j <= settings.sectorCount; j++)
		{
			vertex vert;
		
			float ax = circleVerticies[k];
			float ay = circleVerticies[k+1];
			float az = circleVerticies[k+2];

			vert.position[0] = ax*radius;
			vert.position[1] = h;
			vert.position[2] = ay * radius;


			vert.normal[0] = ax;
			vert.normal[1] = 0;
			vert.normal[2] = az;

			vert.texCoord[0] = (float(j) / settings.sectorCount);
			vert.texCoord[1] = t;

			k += 3;
			data.verticies.push_back(vert);
		}
	}

	int baseCenterIndex = data.verticies.size() / 3;
	int topCenterIndex = baseCenterIndex + settings.sectorCount + 1;

	for (int i = 0; i < 2; i++)
	{
		float h = i * settings.height;
		float ny = -1.0 + i * 2;
		vertex centerVert;
		centerVert.position = glm::vec3(0.0f, h, 0.0f);
		centerVert.normal = glm::vec3(0.0f, ny, 0.0f);
		centerVert.texCoord = glm::vec2(0.5f, 0.5f);
		data.verticies.push_back(centerVert);
		int k = 0;
		float radius = (1 - i) * a + i * b;
		for (int j = 0; j <= settings.sectorCount; j++)
		{
			vertex vert;
			
			float ax = circleVerticies[k];
			float ay = circleVerticies[k + 1];
			float az = circleVerticies[k + 2];

			vert.position[0] = ax * radius;
			vert.position[1] = ay * radius;
			vert.position[2] = h;


			vert.normal[0] = 0;
			vert.normal[1] = ny;
			vert.normal[2] = 0;

			vert.texCoord[0] = (-ax*0.5f + 0.5f);
			vert.texCoord[1] = (-ay*0.5f + 0.5f);

			k += 3;
			data.verticies.push_back(vert);
		}
	}
	//creating indicies
	int k1 = 0;
	int k2 = settings.sectorCount + 1;
	for (int i = 0; i < settings.sectorCount; i++)
	{
		data.indicies.push_back(k1);
		data.indicies.push_back(k1+1);
		data.indicies.push_back(k2);

		data.indicies.push_back(k2);
		data.indicies.push_back(k1 + 1);
		data.indicies.push_back(k2 + 1);

		k1++;
		k2++;

	}
	int k = baseCenterIndex;
	for (int i = 0; i < settings.sectorCount; i++)
	{
		if (i < settings.sectorCount - 1)
		{
			data.indicies.push_back(baseCenterIndex);
			data.indicies.push_back(k + 1);
			data.indicies.push_back(k);
		}
		else
		{
			data.indicies.push_back(baseCenterIndex);
			data.indicies.push_back(baseCenterIndex + 1);
			data.indicies.push_back(k);

		}


		k++;
	}
	 k = topCenterIndex+1;
	for (int i = 0; i < settings.sectorCount; i++)
	{
		if (i < settings.sectorCount - 1)
		{
			data.indicies.push_back(topCenterIndex);
			data.indicies.push_back(k);
			data.indicies.push_back(k+1);
		}
		else
		{
			data.indicies.push_back(topCenterIndex);
			data.indicies.push_back(k);
			data.indicies.push_back(topCenterIndex+1);

		}


		k++;
	}




	return { data,matricies };
}

std::vector<float> plant_generator::UnitCircleVerticies(const cylinderSettings& settings) const
{
	std::vector<float> vertexPos;
	vertexPos.reserve((sectorCount + 1) * 3)
	float sectorAngle;
	for (int i = 0; i <= settings.sectorCount; ++i)
	{
		sectorAngle = i * settings.sectorStep;
		vertexPos.push_back(std::cos(sectorAngle));
		vertexPos.push_back(std::sin(sectorAngle));
		vertexPos.push_back(0);
	}


	return vertexPos;
}
