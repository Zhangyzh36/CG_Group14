#pragma once
#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>
class Tree :
	public Entity
{
public:
	ModelLoader * loader = nullptr;
	Tree(const string& path);
	~Tree();
	void draw(Shader* shader = nullptr) const;
	glm::mat4 getModelMat() const;
};

