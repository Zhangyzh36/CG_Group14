#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "../tiny_obj_loader/tiny_obj_loader.h"
#include <glad/glad.h>

#include <vector>
#include <cfloat>
#include <utility>
#include <algorithm>
#include <iostream>

void initMaterial(tinyobj::material_t &material);

class ModelComponent {
private:
	GLuint vaoID;
	int indexCount;
	tinyobj::material_t material;

public:
	GLuint textureID;
	ModelComponent(GLuint, int, GLuint, tinyobj::material_t);
	ModelComponent(GLuint, int, GLuint);
	ModelComponent();

	int getIndexCount() const;
	GLuint getVaoID() const;
	GLuint getTextureID() const;
	tinyobj::material_t getMaterial() const;
};


class Model {
private:
	std::vector<ModelComponent> components;
	std::vector<float> maxRanges;
public:
	Model(std::vector<ModelComponent>);
	Model();
	void addModelComponent(ModelComponent);
	std::vector<ModelComponent>* getModelComponents();

	void addRange(std::vector<float> vertices);
	std::pair<float, float> getRangeInDim(int dim);
};


#endif