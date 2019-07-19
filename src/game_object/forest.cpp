#include "forest.h"

Forest::Forest(const string& path)
{
	loader = new ModelLoader(path.c_str());
	position = glm::vec3(40, 0.5, -200);
	this->objNum = 3;
	positions.push_back(glm::vec3(150, 0.5, -200));
	positions.push_back(glm::vec3(-20, 0.5, -200));
	positions.push_back(glm::vec3(20, 0.5, -200));
	positions.push_back(glm::vec3(-150, 0.5, -200));
}

glm::mat4 Forest::getNextPosition(int i) const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, positions[i]);
	return model;
}


Forest::~Forest()
{
	delete loader;
}

void Forest::draw(Shader* shader) const {
	loader->draw(*shader);
}


glm::mat4 Forest::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	return model;
}
