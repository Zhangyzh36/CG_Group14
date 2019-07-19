#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class Fence :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	Fence(const string& path);
	glm::mat4 getNextPosition(int i) const;
	~Fence();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
	vector<glm::vec3> positions;
};

