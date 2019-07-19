#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class Forest :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	Forest(const string& path);
	glm::mat4 getNextPosition(int i) const;
	~Forest();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
	vector<glm::vec3> positions;
};

