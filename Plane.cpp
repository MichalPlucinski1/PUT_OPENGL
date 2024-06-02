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