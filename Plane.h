#pragma once

#include "MyModels.h"
#include "Camera.h"
#include "Collisions.h"
#include <glm/gtc/type_ptr.hpp>


class Plane
{

private:
	glm::vec3 kier;
	Camera* kamera;
	int kameraMode = 0;
	PlaneModel* planeModel;
	glm::vec3 pos;
	float animeang;
	PlaneCollision* colli;


public:
	Plane(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, PlaneCollision* pc);  //konstruktor
	~Plane() {
		delete colli;
		delete planeModel;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);  //dopiero kiedy rysowanie jest tutaj to dziala
	void changeMode();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
	glm::vec3* PosPtr() { return &pos; }
	void setAnimeAng(float a);
	bool coll();

};

