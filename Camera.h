#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//jakies potrzebne rzeczy do kamerki

class Camera {
private:
	glm::vec3 position;
	glm::vec3 kierunek; //kierunek patrzenia
	glm::vec3 prawo;  //kierunek w prawo od kamery
	float ang;
	int mode;
	glm::vec3* robpos;
	glm::vec3 acceleration;
	bool onGround;
	float angvert;
public:
	Camera(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec3 getKier();
	glm::vec3 getPrawo();
	void setPos(glm::vec3 v);
	void rotateKier(float angx, float angy);
	float getAng();
	void changeMode();
	int getMode() { return mode; }
	void setRob(glm::vec3* rob) { robpos = rob; }
	void addForce(glm::vec3 f) {
		acceleration += f;
		if (acceleration.length() > 20) {
			acceleration = glm::normalize(acceleration) * 10.0f;
		}
	}
	void applyForce(float time) {
		position += acceleration * time;
	}
	void setOnGround(bool a) {
		onGround = a;
	}
	bool getOnGround() {
		return onGround;
	}
	void zeroForce() {
		acceleration = glm::vec3(0, 0, 0);
	}
	float getVert() {
		return angvert;
	}
};
