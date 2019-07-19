#include "plane.h"

void Plane::initPlaneVAO() {
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void Plane::initTexture() {

	glGenTextures(1, &texture);
	int width, height, nrComponents;
	unsigned char *data = stbi_load("./res/terrain/grass.png", &width, &height, &nrComponents, 0);
	if (data) {
		cout << height << " " << width << endl;
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
}






void Plane::draw(Shader* shader) const {
	shader->setInt("texture_diffuse_1", 98);
	glActiveTexture(GL_TEXTURE0 + 98); 
	glBindTexture(GL_TEXTURE_2D, texture); 
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
glm::mat4 Plane::getModelMat() const {

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10, 1, 10));
	return model;
}

Plane::Plane() {
  initPlaneVAO();
  initTexture();
	this->useVertColor = true;
	this->vertColor = glm::vec3(-1, -1, -1);
}