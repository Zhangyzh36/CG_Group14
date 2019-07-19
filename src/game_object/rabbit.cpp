#include "rabbit.h"

Rabbit::Rabbit(const string& path)
{
	loader = new ModelLoader(path.c_str());
	position = glm::vec3(8, 0.5, 20);
	this->objNum = 3;
	positions.push_back(glm::vec3(4, 0.5, -98));
	positions.push_back(glm::vec3(7, 0.5, -96));
	positions.push_back(glm::vec3(8, 0.5, -94));
}


Rabbit::~Rabbit()
{
	delete loader;
}

void Rabbit::draw(Shader* shader) const {
	loader->draw(*shader);
}

glm::mat4 Rabbit::getNextPosition(int i) const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, positions[i]);
	return model;
}


glm::mat4 Rabbit::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	return model;
}
