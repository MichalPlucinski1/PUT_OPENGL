#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"


float dist(glm::vec3 a, glm::vec3 b);

class Collision {
protected:
	glm::vec3 leftdown, rightup;
public:
	Collision(glm::vec3 a, glm::vec3 b) {
		leftdown = a;
		rightup = b;
	}
	virtual void collAct(Camera* c) = 0;
};

class FloorCollision : public Collision {
public:
	FloorCollision(glm::vec3 a, glm::vec3 b) : Collision(a, b) {

	}
	void collAct(Camera* c);
};



