#include "fence.h"

Fence::Fence(const string& path)
{
	loader = new ModelLoader(path.c_str());
	this->objNum = 20;
	positions.push_back(glm::vec3(4, 0.5, -101));
	positions.push_back(glm::vec3(5.5, 0.5, -101));
	positions.push_back(glm::vec3(7, 0.5, -101));
	positions.push_back(glm::vec3(8.5, 0.5, -101));
	positions.push_back(glm::vec3(10, 0.5, -101));

	positions.push_back(glm::vec3(4, 0.5, -93));
	positions.push_back(glm::vec3(5.5, 0.5, -93));
	positions.push_back(glm::vec3(7, 0.5, -93));
	positions.push_back(glm::vec3(8.5, 0.5, -93));
	positions.push_back(glm::vec3(10, 0.5, -93));

	positions.push_back(glm::vec3(4 + 90, 0.5, 3));
	positions.push_back(glm::vec3(5.5 + 90, 0.5, 3));
	positions.push_back(glm::vec3(7 + 90, 0.5, 3));
	positions.push_back(glm::vec3(8.5 + 90, 0.5, 3));
	positions.push_back(glm::vec3(10 + 90, 0.5, 3));

	positions.push_back(glm::vec3(4 + 90, 0.5, 11));
	positions.push_back(glm::vec3(5.5 + 90, 0.5, 11));
	positions.push_back(glm::vec3(7 + 90, 0.5, 11));
	positions.push_back(glm::vec3(8.5 + 90, 0.5, 11));
	positions.push_back(glm::vec3(10 + 90, 0.5, 11));
}

glm::mat4 Fence::getNextPosition(int i) const
{
	glm::mat4 model = glm::mat4(1.0f);
	if (i >= 10)
	{
		float angle = 90;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
	}
	model = glm::translate(model, positions[i]);
	
	return model;
}


Fence::~Fence()
{
	delete loader;
}

void Fence::draw(Shader* shader) const {
	loader->draw(*shader);
}


glm::mat4 Fence::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	return model;
}
