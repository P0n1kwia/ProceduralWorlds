#include "plant_generator.hpp"
instancedMesh plant_generator::Generate(const std::vector<glm::mat4>& matricies, const cylinderSettings& settings) const
{
    meshData data;
    std::vector<float> circleVerticies = UnitCircleVerticies(settings); 

    float radiusBottom = settings.radiusBottom;
    float radiusTop = settings.radiusTop;

    for (int i = 0; i < 2; i++)
    {
       
        float h = i * settings.height;

        float t = 1.0f - i;
        float radius = (1 - i) * radiusBottom + i * radiusTop;

        int k = 0;
        for (int j = 0; j <= settings.sectorCount; j++)
        {
            float cx = circleVerticies[k];    
            float cz = circleVerticies[k + 1];
          

            vertex vert;

            
            vert.position[0] = cx * radius;
            vert.position[1] = h;            
            vert.position[2] = cz * radius;

            vert.normal[0] = cx;
            vert.normal[1] = 0.0f;
            vert.normal[2] = cz;

            vert.texCoord[0] = (float(j) / settings.sectorCount);
            vert.texCoord[1] = t;

            data.verticies.push_back(vert);
            k += 3;
        }
    }
    int k1 = 0;
    int k2 = settings.sectorCount + 1;

    for (int i = 0; i < settings.sectorCount; i++)
    {
        data.indicies.push_back(k1);
        data.indicies.push_back(k1 + 1);
        data.indicies.push_back(k2);

        data.indicies.push_back(k2);
        data.indicies.push_back(k1 + 1);
        data.indicies.push_back(k2 + 1);

        k1++;
        k2++;
    }

    for (int i = 0; i < 2; i++)
    {
        float h = i * settings.height;
        float ny = (i == 0) ? -1.0f : 1.0f; 
        float radius = (i == 0) ? radiusBottom : radiusTop;

        unsigned int centerIndex = data.verticies.size();

       
        vertex centerVert;
        centerVert.position = glm::vec3(0.0f, h, 0.0f);
        centerVert.normal = glm::vec3(0.0f, ny, 0.0f);
        centerVert.texCoord = glm::vec2(0.5f, 0.5f);
        data.verticies.push_back(centerVert);

        
        int k = 0;
        unsigned int ringStartIndex = data.verticies.size();

        for (int j = 0; j <= settings.sectorCount; j++)
        {
            float cx = circleVerticies[k];
            float cz = circleVerticies[k + 1];

            vertex vert;
            vert.position[0] = cx * radius;
            vert.position[1] = h;
            vert.position[2] = cz * radius;

            vert.normal[0] = 0.0f;
            vert.normal[1] = ny;   
            vert.normal[2] = 0.0f;

            vert.texCoord[0] = cx * 0.5f + 0.5f;
            vert.texCoord[1] = cz * 0.5f + 0.5f;

            data.verticies.push_back(vert);
            k += 3;
        }

        
        for (int j = 0; j < settings.sectorCount; j++)
        {
           

            if (i == 1) 
            {
                data.indicies.push_back(centerIndex);
                data.indicies.push_back(ringStartIndex + j);
                data.indicies.push_back(ringStartIndex + j + 1);
            }
            else 
            {
                data.indicies.push_back(centerIndex);
                data.indicies.push_back(ringStartIndex + j + 1);
                data.indicies.push_back(ringStartIndex + j);
            }
        }
    }

    return instancedMesh(data, matricies);
}

std::vector<float> plant_generator::UnitCircleVerticies(const cylinderSettings& settings) const
{
    std::vector<float> vertexPos;
   
    vertexPos.reserve((settings.sectorCount + 1) * 3);

    float sectorStep = 2.0f * 3.14159f / settings.sectorCount; 

    for (int i = 0; i <= settings.sectorCount; ++i)
    {
        float sectorAngle = i * sectorStep;
        vertexPos.push_back(std::cos(sectorAngle));
        vertexPos.push_back(std::sin(sectorAngle)); 
        vertexPos.push_back(0.0f);
    }

    return vertexPos;
}
