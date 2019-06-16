#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include "../shaders/shader.h"

using namespace std;

class Entity {
public:
  glm::vec3 position = glm::vec3(0, 0, 0);
  bool useVertColor = false;
  glm::vec3 vertColor = glm::vec3(0, 0, 0);

  glm::vec3 velocity = glm::vec3(0, 0, 0);
  virtual void draw(Shader* shader = nullptr) const = 0;
  virtual glm::mat4 getModelMat() const;
};

#endif