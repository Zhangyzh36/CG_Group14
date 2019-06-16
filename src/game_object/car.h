#ifndef CAR_H
#define CAR_H

#include "entity.h"
#include "../model_loader/loader.h"
#include <cmath>

using namespace std;

class Car : public Entity {
public:
  static const float turnAngle;
  Car(const string& path);
  ~Car();
  float a;
  float fa;
  float maxSpeed;
  float speed;
  float angle;
  float originAngle;
  glm::vec3 direction; // unit vector
  ModelLoader* loader = nullptr;
  void move(const float t);
  void speedup(const float t);
  void speeddown(const float t);
  void friction(const float t);
  void draw(Shader* shader = nullptr) const;
  void rotate(const float);
  glm::mat4 getModelMat() const;
};

#endif