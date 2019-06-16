#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class City :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	City(const string& path);
	~City();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
};

