#ifndef RENDERER_MANAGER_H
#define RENDERER_MANAGER_H

#include <vector>
#include "../game_object/light.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace std;

class RendererManager {
public:
  // RendererManager();
  //vector<DefaultRenderer*> manager;
  static Light headlight;
  static unsigned int depthMap;
  static void init();
};

#endif