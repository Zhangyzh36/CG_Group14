#ifndef LIGHT_H
#define LIGHT_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <glad/glad.h>
#include <glm/ext.hpp>
const double PI = 3.14;

// Lights are a simple struct holding all of the relevant information.
struct Light {
  Light(): 
    position(glm::vec4(0.0f)),
    specular(glm::vec3(0.0f)),
    diffuse(glm::vec3(0.0f)),
    ambient(glm::vec3(0.0f)),
    radius(0.0f),
    coneAngle((float)PI),
    coneDirection(glm::vec3(0.0f)){
  }

  glm::vec4 position; // Position of the camera in world coordinates. w value of 0 indicates directional light, 1 indicates positional/point light
  glm::vec3 specular; // Colour intensities of each light component
  glm::vec3 diffuse;
  glm::vec3 ambient;
  float radius;           // Radius used for attenuation
  float coneAngle;        // Angle and direction of cone for spotlights
  glm::vec3 coneDirection;
};

#endif
