#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glad/glad.h>

#include "shader.h"

//const char* SKYBOX_VERTEX_SHADER = "shaders/skybox.vs";
//const char* SKYBOX_FRAGMENT_SHADER = "shaders/skybox.fs";

class SkyboxShader : public Shader
{
public:
	SkyboxShader();
	void loadMatrices(glm::mat4 view, glm::mat4 projection);
};

#endif // SKYBOX_SHADER

