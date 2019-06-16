#ifndef CAR_RENDERER_H
#define CAR_RENDERER_H

#include <string>
#include "../headers.h"
#include "../game_object/entity.h"
#include "../shaders/shader.h"
#include "../game_object/light.h"
#include "../skyloader/skyLoader.h"
#include "./RendererManager.h"

using namespace std;

class EntityRenderer {
public:
  static void render(
    Shader* shader,
    Entity* entity, 
    bool useVertColor,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::mat4 lightSpaceMatrix,
    glm::vec3 lightPos,
    glm::vec3 viewPos,
    glm::vec3 vertColor);
};

#endif