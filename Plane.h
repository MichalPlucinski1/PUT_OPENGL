#pragma once

#include "MyModels.h"
#include "Camera.h"
#include "Collisions.h"
#include <iostream>
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
	float rx = 0;
	float ry = 0;
	float rz = 0;




public:


	glm::vec3 tmpx;
	glm::vec3 tmpy;
	glm::vec3 tmpz;


	Plane(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, PlaneCollision* pc);  //konstruktor
	~Plane() {
		delete colli;
		delete planeModel;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V,
		float speed, float rx, float ry, float rz,
		GLuint tex, glm::vec3 lp1, glm::vec3 lp2);  //dopiero kiedy rysowanie jest tutaj to dziala
	void changeMode();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
	void rotate(float ax, float ay, float az);
	glm::vec3* PosPtr() { return &pos; }
	void setAnimeAng(float a);
	bool coll();
	void outPos();

};

