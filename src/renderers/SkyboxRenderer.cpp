#pragma once
#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(std::vector<std::string> images, const float SIZE)
{
	this->shader = SkyboxShader();

	std::vector<float> vertices = {
		-SIZE, -SIZE, SIZE,
		SIZE, -SIZE, SIZE,
		SIZE, SIZE, SIZE,
		-SIZE, SIZE, SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, SIZE, -SIZE,
		-SIZE, SIZE, -SIZE
	};

	std::vector<unsigned int> indices = {
		0,1,2, 2,3,0,
		1,5,6, 6,2,1,
		5,4,7, 7,6,5,
		4,0,3, 3,7,4,
		3,2,6, 6,7,3,
		4,5,1, 1,0,4
	};

	vao = Loader::getLoader()->loadVAO(vertices, indices);
	indexCount = indices.size();
	texture = Loader::getLoader()->loadCubemapTexture(images);
}

void SkyboxRenderer::render(glm::mat4 view, glm::mat4 projection) {
	glDisable(GL_CULL_FACE);
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	shader.loadMatrices(view, projection);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader.unuse();
	glEnable(GL_CULL_FACE);
}

GLuint SkyboxRenderer::getSkyboxTexture() {
	return texture;
}