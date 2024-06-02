/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "myCube.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "MyModels.h"
#include "Camera.h"
#include "Plane.h"

using namespace std;

Camera* kamera = new Camera(glm::vec3(0, 1, 0));
Sky* niebo;
Lamp* lamp1, * lamp2;
Floor* podloga;
ShaderProgram* sp, *spniebo, * splamp;
ObstacleVector obstacleV;
ObstacleVector obstacleV2;
ObstacleVector obstacleV3;
glm::vec3 cubePos(0, 0, 3);
Plane* plane;

double lastx = 999999, lasty = 999999;
float speed_x = 0;
float speed_y = 0;
float speed_z = 0;
float speed_m = 0;
float aspectRatio = 1;





bool loadOBJ(const char* path, std::vector < glm::vec4 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec4 >& out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();

#pragma warning (disable : 4996)
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !!!\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		//pierwsze slowo linii
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec4 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.a = 1.0f;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.a = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec4 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvsIndex = uvIndices[i];
		glm::vec2 uvs = temp_uvs[uvsIndex - 1];
		out_uvs.push_back(uvs);

		unsigned int normalIndex = normalIndices[i];
		glm::vec4 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}


GLuint readTexture(const char* filename) {

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);


	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka


	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);



	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt


	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	return tex;

}


GLuint tex;
GLuint pustynia;
GLuint skyy;
GLuint tex4;
GLuint lampka;
GLuint samolot;
GLuint yellow;
GLuint grey;
GLuint red;
GLuint texPlane;

GLuint colors[4];

GLuint randColor() {
	srand(time(NULL));
	int r = rand() % 4;
	switch (r) {
	case 0:
		return red;
	case 1:
		return yellow;
	case 2:
		return grey;
	}
}


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(
	GLFWwindow* window,	int key,	int scancode,	int action,	int mod) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_Q) {
			speed_x = PI / 2;
		}
		if (key == GLFW_KEY_E) {
			speed_x = -PI / 2;
		}
		if (key == GLFW_KEY_W) speed_y = 15;
		if (key == GLFW_KEY_S) speed_y = -15;
		if (key == GLFW_KEY_SPACE) {
			if (kamera->getOnGround()) {
				kamera->addForce(glm::vec3(0, 5, 0));
				kamera->setOnGround(false);
			}
		}
		if (key == GLFW_KEY_A) speed_m = 15;
		if (key == GLFW_KEY_D) speed_m = -15;
		//legacy
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {

		if (key == GLFW_KEY_Q) {
			speed_x = 0;
		}
		if (key == GLFW_KEY_E) {
			speed_x = 0;
		}
		if (key == GLFW_KEY_W) speed_y = 0;
		if (key == GLFW_KEY_S) speed_y = 0;
		if (key == GLFW_KEY_A) speed_m = 0;
		if (key == GLFW_KEY_D) speed_m = 0;
		if (key == GLFW_KEY_F) {
			if (plane->coll()) {
				switch (kamera->getMode()) {
				case 0:
					kamera->changeMode();
					plane->changeMode();
					break;
				case 1:
					kamera->changeMode();
					plane->changeMode();
					break;
				}
			}
		}

		//legacy code
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (lastx == 999999 && lasty == 999999) {
		kamera->rotateKier(0, 0);
		lastx = xpos;
		lasty = ypos;
		return;
	}
	float xoffset = lastx - xpos;
	float yoffset = ypos - lasty;
	lastx = xpos;
	lasty = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	if (kamera->getVert() >= PI / 2 && yoffset > 0) {
		yoffset = 0;
	}
	if (kamera->getVert() <= -PI / 2 && yoffset < 0) {
		yoffset = 0;
	}
	if (xoffset != 0) {
		if (xoffset > 0) {
			plane->setAnimeAng(-PI / 30);
		}
		else {
			plane->setAnimeAng(PI / 30);
		}
	}
	else {
		plane->setAnimeAng(0);
	}
	kamera->rotateKier(glm::radians(xoffset), glm::radians(yoffset));

}


void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}



//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.53, 0.80, 0.92, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);


	sp = new ShaderProgram("v_obstacle.glsl", NULL, "f_obstacle.glsl");
	spniebo = new ShaderProgram("v_sky.glsl", NULL, "f_sky.glsl");
	splamp = new ShaderProgram("v_lamp.glsl", NULL, "f_lamp.glsl");

	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;
	std::vector< glm::vec4 > temp_vertices2;
	std::vector< glm::vec2 > temp_uvs2;
	std::vector< glm::vec4 > temp_normals2;

	pustynia = readTexture("pustynia.png");
	samolot = readTexture("airplane.png");
	skyy = readTexture("skyy.png");
	tex4 = readTexture("skyyref.png");
	lampka = readTexture("light.png");
	yellow = readTexture("zolty.png");
	grey = readTexture("szary.png");
	red = readTexture("czerwony.png");

	tex = readTexture("stone-wall.png");

	texPlane = readTexture("plane.png");


	colors[0] = red;
	colors[1] = yellow;
	colors[2] = grey;
	colors[3] = tex;




	loadOBJ("square.obj", temp_vertices, temp_uvs, temp_normals);
	cout << "loaded square";
	podloga = new Floor(temp_vertices, temp_normals, temp_uvs, new FloorCollision(glm::vec3(-100, 0, -100), glm::vec3(100, 0, 100)));
	
	loadOBJ("kopula.obj", temp_vertices, temp_uvs, temp_normals);
	cout << "loaded kopula";
	niebo = new Sky(temp_vertices, temp_normals, temp_uvs);

	loadOBJ("nowy.obj", temp_vertices, temp_uvs, temp_normals);
	cout << "loaded nowy";

	lamp1 = new Lamp(temp_vertices, temp_normals, temp_uvs, glm::vec3(1, 20, 1));
	lamp2 = new Lamp(temp_vertices, temp_normals, temp_uvs, glm::vec3(15, 5, 20));


	
	loadOBJ("skyscrapers.obj", temp_vertices, temp_uvs, temp_normals);
	obstacleV.set(temp_vertices, temp_normals, temp_uvs);
	obstacleV.add(glm::vec3(-5, 1, 0), 0);
	obstacleV.add(glm::vec3(10, 1, 10), 0);
	obstacleV.add(glm::vec3(15, 1, -5), 0);

	
	loadOBJ("skyscrapers3.obj", temp_vertices, temp_uvs, temp_normals);
	obstacleV2.set(temp_vertices, temp_normals, temp_uvs);
	obstacleV2.add(glm::vec3(-10, 1, 10), 0);
	obstacleV2.add(glm::vec3(20, 1, 10), 0);
	obstacleV2.add(glm::vec3(-5, 1, 15), 0);
	/*
	loadOBJ("47.obj", temp_vertices, temp_uvs, temp_normals);
	obstacleV3.set(temp_vertices, temp_normals, temp_uvs);
	obstacleV3.add(glm::vec3(-5, 1, -5), 0);
	obstacleV3.add(glm::vec3(-20, 1, -7), 0);
	obstacleV3.add(glm::vec3(-10, 1, 15), 0);
	*/

	loadOBJ("plane.obj", temp_vertices, temp_uvs, temp_normals);
	plane = new Plane(temp_vertices, temp_normals, temp_uvs, kamera, glm::vec3(0, 0, 7) , new PlaneCollision);



	kamera->setRob(plane->PosPtr());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex);
	delete sp;
	delete spniebo;
	delete kamera;
}


void texKostka(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}





//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości




	glm::mat4 V = glm::lookAt(
		kamera->getPos(),
		kamera->getPos() + kamera->getKier(),
		glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 250.0f);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, cubePos);

	niebo->draw(spniebo, P, V, skyy);
	podloga->draw(sp, P, V, pustynia, lamp1->getPos(), lamp2->getPos());
	plane->draw(sp, P, V, red, lamp1->getPos(), lamp2->getPos());

	lamp1->draw(splamp, P, V, lampka);
	lamp2->draw(splamp, P, V, lampka);
	obstacleV.draw(sp, P, V, yellow, lamp1->getPos(), lamp2->getPos());
	obstacleV2.draw(sp, P, V, grey, lamp1->getPos(), lamp2->getPos());
	obstacleV3.draw(sp, P, V, red, lamp1->getPos(), lamp2->getPos());



	
	
	//obstacleV.draw(sp, P, V, samolot, lamp1->getPos(), lamp2->getPos());

	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	/*
	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::rotate(M, angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M = glm::rotate(M, angle_x, glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
	glm::mat4 V = glm::lookAt(
		glm::vec3(0.0f, 1.5f, -1.5f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania


	texKostka(P, V, M);
	
	*/
	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		kamera->rotateKier(speed_x * glfwGetTime(), 0); //obraca kamere o  kat odpowiedni do speeda z inputu
		kamera->setPos(kamera->getPos() + glm::vec3(kamera->getKier().x, 0, kamera->getKier().z) * speed_y * (float)glfwGetTime() + kamera->getPrawo() * speed_m * (float)glfwGetTime() + glm::vec3(0, speed_z * glfwGetTime(), 0)); //ustawia kamere zgodnie ze speedem
		if (!kamera->getOnGround()) {
			kamera->addForce(glm::vec3(0, -0.25, 0));
		}
		kamera->applyForce(glfwGetTime());
		podloga->colli(kamera);
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
