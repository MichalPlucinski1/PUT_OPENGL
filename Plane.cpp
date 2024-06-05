#include "Plane.h"

const float PI = 3.141592653589793f;




Plane::Plane(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, PlaneCollision* pc) {
	kier = glm::vec3(0, 0, 1);
	kamera = kam;
	planeModel = new PlaneModel(vert, norm, uv);

	pos = posi;
	animeang = 0;
	colli = pc;
	rx = 0;
	ry = 0;
	rz = 0;

	tmpx = glm::vec3(1, 0, 0);
	tmpy = glm::vec3(0, 1, 0);
	tmpz = glm::vec3(0, 0, 1);
}



void Plane::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, float speed, float rx, float ry, float rz, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) { //dopiero kiedy rysowanie jest tutaj to dziala
		glm::mat4 M = planeModel->draw(sp, P, V,speed, rx, ry, rz, pos, 0, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
}

void Plane::changeMode() {
	switch (kameraMode) {
	case 0:
		kameraMode = 1;
		break;
	case 1:
		kameraMode = 0;
		break;
	}
}

glm::vec3 Plane::getPos() {
	return pos;
}
void Plane::SetPos(glm::vec3 v) {
	pos = v;
}


void Plane::outPos() {
	std::cout << "x:" << pos.x << " y:" << pos.y << " z:" << pos.z << "\n";
}

void Plane::outRot() {
	std::cout << "x:" << tmpx.x + tmpy.x + tmpz.x << " y:" << tmpz.y << " z:" << tmpz.z << "\n";
}

void Plane::rotate(float ax, float ay, float az) {
	rx += ax;
	ry += ay;
	rz += az;
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::rotate(M, ay, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 tmp = M * glm::vec4(tmpy, 0);
	tmpx.x = tmp.x;
	tmpx.y = tmp.y;
	tmpx.z = tmp.z;


	
	M = glm::rotate(M, az, glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = M * glm::vec4(tmpx, 0);
	tmpy.x = tmp.x;
	tmpy.y = tmp.y;
	tmpy.z = tmp.z;

	M = glm::rotate(M, ax, glm::vec3(0.0f, 0.0f, 1.0f));
	tmp = M * glm::vec4(tmpz, 0);
	tmpz.x = tmp.x;
	tmpz.y = tmp.y;
	tmpz.z = tmp.z;

	

	/*
	M = glm::rotate(M, ax, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec4 tmp = M * glm::vec4(tmpx, 0);
	tmpx.x = tmp.x;
	tmpx.y = tmp.y;
	tmpx.z = tmp.z;
	M = glm::rotate(M, ay, tmpx);
	tmp = M * glm::vec4(tmpy, 0);
	tmpy.x = tmp.x;
	tmpy.y = tmp.y;
	tmpy.z = tmp.z;
	M = glm::rotate(M, az, tmpz);
	tmp = M * glm::vec4(tmpz, 0);

	tmpz.x = tmp.x;
	tmpz.y = tmp.y;
	tmpz.z = tmp.z;
	
	*/

}

void Plane::setAnimeAng(float a) {
	animeang = a;
}

bool Plane::coll() {
	return colli->collAct(kamera, pos);
}
