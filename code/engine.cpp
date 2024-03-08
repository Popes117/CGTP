#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tinyxml2-master/tinyxml2.h"

#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, betA = 0.0f, radius = 5.0f;
float camX, camY, camZ;

int length_form;
int divisions_form;

int opcao;

struct Coordenadas{
	double p1, p2, p3;
};

struct Triangle{
	std::vector<Coordenadas> pontos;
};

using namespace std;

void spherical2Cartesian() {

	camX = radius * cos(betA) * sin(alfa);
	camY = radius * sin(betA);
	camZ = radius * cos(betA) * cos(alfa);
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

std::vector<Triangle> parsePlane(const std::string& filename) {
    int length;
    int divisions;

    std::vector<Triangle> triangles;
	std::ifstream file(filename);
	char separador;
    
    std::string linha;
    
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> length >> divisions;
    }
    
    while (std::getline(file, linha)) {
        
        if (linha.empty()) {
            continue;
        }

        std::istringstream iss(linha);
        Coordenadas ponto;
        Triangle triangle;

        while (iss >> ponto.p1 >> ponto.p2 >> ponto.p3) {
            triangle.pontos.push_back(ponto);
			iss >> separador;
        }
        triangles.push_back(triangle);
    }

    length_form = length;
    divisions_form = divisions;

	return triangles;
}

std::vector<std::vector<Triangle>> parseBox(const std::string& filename) {
    int length;
    int divisions;
    int count = -1;

    std::vector<std::vector<Triangle>> triangles;
    triangles.push_back(std::vector<Triangle>()); // Bottom
    triangles.push_back(std::vector<Triangle>()); // Top
    triangles.push_back(std::vector<Triangle>()); // Side1
    triangles.push_back(std::vector<Triangle>()); // Side2
    triangles.push_back(std::vector<Triangle>()); // Side3
    triangles.push_back(std::vector<Triangle>()); // Side4

    std::ifstream file(filename);
    char separador;

    std::string linha;
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> length >> divisions;
    }

    while (std::getline(file, linha)) {
        if (linha.empty()) {
            count++;
        } else {
            std::istringstream iss(linha);
            Coordenadas ponto;
            Triangle triangle;

            while (iss >> ponto.p1 >> ponto.p2 >> ponto.p3) {
                triangle.pontos.push_back(ponto);
                iss >> separador;
            }
            triangles[count].push_back(triangle);
        }
    }

    //std::cout << "Length: " << length << "\n";
    //std::cout << "Divisions: " << divisions << "\n";
    //std::cout << "Bottom triangles: " << triangles[0].size() << "\n";
    //std::cout << "Top triangles: " << triangles[1].size() << "\n";
    //std::cout << "Side1 triangles: " << triangles[2].size() << "\n";
    //std::cout << "Side2 triangles: " << triangles[3].size() << "\n";
    //std::cout << "Side3 triangles: " << triangles[4].size() << "\n";
    //std::cout << "Side4 triangles: " << triangles[5].size() << "\n";

    return triangles;
}

std::vector<std::vector<Triangle>> parseCone(const std::string& filename) {
    float radius; 
    float height; 
    int slices;
    int stacks;
    int count = -1;

    std::vector<std::vector<Triangle>> triangles;
    triangles.push_back(std::vector<Triangle>()); // Bottom
    triangles.push_back(std::vector<Triangle>()); // Stack
    triangles.push_back(std::vector<Triangle>()); // Body

    std::ifstream file(filename);
    char separador;

    std::string linha;
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> radius >> height >> slices >> stacks;
    }

    while (std::getline(file, linha)) {
        if (linha.empty()) {
            count++;
        } else {
            std::istringstream iss(linha);
            Coordenadas ponto;
            Triangle triangle;

            while (iss >> ponto.p1 >> ponto.p2 >> ponto.p3) {
                triangle.pontos.push_back(ponto);
                iss >> separador;
            }
            triangles[count].push_back(triangle);
        }
    }

    return triangles;
}

void draw_plane(){

    std::vector<Triangle> plane_triangles = parsePlane("build/3DFiles/plane.3d");
	
	glBegin(GL_TRIANGLES);

	for(Triangle triangle : plane_triangles){

		glVertex3f(triangle.pontos[0].p1,triangle.pontos[0].p2, triangle.pontos[0].p3); 
    	glVertex3f(triangle.pontos[1].p1,triangle.pontos[1].p2, triangle.pontos[1].p3); 
		glVertex3f(triangle.pontos[2].p1,triangle.pontos[2].p2, triangle.pontos[2].p3); 
	}
	glEnd();

}

void draw_box(){

	std::vector<std::vector<Triangle>> box_triangles = parseBox("build/3DFiles/box.3d");
	
	glBegin(GL_TRIANGLES);
	for(std::vector<Triangle> triangles : box_triangles){
		for(Triangle triangle : triangles){

			glVertex3f(triangle.pontos[0].p1,triangle.pontos[0].p2, triangle.pontos[0].p3); 
    		glVertex3f(triangle.pontos[1].p1,triangle.pontos[1].p2, triangle.pontos[1].p3); 
			glVertex3f(triangle.pontos[2].p1,triangle.pontos[2].p2, triangle.pontos[2].p3); 

		}
	}
	glEnd();

}

void draw_cone(){

	std::vector<std::vector<Triangle>> cone_triangles = parseCone("build/3DFiles/cone.3d");
	
	glBegin(GL_TRIANGLES);
	for(std::vector<Triangle> triangles : cone_triangles){
		for(Triangle triangle : triangles){

			glVertex3f(triangle.pontos[0].p1,triangle.pontos[0].p2, triangle.pontos[0].p3); 
    		glVertex3f(triangle.pontos[1].p1,triangle.pontos[1].p2, triangle.pontos[1].p3); 
			glVertex3f(triangle.pontos[2].p1,triangle.pontos[2].p2, triangle.pontos[2].p3); 

		}
	}
	glEnd();

}

void printMuitosTriangles(const std::vector<std::vector<Triangle>>& triangles) {
    for (const auto& triangleList : triangles) {
        for (const auto& triangle : triangleList) {
            std::cout << "Triangle:\n";
            for (const auto& ponto : triangle.pontos) {
                std::cout << "  (" << ponto.p1 << ", " << ponto.p2 << ", " << ponto.p3 << ")\n";
            }
            std::cout << std::endl;
        }
    }
}

void printPoucosTriangles(const std::vector<Triangle>& triangles) {
    for (const auto& triangle : triangles){
        std::cout << "Triangle:\n";
        for (const auto& ponto : triangle.pontos) {
            std::cout << "  (" << ponto.p1 << ", " << ponto.p2 << ", " << ponto.p3 << ")\n";
        }
        std::cout << std::endl;
    }
}

void processWindowElement(tinyxml2::XMLElement* windowElement) {
    int width, height;
    windowElement->QueryIntAttribute("width", &width);
    windowElement->QueryIntAttribute("height", &height);

    //Teste a imprimir para a consola a ver se leu bem
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
    //Teste a imprimir para a consola a ver se leu bem
    std::cout << "Camera: Position(" << px << ", " << py << ", " << pz << "), ";
    std::cout << "LookAt(" << lx << ", " << ly << ", " << lz << "), ";
    std::cout << "Up(" << ux << ", " << uy << ", " << uz << "), ";
    std::cout << "Projection(FOV=" << fov << ", Near=" << near << ", Far=" << far << ")" << std::endl;
}
 
void processModelElement(tinyxml2::XMLElement* modelElement) {
    const char* file = modelElement->Attribute("file");
    if (file) {
        //nao sei o que fazer com isto
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
        //Adicione mais condições conforme necessário para outros tipos de elementos dentro de 'group'
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
        //Falta light e transform para o futuro
    }
}
 
int parsexml() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("test_files/test_files_phase_1/test_1_1.xml") != tinyxml2::XML_SUCCESS) {
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

void renderScene(){

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
	
	//draw_plane();
	//draw_box();
	draw_cone();

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
		betA += 0.1f;
		if (betA > 1.5f)
			betA = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		betA -= 0.1f;
		if (betA < -1.5f)
			betA = -1.5f;
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


int main(int argc, char** argv){

    parsexml();
/*
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
*/
    return 1;
}