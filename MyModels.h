#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "Collisions.h"







class Model {
protected:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs; // wspolrzedne teksturowania ??chyba
	glm::vec3 pos;
public:
	Model(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
		vertices = vert;
		normals = norm;
		uvs = uv;
		pos = glm::vec3(0, 0, 0);
	}
	std::vector<glm::vec4> getVert();
	std::vector<glm::vec4> getNorm();
	std::vector<glm::vec2> getUv();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
};



class Sky : public Model {
public:
	Sky(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) :Model(vert, norm, uv) {}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex);
};




class Floor : public Model {
private:
	FloorCollision* coll;
public:
	Floor(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, FloorCollision* c) : Model(vert, norm, uv) {
		coll = c;
	}
	~Floor() {
		delete coll;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);
	void colli(Camera* c);
};



class Lamp : public Model {
public:
	Lamp(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, glm::vec3 p) : Model(vert, norm, uv) {
		pos = p;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex);
};


