#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../shaders/shader.h"

using std::vector;
using std::string;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 dcolor;
	glm::vec3 scolor;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, 
		vector<unsigned int> indices, 
		vector<Texture> textures,
		glm::vec3 dcolor = glm::vec3(-1.0f));
	void draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};


#endif