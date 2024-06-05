#include "MyModels.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include <iostream>

float PI = 3.141592;


std::vector<glm::vec4> Model::getVert() {
	return vertices;
}
std::vector<glm::vec4> Model::getNorm() {
	return normals;
}
std::vector<glm::vec2> Model::getUv() {
	return uvs;
}
glm::vec3 Model::getPos() {
	return pos;
}

void Model::SetPos(glm::vec3 v) {
	pos = v;
}




void Sky::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, pos);
	M = glm::translate(M, glm::vec3(0, -1, 0));
	M = glm::scale(M, glm::vec3(100, 50, 100));
	M = glm::scale(M, glm::vec3(1.6, 1.6, 1.6));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}



void Floor::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(0, 0, 0));
	M = glm::scale(M, glm::vec3(300, 1, 300));

	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);


	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void Floor::colli(Camera* c) {
	coll->collAct(c);
}




void Lamp::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	float PI = 3.141592;
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	M = glm::rotate(M, PI, glm::vec3(1, 0, 0));
	M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("TexCoord"));
	glVertexAttribPointer(sp->a("TexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);

	glUniform1i(sp->u("Texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}



void Obstacle::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec4 lp1, glm::vec4 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(3.0, 3.0, 3.0));

	M = glm::translate(M, pos + glm::vec3(0, -1.9, 0));
	M = glm::rotate(M, PI / 2 + angle, glm::vec3(0, 1, 0));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}






void ObstacleVector::add(glm::vec3 pos, float ang) {
	vector.push_back(new Obstacle(vertices, normals, uvs, pos, ang));
}

int ObstacleVector::size() {
	return vector.size();
}

Obstacle* ObstacleVector::operator[] (int i) {
	return vector[i];
}

void ObstacleVector::set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
	vertices = vert;
	normals = norm;
	uvs = uv;
}

void ObstacleVector::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {

	int r = rand() % 4;
	GLuint texture = tex;

	

	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	}
}



glm::mat4 PlaneModel::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, float acc, float rx, float ry, float rz, glm::vec3 pos, float ang, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	
	speed += acc;
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	
	M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	M = glm::rotate(M, -PI/2, glm::vec3(1, 0, 0));
	M = glm::rotate(M, -PI/2, glm::vec3(0, 0, 1));
	M = glm::rotate(M, rx, glm::vec3(1, 0, 0)); // rx
	M = glm::rotate(M, ry, glm::vec3(0, 1, 0)); // ry
	M = glm::rotate(M, rz, glm::vec3(0, 0, 1)); // rz
	//M = glm::translate(M, glm::vec3(-speed, 0, 0));
	M = glm::scale(M, glm::vec3(0.01, 0.01, 0.01));
	
	std::cout << speed << std::endl;
	
	//M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	return M;
}