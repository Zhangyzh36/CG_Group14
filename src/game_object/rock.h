#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class Rock :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	Rock(const string& path);
	~Rock();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
	glm::mat4 getNextPosition(int i) const;
	vector<glm::vec3> positions;
	vector<float> sizes;
};

