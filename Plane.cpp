#include "Plane.h"

const float PI = 3.141592653589793f;




Plane::Plane(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, PlaneCollision* pc) {
	kier = glm::vec3(0, 0, 1);
	kamera = kam;
	planeModel = new PlaneModel(vert, norm, uv);

	pos = posi;
	animeang = 0;
	colli = pc;
}



void Plane::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) { //dopiero kiedy rysowanie jest tutaj to dziala
	if (kameraMode == 1) {
		pos = kamera->getPos() + glm::vec3(0, 3, 0);
		glm::mat4 M = planeModel->draw(sp, P, V, pos, kamera->getAng() + PI / 2, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));

	}
	else {
		glm::mat4 M = planeModel->draw(sp, P, V, pos, 0, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));

	}
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

void Plane::setAnimeAng(float a) {
	animeang = a;
}

bool Plane::coll() {
	return colli->collAct(kamera, pos);
}