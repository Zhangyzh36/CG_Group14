#include "RendererManager.h"

Light RendererManager::headlight = Light();
unsigned int RendererManager::depthMap = 0;
void RendererManager::init() {
  headlight.position = glm::vec4(16, 10, 16, 1);
  headlight.specular = glm::vec3(0.8f, 0.8f, 0.4f);
  headlight.diffuse = glm::vec3(0.8f, 0.8f, 0.4f);
  headlight.coneDirection = glm::vec3(0.0f, -1.0f, 0.0f);
  headlight.coneAngle = 3.1415926/4;
  headlight.radius = 10.0f;
}
