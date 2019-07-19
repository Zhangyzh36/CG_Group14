#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class Rabbit :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	Rabbit(const string& path);
	glm::mat4 getNextPosition(int i) const;
	~Rabbit();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
	vector<glm::vec3> positions;
};

