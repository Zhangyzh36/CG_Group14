#pragma once
#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glad/glad.h>

#include "../shaders/SkyboxShader.h"
#include "../skyloader/skyLoader.h"

class SkyboxRenderer
{
public:
	SkyboxRenderer(std::vector<std::string> images, const float SIZE);
	unsigned int getSkyboxTexture();
	void render(glm::mat4 view, glm::mat4);

private:
	SkyboxShader shader;
	unsigned int vao;
	unsigned int texture;
	int indexCount;
};


#endif