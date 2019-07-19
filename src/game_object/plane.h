#ifndef PLANE_H
#define PLANE_H

#include "./entity.h"
#include "../stb_image/stb_image.h" 
#include "../renderers/RendererManager.h" 

class Plane: public Entity {
public:
  float planeVertices[48 *6] = {
	  -25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f, 25.0f,  0.0f, // Bottom-left

	  25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,// top-right

	  25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f, 0.0f, 25.0f, // bottom-right         

	  25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f,  25.0f,  0.0f, // top-right

	  -25.f, -0.5f, -25.f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,// bottom-left

	  -25.f, 0.5f, -25.f, 0.0f, 0.0f, -1.0f,0.0f, 25.0f, // top-left



	  -25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f, 25.0f,  0.0f, // bottom-left

	  25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,// bottom-right

	  25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f,  0.0f, 25.0f, // top-right

	  25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f, 25.0f,  0.0f, // top-right

	  -25.f, 0.5f, 25.f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,// top-left

	  -25.f, -0.5f, 25.f, 0.0f, 0.0f, 1.0f,  0.0f, 25.0f, // bottom-left



	  -25.f, 0.5f, 25.f, -1.0f, 0.0f, 0.0f, 25.0f,  0.0f, // top-right

	  -25.f, 0.5f, -25.f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,// top-left

	  -25.f, -0.5f, -25.f, -1.0f, 0.0f, 0.0f, 0.0f, 25.0f,  // bottom-left

	  -25.f, -0.5f, -25.f, -1.0f, 0.0f, 0.0f, 25.0f,  0.0f, // bottom-left

	  -25.f, -0.5f, 25.f, -1.0f, 0.0f, 0.0f,  0.0f,  0.0f,// bottom-right

	  -25.f, 0.5f, 25.f, -1.0f, 0.0f, 0.0f, 0.0f, 25.0f, // top-right



	  25.f, 0.5f, 25.f, 1.0f, 0.0f, 0.0f, 25.0f,  0.0f, // top-left

	  25.f, -0.5f, -25.f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,// bottom-right

	  25.f, 0.5f, -25.f, 1.0f, 0.0f, 0.0f, 0.0f, 25.0f, // top-right         

	  25.f, -0.5f, -25.f, 1.0f, 0.0f, 0.0f,  25.0f,  0.0f, // bottom-right

	  25.f, 0.5f, 25.f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,// top-left

	  25.f, -0.5f, 25.f, 1.0f, 0.0f, 0.0f,0.0f, 25.0f,  // bottom-left     



	  -25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f, 25.0f,  0.0f, // top-right

	  25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f,// top-left

	  25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f,0.0f, 25.0f, // bottom-left

	  25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f, 25.0f,  0.0f, // bottom-left

	  -25.f, -0.5f, 25.f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f,// bottom-right

	  -25.f, -0.5f, -25.f, 0.0f, -1.0f, 0.0f,0.0f, 25.0f,  // top-right



	  -25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f,25.0f,  0.0f, // top-left

	  25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,// bottom-right

	  25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f,0.0f, 25.0f,  // top-right     

	  25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f, 25.0f,  0.0f, // bottom-right

	  -25.f, 0.5f, -25.f, 0.0f, 1.0f, 0.0f,0.0f,  0.0f,// top-left

	  -25.f, 0.5f, 25.f, 0.0f, 1.0f, 0.0f ,0.0f, 25.0f // bottom-left 
  };
  unsigned int planeVAO, texture;
  void initPlaneVAO();
  void initTexture();
  Plane();
  glm::mat4 getModelMat()  const;
  void draw(Shader* shader = nullptr) const;
};

#endif