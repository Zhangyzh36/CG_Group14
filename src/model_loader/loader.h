#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "../mesh/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>	
#include "../stb_image/stb_image.h"

using namespace std;

class ModelLoader {
public:
	ModelLoader(const char *path);
	void draw(Shader shader);
private:
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int textureFromFile(const char *path, const string &directory, bool gamma = false);
};

#endif