#include "Tree.h"

Tree::Tree(const string& path)
{
	loader = new ModelLoader(path.c_str());
	position = glm::vec3(-60, 7.5, -1200);
}


Tree::~Tree()
{
	delete loader;
}

void Tree::draw(Shader* shader) const {
	loader->draw(*shader);
}


glm::mat4 Tree::getModelMat() const {
	glm::mat4 model = glm::mat4(1.0f);
	float originAngle = -90;
	//model = glm::rotate(model, glm::radians(originAngle), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	model = glm::translate(model, position);

	return model;
}
