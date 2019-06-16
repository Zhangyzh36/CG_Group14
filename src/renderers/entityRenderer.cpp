#include "entityRenderer.h"

void EntityRenderer::render(
    Shader* shader,
    Entity* entity,
    bool useVertColor,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::mat4 lightSpaceMatrix,
    glm::vec3 lightPos,
    glm::vec3 viewPos,
    glm::vec3 vertColor) {
  shader->use();
  shader->setBool("useVertColor", useVertColor);
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setMat4("model", model);
  shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
  shader->setVec3("lightPos", lightPos);
  shader->setVec3("viewPos", viewPos);
  shader->setVec3("vertColor", vertColor);
  shader->setFloat("ambient", 1.0f);
  shader->setFloat("diffuse", 1.0f);
  shader->setFloat("specular", 0.45f);
  entity->draw(shader);
  shader->unuse();
} 
