#include <vector>
#include <shader.hpp>


class skybox
{
public:
	skybox(const std::vector<std::string>& boxFaces);
	void draw(shader& shad);
	~skybox();
private:
	unsigned int VAO, VBO, EBO;
	unsigned int texture;
	void SetupSkybox();
	

};