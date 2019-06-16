#include "SkyboxShader.h"

SkyboxShader::SkyboxShader() :Shader("shaders/skybox.vs", "shaders/skybox.fs")
{

}

void SkyboxShader::loadMatrices(glm::mat4 view, glm::mat4 projection)
{
	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;

	setMat4("view", view);
	setMat4("projection", projection);
}