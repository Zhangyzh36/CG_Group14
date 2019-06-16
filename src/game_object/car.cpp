#include "car.h"

const float Car::turnAngle = 0.5f;

void Car::move(const float t) {
  originAngle = angle;
  position += speed * t * direction;
}

void Car::speedup(const float t) {
	if (speed < abs(this->maxSpeed))
	{
		if (speed >= 0)
		{
			speed += (a - fa) * t;
		}
		else
		{
			speed += (a + fa) * t;
		}
	}
}

void Car::speeddown(const float t) {
	if (speed < abs(this->maxSpeed))
	{
		if (speed < 0)
		{
			speed += (-a + fa) * t;
		}
		else
		{
			speed += (-a - fa) * t;
		}
	}
}

void Car::friction(const float t)
{
	if (speed > 0)
	{
		speed -= fa * t;
		speed = speed < 0 ? 0 : speed;
	}
	else if(speed < 0)
	{
		speed += fa * t;
		speed = speed < 0 ? 0 : speed;
	}
}

Car::Car(const string& path) {
  speed = 0.f;
  a = 1.06f;
  fa = 0.005f;
  maxSpeed = 20.f;
  direction = glm::vec3(-1, 0, 0);
  angle = 0.0;
  originAngle = 0.0f;
  loader = new ModelLoader(path.c_str());
  position = glm::vec3(0, -0.5, 0);
}

Car::~Car() {
  delete loader;
}

void Car::draw(Shader* shader) const {
  loader->draw(*shader);
}

void Car::rotate(const float offset) {
  angle = (angle > 360) ? angle - 360 : angle;
  angle = (angle < -360) ? angle + 360 : angle;
  if (abs(angle - originAngle) <= 22.5 || abs(angle + offset - originAngle) <= 22.5) {
    angle += offset;
  }
  glm::vec3 rot = glm::vec3(-cos(glm::radians(angle)), 0, sin(glm::radians(angle)));
  direction = rot;
}

glm::mat4 Car::getModelMat() const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
	// // model = glm::translate(model, glm::vec3(0, 1, 0));
  model = glm::rotate(model, glm::radians(originAngle), glm::vec3(0, 1, 0));
	return model;
}