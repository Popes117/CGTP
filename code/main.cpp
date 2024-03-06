#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tinyxml2-master/tinyxml2.h"



#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <tuple>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
using namespace std;

void processWindowElement(tinyxml2::XMLElement* windowElement) {
	int width, height;
	windowElement->QueryIntAttribute("width", &width);
	windowElement->QueryIntAttribute("height", &height);

	// Teste a imprimir para a consola a ver se leu bem
	std::cout << "Window: Width = " << width << ", Height = " << height << std::endl;
}

void processCameraElement(tinyxml2::XMLElement* cameraElement) {
	float px, py, pz, lx, ly, lz, ux, uy, uz, fov, near, far;

	for (tinyxml2::XMLElement* child = cameraElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
		const char* childName = child->Name();

		if (strcmp(childName, "position") == 0) {
			child->QueryFloatAttribute("x", &px);
			child->QueryFloatAttribute("y", &py);
			child->QueryFloatAttribute("z", &pz);
		}
		else if (strcmp(childName, "lookAt") == 0) {
			child->QueryFloatAttribute("x", &lx);
			child->QueryFloatAttribute("y", &ly);
			child->QueryFloatAttribute("z", &lz);
		}
		else if (strcmp(childName, "up") == 0) {
			child->QueryFloatAttribute("x", &ux);
			child->QueryFloatAttribute("y", &uy);
			child->QueryFloatAttribute("z", &uz);
		}
		else if (strcmp(childName, "projection") == 0) {
			child->QueryFloatAttribute("fov", &fov);
			child->QueryFloatAttribute("near", &near);
			child->QueryFloatAttribute("far", &far);
		}
	}
	// Teste a imprimir para a consola a ver se leu bem
	std::cout << "Camera: Position(" << px << ", " << py << ", " << pz << "), ";
	std::cout << "LookAt(" << lx << ", " << ly << ", " << lz << "), ";
	std::cout << "Up(" << ux << ", " << uy << ", " << uz << "), ";
	std::cout << "Projection(FOV=" << fov << ", Near=" << near << ", Far=" << far << ")" << std::endl;
}

void processModelElement(tinyxml2::XMLElement* modelElement) {
	const char* file = modelElement->Attribute("file");
	if (file) {
		// nao sei o que fazer com isto
		std::cout << "Model: File = " << file << std::endl;
	}
	else {
		std::cerr << "Model element is missing the 'file' attribute." << std::endl;
	}
}

void processModelsElement(tinyxml2::XMLElement* modelsElement) {
	for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
		processModelElement(modelElement);
	}
}

void processGroupElement(tinyxml2::XMLElement* groupElement) {
	for (tinyxml2::XMLElement* child = groupElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
		const char* childName = child->Name();

		if (strcmp(childName, "models") == 0) {
			processModelsElement(child);
		}
		// Adicione mais condições conforme necessário para outros tipos de elementos dentro de 'group'
	}
}

void processWorldElement(tinyxml2::XMLElement* worldElement) {
	for (tinyxml2::XMLElement* child = worldElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
		const char* childName = child->Name();

		if (strcmp(childName, "window") == 0) {
			processWindowElement(child);
		}
		else if (strcmp(childName, "camera") == 0) {
			processCameraElement(child);
		}
		else if (strcmp(childName, "group") == 0) {
			processGroupElement(child);
		}
		// Falta light e transform para o futuro
	}
}

int parsexml() {
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("config.xml") != tinyxml2::XML_SUCCESS) {
		std::cerr << "Erro ao carregar o arquivo XML." << std::endl;
		return 1;
	}

	tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
	if (!worldElement) {
		std::cerr << "Elemento 'world' não encontrado." << std::endl;
		return 1;
	}

	processWorldElement(worldElement);

	return 0;
}



void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


/*----------------------------------------------------------------------------------- 
	Draw Cylinder with strips and fans

	  parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder0(float radius, float height, int sides) {

	int i;
	float step;

	step = 360.0/sides;

	glColor3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,-height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,0,1);
	glBegin(GL_TRIANGLE_STRIP);

		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();
}


/*-----------------------------------------------------------------------------------
	Draw Cylinder

		parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cone(float radius, float height, int slices,int stacks) {

	int i;
	float step;
	float n_stacks;
	
	step = 360.0 / slices;
	n_stacks = height/stacks;

	glBegin(GL_TRIANGLES);

	// bottom
	for (i = 0; i < slices; i++) {
		glVertex3f(0, 0, 0);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, 0, -sin((i + 1) * step *M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, 0, -sin(i * step *M_PI / 180.0)*radius);
	}

	// body
	for (i = 0; i <= slices; i++) {
		
		//Triângulos da primeira stack
		glVertex3f(0.0f,height,0.0f);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin(i * step *M_PI / 180.0)*radius/stacks);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin((i + 1) * step *M_PI / 180.0)*radius/stacks);
		
		for(int j = 1; j < stacks; j++){

			//Triângulo da metade da esquerda
			glVertex3f(cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j));
			glVertex3f(cos(i * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin(i * step *M_PI / 180.0)*((radius/stacks)*(j+1)));
			glVertex3f(cos((i + 1) * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin((i + 1) * step *M_PI / 180.0)*((radius/stacks)*(j+1)));	
			
			//Triângulo da metade da direita
			glVertex3f(cos((i+1) * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin((i+1) * step *M_PI / 180.0)*((radius/stacks)*j));
			glVertex3f(cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j));
			glVertex3f(cos((i + 1) * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin((i + 1) * step *M_PI / 180.0)*((radius/stacks)*(j+1)));	
		}
	}
	glEnd();
}

void sphere(float radius, int slices, int stacks) {

	int i;
	float step;
	float n_stacks;
	tuple<float, float, float>** matrix = new tuple<float, float, float>* [slices];
	for (int i = 0; i < slices; ++i)
		matrix[i] = new tuple<float, float, float>[stacks];


	step = 360.0 / slices; 
	n_stacks = radius / stacks; 

	glBegin(GL_TRIANGLES);

	// body
	for (i = 0; i <= slices; i++) {

		/*for (int j = 1; j < stacks; j++) {

			//Triângulo da metade da esquerda
			glVertex3f(cos(i * step * M_PI / 180.0) * ((radius / stacks) * j), radius - n_stacks * j, -sin(i * step * M_PI / 180.0) * ((radius / stacks) * j));
			glVertex3f(cos(i * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin(i * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));
			glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));

			//Triângulo da metade da direita
			glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * j), radius - n_stacks * j, -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * j));
			glVertex3f(cos(i * step * M_PI / 180.0) * ((radius / stacks) * j), radius - n_stacks * j, -sin(i * step * M_PI / 180.0) * ((radius / stacks) * j));
			glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));
		}*/
	}
	for (i = 0; i < stacks; i++) {
		float lat = 180/stacks * i;
		for (int j = 0; j < slices; j++) {
			float lon = j * step;
			float x = radius * sin(lat) * cos(lon);
			float z = radius * sin(lat) * sin(lon);
			float y = radius * cos(lat);
			matrix[i][j] = make_tuple(x, y, z);
		}

	}
	for (i = 0; i < stacks-1; i++) {
		glBegin(GL_TRIANGLES);
		for (int j = 1; j < slices-1; j++) {

			//Triângulo da metade da esquerda
			glVertex3f(get<0>(matrix[i][j]), get<1>(matrix[i][j]), get<2>(matrix[i][j]));
			//glVertex3f(get<0>(matrix[i][j+1]), get<1>(matrix[i][j+1]), get<2>(matrix[i][j+1]));
			//glVertex3f(get<0>(matrix[i+1][j]), get<1>(matrix[i+1][j]), get<2>(matrix[i+1][j+1]));

			//glVertex3f(cos(i * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin(i * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));
			//glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));

			//Triângulo da metade da direita
			glVertex3f(get<0>(matrix[i+1][j]), get<1>(matrix[i+1][j]), get<2>(matrix[i+1][j]));
			glVertex3f(get<0>(matrix[i][j+1]), get<1>(matrix[i][j+1]), get<2>(matrix[i][j+1]));
			//glVertex3f(get<0>(matrix[i][j]), get<1>(matrix[i][j]), get<2>(matrix[i][j]));
			//glVertex3f(get<0>(matrix[i+1][j]), get<1>(matrix[i][j+1]), get<2>(matrix[i+1][j+1]));
			
			//glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * j), radius - n_stacks * j, -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * j));
			//glVertex3f(cos(i * step * M_PI / 180.0) * ((radius / stacks) * j), radius - n_stacks * j, -sin(i * step * M_PI / 180.0) * ((radius / stacks) * j));
			//glVertex3f(cos((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)), radius - n_stacks * (j + 1), -sin((i + 1) * step * M_PI / 180.0) * ((radius / stacks) * (j + 1)));
		}
		glEnd();
	}
	for (int i = 0; i < slices; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

void plane(int length, int divisions){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	glBegin(GL_TRIANGLES);

	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, 0.0f, z); 
			glVertex3f(x, 0.0f, z + lado_length); 
			glVertex3f(x + lado_length, 0.0f, z + lado_length); 

			glVertex3f(x + lado_length, 0.0f, z + lado_length); 
			glVertex3f(x + lado_length, 0.0f, z); 
			glVertex3f(x, 0.0f, z); 

			x += lado_length;
			count += 1;
		}
		z += lado_length;
	}
	glEnd();
}

void box(int length, int divisions){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	float y = -((float)length / 2.0f);
	glBegin(GL_TRIANGLES);

	//Base de baixo
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x, y, z + lado_length); 
			glVertex3f(x + lado_length, y, z + lado_length); 

			glVertex3f(x + lado_length, y, z + lado_length); 
			glVertex3f(x + lado_length, y, z); 
			glVertex3f(x, y, z); 

			x += lado_length;
			count += 1;
		}
		z += lado_length;
	}

	//Base de cima
	y = (float)length / 2.0f;
	z = -((float)length / 2.0f);
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x, y, z + lado_length); 
			glVertex3f(x + lado_length, y, z + lado_length); 

			glVertex3f(x + lado_length, y, z + lado_length); 
			glVertex3f(x + lado_length, y, z); 
			glVertex3f(x, y, z); 

			x += lado_length;
			count += 1;
		}
		z += lado_length;
	}

	// Bases Laterais de acordo com o eixo Z
	float x = -((float)length / 2.0f);
	y = -((float)length / 2.0f);
	for(int i = 0; i < divisions; i++){
		float z = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x, y, z + lado_length); 
			glVertex3f(x, y + lado_length, z + lado_length); 

			glVertex3f(x, y + lado_length, z + lado_length); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x, y, z); 

			z += lado_length;
			count += 1;
		}
		y += lado_length;
	}

	x = (float)length / 2.0f;
	y = -((float)length / 2.0f);
	for(int i = 0; i < divisions; i++){
		float z = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x, y, z + lado_length); 
			glVertex3f(x, y + lado_length, z + lado_length); 

			glVertex3f(x, y + lado_length, z + lado_length); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x, y, z); 

			z += lado_length;
			count += 1;
		}
		y += lado_length;
	}

	// Bases Laterais de acordo com o eixo X
	z = -((float)length / 2.0f);
	y = -((float)length / 2.0f);
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x + lado_length, y, z); 
			glVertex3f(x + lado_length, y + lado_length, z); 

			glVertex3f(x + lado_length, y + lado_length, z); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x, y, z); 

			x += lado_length;
			count += 1;
		}
		y += lado_length;
	}

	z = (float)length / 2.0f;
	y = -((float)length / 2.0f);
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x, y, z); 
			glVertex3f(x + lado_length, y, z); 
			glVertex3f(x + lado_length, y + lado_length, z); 

			glVertex3f(x + lado_length, y + lado_length, z); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x, y, z); 

			x += lado_length;
			count += 1;
		}
		y += lado_length;
	}
	glEnd();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// Eixos
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	//cylinder0(1,2,10);
	//plane(2,7);
	//box(2,5);
	sphere(1,20,20);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
