#include "Rock.h"

Rock::Rock(const string& path)
{
	loader = new ModelLoader(path.c_str());
	position = glm::vec3(8, 0.5, 20);
	this->objNum = 20;
	for (int i = 0; i < objNum; i++)
	{
		int x = rand() % 500 - 250;
		int y = rand() % 500 - 250;
		position = glm::vec3(x, 0.5, y);
		positions.push_back(glm::vec3(x, 0.5, y));
		float size = rand() / (double)(RAND_MAX)+0.5;
		sizes.push_back(size);
	}
}


Rock::~Rock()
{
	delete loader;
}

void Rock::draw(Shader* shader) const {
	loader->draw(*shader);
}

glm::mat4 Rock::getNextPosition(int i) const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, positions[i]);
	model = glm::scale(model, glm::vec3(sizes[i], sizes[i], sizes[i]));
	return model;
}


glm::mat4 Rock::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	return model;
}
