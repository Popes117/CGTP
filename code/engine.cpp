#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include "tinyxml2-master/tinyxml2.h"
#include "Includes/common.hpp"

#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, betA = 0.0f, radius = 5.0f;
float camX, camY, camZ;

std::string filePath;
std::vector<std::string> filePaths;

int widtH;
int heighT;

float cameraX;
float cameraY;
float cameraZ;

// Variáveis globais para a posição do lookAt
float lookAtX;
float lookAtY;
float lookAtZ;

// Variáveis globais para a direção para cima
float upX;
float upY;
float upZ;

// Variáveis globais para a perspetiva
float foV;
float neaR;
float faR;

int length_form;
int divisions_form;

int opcao;

using namespace std;

// Stack de matrizes
std::stack<Matrix> matrixStack;
Group og_group = Group();

// Função para armazenar a matriz atual na stack
void pushMatrix() {
    Matrix mat;
    glGetFloatv(GL_MODELVIEW_MATRIX, mat.data);
    matrixStack.push(mat);
}

// Função para restaurar a matriz anterior da stack
void popMatrix() {
    if (!matrixStack.empty()) {
        Matrix mat = matrixStack.top();
        glLoadMatrixf(mat.data);
        matrixStack.pop();
    } else {
        std::cerr << "Error: Matrix stack is empty!" << std::endl;
    }
}

void spherical2Cartesian() {

	camX = radius * cos(betA) * sin(alfa);
	camY = radius * sin(betA);
	camZ = radius * cos(betA) * cos(alfa);

// Usa-mos isto quando for para mexer a câmara
    //cameraX = radius * cos(betA) * sin(alfa);
	//cameraY = radius * sin(betA);
	//cameraZ = radius * cos(betA) * cos(alfa);
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
	gluPerspective(foV ,1.0f, neaR, faR);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

std::vector<Square> parsePlane(const std::string& filename) {

	std::string first_line;
    std::vector<Square> triangles;
	std::ifstream file(filename);
	char separador;
    
    std::string linha;
    
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }
    
    while (std::getline(file, linha)) {
        
        if (linha.empty()) {
            continue;
        }

        std::istringstream iss(linha);
        Coordenadas ponto;
        Square square;

        while (iss >> ponto.x >> ponto.y >> ponto.z) {
            square.pontos.push_back(ponto);
			iss >> separador;
        }
        triangles.push_back(square);
    }

	return triangles;
}
/*
std::vector<std::vector<Triangle>> parseBox(const std::string& filename) {
    
    std::string first_line;
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
        iss >> first_line;
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
*/

std::vector<std::vector<Square>> parseBox(const std::string& filename) {
    
    std::string first_line;
    int length;
    int divisions;
    int count = -1;

    std::vector<std::vector<Square>> squares;
    squares.push_back(std::vector<Square>()); // Bottom
    squares.push_back(std::vector<Square>()); // Top
    squares.push_back(std::vector<Square>()); // Side1
    squares.push_back(std::vector<Square>()); // Side2
    squares.push_back(std::vector<Square>()); // Side3
    squares.push_back(std::vector<Square>()); // Side4

    std::ifstream file(filename);
    char separador;

    std::string linha;
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }

    while (std::getline(file, linha)) {
        if (linha.empty()) {
            count++;
        } else {
            std::istringstream iss(linha);
            Coordenadas ponto;
            Square square;

            while (iss >> ponto.x >> ponto.y >> ponto.z) {
                square.pontos.push_back(ponto);
                iss >> separador;
            }
            squares[count].push_back(square);
        }
    }

    return squares;
}


std::vector<std::vector<Square>> parseCone(const std::string& filename) {
    
    std::string first_line;
    float radius; 
    float height; 
    int slices;
    int stacks;
    int count = -1;

    std::vector<std::vector<Square>> triangles;
    triangles.push_back(std::vector<Square>()); // Bottom
    triangles.push_back(std::vector<Square>()); // Stack
    triangles.push_back(std::vector<Square>()); // Body

    std::ifstream file(filename);
    char separador;

    std::string linha;
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }

    while (std::getline(file, linha)) {
        if (linha.empty()) {
            count++;
        } else {
            std::istringstream iss(linha);
            Coordenadas ponto;
            Square triangle;

            while (iss >> ponto.x >> ponto.y >> ponto.z) {
                triangle.pontos.push_back(ponto);
                iss >> separador;
            }
            triangles[count].push_back(triangle);
        }
    }

    return triangles;
}

std::vector<Square> parseSphere(const std::string& filename) {
    int count = -1;

	std::string first_line;
    std::vector<Square> triangles;

    std::ifstream file(filename);
    char separador;

    std::string linha;
    
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }

    while (std::getline(file, linha)) {
        
        if (linha.empty()) {
            continue;
        }

        std::istringstream iss(linha);
        Coordenadas ponto;
        Square triangle;

        while (iss >> ponto.x >> ponto.y >> ponto.z) {
            triangle.pontos.push_back(ponto);
			iss >> separador;
        }
        triangles.push_back(triangle);
    }

    return triangles;
}


std::vector<vector<Coordenadas>> parsePatches(const std::string& filename) {
    std::string first_line;

    int count = -1;

    std::vector<vector<Coordenadas>> coordenadas;
    coordenadas.push_back(std::vector<Coordenadas>()); // Bottom
    coordenadas.push_back(std::vector<Coordenadas>()); // Stack
    coordenadas.push_back(std::vector<Coordenadas>()); // Body

    std::ifstream file(filename);
    char separador;

    std::string linha;
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }

    while (std::getline(file, linha)) {
        if (linha.empty()) {
            count++;
        } else {
            std::istringstream iss(linha);
            Coordenadas ponto;

            while (iss >> ponto.x >> ponto.y >> ponto.z) {
                coordenadas[count].push_back(ponto);
                iss >> separador;
            }
        }
    }

    return coordenadas;

}


void draw_plane(const std::string& filename){

    std::vector<Square> plane_triangles = parsePlane(filename);
	
	glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(Square square : plane_triangles){

		glVertex3f(square.pontos[0].x,square.pontos[0].y, square.pontos[0].z); 
    	glVertex3f(square.pontos[1].x,square.pontos[1].y, square.pontos[1].z); 
		glVertex3f(square.pontos[2].x,square.pontos[2].y, square.pontos[2].z); 
        glVertex3f(square.pontos[2].x,square.pontos[2].y, square.pontos[2].z); 
    	glVertex3f(square.pontos[3].x,square.pontos[3].y, square.pontos[3].z); 
		glVertex3f(square.pontos[0].x,square.pontos[0].y, square.pontos[0].z); 
	}
	glEnd();

}


void draw_box(const std::string& filename){

	std::vector<std::vector<Square>> box_squares = parseBox(filename);
	
	glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(std::vector<Square> squares : box_squares){
		for(Square square : squares){

			glVertex3f(square.pontos[0].x,square.pontos[0].y, square.pontos[0].z); 
    		glVertex3f(square.pontos[1].x,square.pontos[1].y, square.pontos[1].z); 
			glVertex3f(square.pontos[2].x,square.pontos[2].y, square.pontos[2].z);
            
            glVertex3f(square.pontos[2].x,square.pontos[2].y, square.pontos[2].z); 
    	    glVertex3f(square.pontos[3].x,square.pontos[3].y, square.pontos[3].z); 
		    glVertex3f(square.pontos[0].x,square.pontos[0].y, square.pontos[0].z);  

		}
	}
	glEnd();

}


void draw_cone(const std::string& filename){

	std::vector<std::vector<Square>> cone_triangles = parseCone(filename);
	
	glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(std::vector<Square> triangles : cone_triangles){
		for(Square triangle : triangles){
            if(triangle.pontos.size() == 3){
                glVertex3f(triangle.pontos[0].x,triangle.pontos[0].y, triangle.pontos[0].z); 
    		    glVertex3f(triangle.pontos[1].x,triangle.pontos[1].y, triangle.pontos[1].z); 
			    glVertex3f(triangle.pontos[2].x,triangle.pontos[2].y, triangle.pontos[2].z);
            }
            else{
                glVertex3f(triangle.pontos[0].x,triangle.pontos[0].y, triangle.pontos[0].z); 
    		    glVertex3f(triangle.pontos[1].x,triangle.pontos[1].y, triangle.pontos[1].z); 
			    glVertex3f(triangle.pontos[2].x,triangle.pontos[2].y, triangle.pontos[2].z);
                glVertex3f(triangle.pontos[3].x,triangle.pontos[3].y, triangle.pontos[3].z); 
    		    glVertex3f(triangle.pontos[0].x,triangle.pontos[0].y, triangle.pontos[0].z); 
			    glVertex3f(triangle.pontos[2].x,triangle.pontos[2].y, triangle.pontos[2].z);
            }

		}
	}
	glEnd();

}

void draw_sphere(const std::string& filename) {

    std::vector<Square> sphere_triangles = parseSphere(filename);
 
    // body
	glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(Square triangle : sphere_triangles){

		glVertex3f(triangle.pontos[0].x,triangle.pontos[0].y, triangle.pontos[0].z); 
    	glVertex3f(triangle.pontos[1].x,triangle.pontos[1].y, triangle.pontos[1].z); 
		glVertex3f(triangle.pontos[2].x,triangle.pontos[2].y, triangle.pontos[2].z); 
        glVertex3f(triangle.pontos[3].x,triangle.pontos[3].y, triangle.pontos[3].z); 
    	glVertex3f(triangle.pontos[1].x,triangle.pontos[1].y, triangle.pontos[1].z); 
		glVertex3f(triangle.pontos[0].x,triangle.pontos[0].y, triangle.pontos[0].z); 
	}
	glEnd();

}

void draw_patches(const std::string& filename) {
    std::vector<vector<Coordenadas>> patches = parsePatches(filename);

    for (const auto& patch : patches) {
        glBegin(GL_LINE_STRIP);
        for (const auto& ponto : patch) {
            glVertex3f(ponto.x, ponto.y, ponto.z);
        }
        glEnd();
    }
}

void handle_form(const std::string& filename){
	
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return; // ou faça alguma outra coisa para lidar com o erro
    }
	std::string first_line;
    
    std::string linha;
    
    if (std::getline(file, linha)) {
        std::istringstream iss(linha);
        iss >> first_line;
    }

    if (first_line == "plane") {
        draw_plane(filename);
    }
    else if (first_line == "box") {
        draw_box(filename);
    }
    else if (first_line == "cone") {
        draw_cone(filename);
    }
    else if (first_line == "sphere") {
        draw_sphere(filename);
    }
}

void handle_groups(const Group& group) {
    
    pushMatrix();

    for (const auto& transform : group.transforms) {
        if (transform.type == "translate") {
            glTranslatef(transform.x, transform.y, transform.z);
        } else if (transform.type == "rotate") {
            glRotatef(transform.angle, transform.x, transform.y, transform.z);
        } else if (transform.type == "scale") {
            glScalef(transform.x, transform.y, transform.z);
        }
    }

    for (const auto& model_path : group.model_paths) {
        handle_form(model_path);
    }

    for (const auto& sub_group : group.groups) {
        handle_groups(sub_group);
    }

    popMatrix();
}

void printMuitosTriangles(const std::vector<std::vector<Triangle>>& triangles) {
    for (const auto& triangleList : triangles) {
        for (const auto& triangle : triangleList) {
            std::cout << "Triangle:\n";
            for (const auto& ponto : triangle.pontos) {
                std::cout << "  (" << ponto.x << ", " << ponto.y << ", " << ponto.z << ")\n";
            }
            std::cout << std::endl;
        }
    }
}

void printPoucosTriangles(const std::vector<Triangle>& triangles) {
    for (const auto& triangle : triangles){
        std::cout << "Triangle:\n";
        for (const auto& ponto : triangle.pontos) {
            std::cout << "  (" << ponto.x << ", " << ponto.y << ", " << ponto.z << ")\n";
        }
        std::cout << std::endl;
    }
}

void processWindowElement(tinyxml2::XMLElement* windowElement) {
    int width, height;
    windowElement->QueryIntAttribute("width", &width);
    windowElement->QueryIntAttribute("height", &height);

    widtH = width;
    heighT = height;

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

    cameraX = px;
    cameraY = py;
    cameraZ = pz;

    lookAtX = lx;
    lookAtY = ly;
    lookAtZ = lz;

    upX = ux;
    upY = uy;
    upZ = uz;

    foV = fov;
    neaR = near;
    faR = far;

}

void processColorElement(tinyxml2::XMLElement* colorElement) {
    float dr, dg, db, ar, ag, ab, sr, sg, sb, er, eg, eb, value;

    for (tinyxml2::XMLElement* child = colorElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();

        if (strcmp(childName, "diffuse") == 0) {
            child->QueryFloatAttribute("R", &dr);
            child->QueryFloatAttribute("g", &dg);
            child->QueryFloatAttribute("b", &db);
        }
        else if (strcmp(childName, "ambient") == 0) {
            child->QueryFloatAttribute("R", &ar);
            child->QueryFloatAttribute("g", &ag);
            child->QueryFloatAttribute("b", &ab);
        }
        else if (strcmp(childName, "specular") == 0) {
            child->QueryFloatAttribute("R", &sr);
            child->QueryFloatAttribute("g", &sg);
            child->QueryFloatAttribute("b", &sb);
        }
        else if (strcmp(childName, "shininess") == 0) {
            child->QueryFloatAttribute("value", &value);
        }
    }

    // guardar em variaveis globais
}

void processTextureElement(tinyxml2::XMLElement* textureElement) {
    const char* file = textureElement->Attribute("file");
    filePath = "3DFiles/" + std::string(file);
    filePaths.push_back(filePath);

    if (file) {
        std::cout << "Model: File = " << file << std::endl;
    }
    else {
        std::cerr << "Model element is missing the 'file' attribute." << std::endl;
    }
}
 
void processModelElement(tinyxml2::XMLElement* modelElement, Group& og_group) {
    const char* file = modelElement->Attribute("file");
    filePath = "3DFiles/" + std::string(file);
    filePaths.push_back(filePath);
    og_group.model_paths.push_back(filePath);

    /*
    if (strcmp(childName, "color") == 0) {
        processColorElement(child);
    }

    if (strcmp(childName, "texture") == 0) {
        processTextureElement(child);
    }
    */

    if (file) {
        std::cout << "Model: File = " << file << std::endl;
    }
    else {
        std::cerr << "Model element is missing the 'file' attribute." << std::endl;
    }
}
 
void processModelsElement(tinyxml2::XMLElement* modelsElement, Group& og_group) {
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
        processModelElement(modelElement, og_group);
    }
}

void processTransformElement(tinyxml2::XMLElement* transformElement, Group& og_group) {

    float tx, ty, tz, rx, ry, rz, sx, sy, sz, angle;

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();

        if (strcmp(childName, "translate") == 0) {
            child->QueryFloatAttribute("x", &tx);
            child->QueryFloatAttribute("y", &ty);
            child->QueryFloatAttribute("z", &tz);
            Transform transform = Transform("translate", 0, tx, ty, tz);
            og_group.transforms.push_back(transform);
        }
        else if (strcmp(childName, "rotate") == 0) {
            child->QueryFloatAttribute("angle", &angle);
            child->QueryFloatAttribute("x", &rx);
            child->QueryFloatAttribute("y", &ry);
            child->QueryFloatAttribute("z", &rz);
            Transform transform = Transform("rotate", angle, rx, ry, rz);
            og_group.transforms.push_back(transform);
        }
        else if (strcmp(childName, "scale") == 0) {
            child->QueryFloatAttribute("x", &sx);
            child->QueryFloatAttribute("y", &sy);
            child->QueryFloatAttribute("z", &sz);
            Transform transform = Transform("scale", 0, sx, sy, sz);
            og_group.transforms.push_back(transform);
        }
    }

    // guardar em variaveis globais
}


 
void processGroupElement(tinyxml2::XMLElement* groupElement, Group& og_group) {
        
    for (tinyxml2::XMLElement* child = groupElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();
 
        if (strcmp(childName, "models") == 0) {
            processModelsElement(child, og_group);
        }

        if (strcmp(childName, "transform") == 0) {
            
            processTransformElement(child, og_group);
        }

        if (strcmp(childName, "group") == 0) {

            Group group_bla = Group();
            
            processGroupElement(child, group_bla);
            og_group.groups.push_back(group_bla);
        }
        //Adiciona mais condições conforme necessário para outros tipos de elementos dentro de 'group'
    }

}
 
void processLightElement(tinyxml2::XMLElement* lightElement) {
    float px, py, pz, dx, dy, dz, sx, sy, sz, sdx, sdy, sdz, c;

    for (tinyxml2::XMLElement* child = lightElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();

        if (strcmp(childName, "point") == 0) {
            child->QueryFloatAttribute("posX", &px);
            child->QueryFloatAttribute("posY", &py);
            child->QueryFloatAttribute("posZ", &pz);
        }
        else if (strcmp(childName, "directional") == 0) {
            child->QueryFloatAttribute("dirX", &dx);
            child->QueryFloatAttribute("dirY", &dy);
            child->QueryFloatAttribute("dirZ", &dz);
        }
        else if (strcmp(childName, "spotlight") == 0) {
            child->QueryFloatAttribute("posX", &sx);
            child->QueryFloatAttribute("posY", &sy);
            child->QueryFloatAttribute("posZ", &sz);
            child->QueryFloatAttribute("dirX", &sdx);
            child->QueryFloatAttribute("dirY", &sdy);
            child->QueryFloatAttribute("dirZ", &sdz);
            child->QueryFloatAttribute("cutoff", &c);
        }
    }

    // guardar em variaveis globais
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

            processGroupElement(child, og_group);
        }
        else if (strcmp(childName, "light") == 0) {
            processLightElement(child);
        }
    }
}
 
int parsexml(const char *filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
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
	gluLookAt(cameraX, cameraY, cameraZ,
		lookAtX, lookAtY, lookAtZ,
		upX, upY, upZ);

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
	
    handle_groups(og_group);

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

    parsexml(argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(widtH,heighT);
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