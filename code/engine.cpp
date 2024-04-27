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

float cam2X = 0, cam2Y, cam2Z = 5;
int startX, startY, tracking = 0;

int alpha = 0, betaB = 0, r = 5;

float alfa = 0.0f, betA = 0.0f, radius = 5.0f;
float camX, camY, camZ;
float elapsedTime = 0.0f;

bool draw_curve = true;
bool draw_axis = true;

int timebase = 0, frame = 0;

std::string filePath;
std::vector<std::string> filePaths;

int widtH;
int heighT;
int TESSELATION = 100;

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

std::vector<GLuint> vertices;
int counter = 1;

// Stack de matrizes
std::stack<Matrix> matrixStack;
Group og_group = Group();


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float m[4][4], float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j][k];
		}
	}

}


void getCatmullRomPoint(float time, int indices[4], Transform transform, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
					  { 1.0f, -2.5f,  2.0f, -0.5f},
					  {-0.5f,  0.0f,  0.5f,  0.0f},
					  { 0.0f,  1.0f,  0.0f,  0.0f} };

    float a[4] = {0};

    //for x
    float pComponentX[4] = {
        transform.pontos[indices[0]].x,
        transform.pontos[indices[1]].x,
        transform.pontos[indices[2]].x,
        transform.pontos[indices[3]].x
    };

    multMatrixVector(m,pComponentX,a);
    pos[0]   = powf(time, 3.0) * a[0] + powf(time, 2.0) * a[1] + time * a[2] + a[3];
    deriv[0] = 3 * powf(time, 2.0) * a[0] + 2 * time * a[1] + a[2];


    //for y
    float pComponentY[4] = {
        transform.pontos[indices[0]].y,
        transform.pontos[indices[1]].y,
        transform.pontos[indices[2]].y,
        transform.pontos[indices[3]].y
    };
    multMatrixVector(m,pComponentY,a);
    pos[1]   = powf(time, 3.0) * a[0] + powf(time, 2.0) * a[1] + time * a[2] + a[3];
    deriv[1] = 3 * powf(time, 2.0) * a[0] + 2 * time * a[1] + a[2];

    //for z
    float pComponentZ[4] = {
        transform.pontos[indices[0]].z,
        transform.pontos[indices[1]].z,
        transform.pontos[indices[2]].z,
        transform.pontos[indices[3]].z
    };
    multMatrixVector(m,pComponentZ,a);
    pos[2]   = powf(time, 3.0) * a[0] + powf(time, 2.0) * a[1] + time * a[2] + a[3];
    deriv[2] = 3 * powf(time, 2.0) * a[0] + 2 * time * a[1] + a[2];
}

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, Transform transform) {

    int pointCount = transform.pontos.size();
	float t = gt * pointCount; 
	int index = floor(t);  
	t = t - index; 

	// indices store the points
	int indices[4];
	indices[0] = (index + pointCount - 1) % pointCount;
	indices[1] = (indices[0] + 1) % pointCount;
	indices[2] = (indices[1] + 1) % pointCount;
	indices[3] = (indices[2] + 1) % pointCount;

	getCatmullRomPoint(
        t,
        indices,
        transform,
        pos,
        deriv
    );
}

void renderCatmullRomCurve(float* pos, float* deriv, Transform transform) {

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
    
    float gt = 0;
	while (gt < 1) {
		getGlobalCatmullRomPoint(gt, pos, deriv, transform);
		glVertex3f(pos[0], pos[1], pos[2]);
		gt += 1.0 / TESSELATION;
	}
	glEnd();
}

void processCatmullRomTranslation(Transform transform){

    float pos[3], deriv[3];
    float gt = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) / transform.time;


    if(draw_curve)
        renderCatmullRomCurve(pos, deriv, transform);
    getGlobalCatmullRomPoint(gt, pos, deriv, transform);
    glTranslatef(pos[0], pos[1], pos[2]);

    float xv[3],
          yv[3] = {0.0f,1.0f,0.0f},
          zv[3];

	xv[0] = deriv[0];
	xv[1] = deriv[1];
	xv[2] = deriv[2];

    normalize(xv);
    cross(xv,yv,zv);
    normalize(zv);
    cross(zv,xv,yv);
    normalize(yv);
        
    if (transform.align){
        float rot[16];
        buildRotMatrix(xv,yv,zv,rot);
        glMultMatrixf(rot);
    }
}


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

std::vector<float> parsePlane(const std::string& filename) {

	std::string first_line;
    std::vector<float> pontos_a;
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
            pontos_a.push_back(ponto.x);
            pontos_a.push_back(ponto.y);
            pontos_a.push_back(ponto.z);
			iss >> separador;
        }
        //triangles.push_back(square);
    }

	return pontos_a;
}

std::vector<float> parseBox(const std::string& filename) {
    
    std::string first_line;
    int length;
    int divisions;
    int count = -1;

    std::vector<float> squares;

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
                squares.push_back(ponto.x);
                squares.push_back(ponto.y);
                squares.push_back(ponto.z);
                iss >> separador;
            }
        }
    }

    return squares;
}


std::vector<float> parseCone(const std::string& filename) {
    
    std::string first_line;
    float radius; 
    float height; 
    int slices;
    int stacks;
    int count = -1;

    std::vector<float> triangles;

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
                triangles.push_back(ponto.x);
                triangles.push_back(ponto.y);
                triangles.push_back(ponto.z);
                iss >> separador;
            }
        }
    }

    return triangles;
}

std::vector<float> parseSphere(const std::string& filename) {
    int count = -1;

	std::string first_line;
    std::vector<float> triangles;

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
            triangles.push_back(ponto.x);
            triangles.push_back(ponto.y);
            triangles.push_back(ponto.z);
			iss >> separador;
        }
    }

    return triangles;
}


std::vector<float> parsePatches(const std::string& filename) {
    std::string first_line;

    int count = -1;

    std::vector<vector<Coordenadas>> coordenadas;
    coordenadas.push_back(std::vector<Coordenadas>()); // Bottom
    coordenadas.push_back(std::vector<Coordenadas>()); // Stack
    coordenadas.push_back(std::vector<Coordenadas>());
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
    
    const auto& patch = coordenadas[0]; // Acessa o primeiro patch

    const auto& indexes = coordenadas[1]; // Acessa os índices dos patches
    std::vector<float> coors;
    for (size_t i = 0; i < indexes.size(); i++) {
        const Coordenadas& ponto1 = patch[indexes[i].x]; 
        const Coordenadas& ponto2 = patch[indexes[i].y]; 
        const Coordenadas& ponto3 = patch[indexes[i].z]; 

        coors.push_back(ponto1.x);
        coors.push_back(ponto1.y);
        coors.push_back(ponto1.z);
        coors.push_back(ponto2.x);
        coors.push_back(ponto2.y);
        coors.push_back(ponto2.z);
        coors.push_back(ponto3.x);
        coors.push_back(ponto3.y);
        coors.push_back(ponto3.z);
    }

    return coors;

}


void draw_model(GLuint vbo_id, GLuint count){
	//glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Limpar o estado do buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glEnd();

}

/*
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
    std::vector<std::vector<Coordenadas>> patches = parsePatches(filename);

    const auto& patch = patches[0]; // Acessa o primeiro patch

    const auto& indexes = patches[1]; // Acessa os índices dos patches
	glBegin(GL_TRIANGLES); 
    glColor3f(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < indexes.size(); i++) {
        const Coordenadas& ponto1 = patch[indexes[i].x]; 
        const Coordenadas& ponto2 = patch[indexes[i].y]; 
        const Coordenadas& ponto3 = patch[indexes[i].z]; 

        // Primeiro triângulo
        glVertex3f(ponto1.x, ponto1.y, ponto1.z);
        glVertex3f(ponto2.x, ponto2.y, ponto2.z);
        glVertex3f(ponto3.x, ponto3.y, ponto3.z);

    }
    glEnd(); 
    
}
*/
void handle_form(const Model &filename){
    draw_model(filename.vbo_id,filename.count);
}

void update(int value) {
    // Atualizar o tempo e solicitar uma nova renderização

    elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Obter o tempo decorrido em segundos
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);

    //elapsedTime += 0.01f; // Incrementar o tempo decorrido
    //glutPostRedisplay();
    //glutTimerFunc(16, update, 0); // Redesenhar a cada 16 milissegundos (aproximadamente 60 FPS)
}

void handle_groups(const Group& group) {
    
    pushMatrix();

    for (const auto& transform : group.transforms) {
        if (transform.type == "translate") {
            glTranslatef(transform.x, transform.y, transform.z);
        } else if (transform.type == "translateP") {
            processCatmullRomTranslation(transform);
        } else if (transform.type == "rotateA") {
            glRotatef(transform.angle, transform.x, transform.y, transform.z);
        } else if (transform.type == "rotateT") {
            float angle = (elapsedTime / transform.angle) * 360.0f; 
            glRotatef(angle, transform.x, transform.y, transform.z);
        } else if (transform.type == "scale") {
            glScalef(transform.x, transform.y, transform.z);
        }
    }

    for (const auto& model : group.models) {
        handle_form(model);
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
    const char* file2 = modelElement->Attribute("file");
    const std::string filename = "3DFiles/" + std::string(file2);
    
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
    std::vector<float> vetor;
    if (first_line == "plane") {
        vetor = parsePlane(filename);
    }
    else if (first_line == "box") {
        vetor = parseBox(filename);
    }
    else if (first_line == "cone") {
        vetor = parseCone(filename);
    }
    else if (first_line == "sphere") {
        vetor = parseSphere(filename);
    }
    else if (first_line == "patch") {
        vetor = parsePatches(filename);
    }
    Model m;
    m.vbo_id = counter;
    m.coords = vetor;
    m.count = m.coords.size() / 3;
    
    GLuint vecLoc;
    glGenBuffers(1,&m.vbo_id);

    vertices.push_back(m.vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo_id);
    glBufferData(
     GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
     sizeof(float) * m.coords.size(), // tamanho do vector em bytes
     m.coords.data(), // os dados do array associado ao vector
    GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)


    counter++;

    filePaths.push_back(filePath);

    og_group.model_paths.push_back(filePath);
    og_group.models.push_back(m);

    /*
    if (strcmp(childName, "color") == 0) {
        processColorElement(child);
    }

    if (strcmp(childName, "texture") == 0) {
        processTextureElement(child);
    }
    */

    if (file2) {
        std::cout << "Model: File = " << file2 << std::endl;
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

    float tx, ty, tz, rx, ry, rz, sx, sy, sz, angle, time;
    bool align;

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();

        if (strcmp(childName, "translate") == 0) {
            if (child->QueryFloatAttribute("time", &time) == tinyxml2::XML_SUCCESS &&
                child->QueryBoolAttribute("align", &align) == tinyxml2::XML_SUCCESS) {
                // Complex translation with time and alignment
                std::vector<Coordenadas> points;
                for (tinyxml2::XMLElement* pointElement = child->FirstChildElement("point"); pointElement; pointElement = pointElement->NextSiblingElement("point")) {
                    float px, py, pz;
                    pointElement->QueryFloatAttribute("x", &px);
                    pointElement->QueryFloatAttribute("y", &py);
                    pointElement->QueryFloatAttribute("z", &pz);
                    points.push_back(Coordenadas(px, py, pz));
                }
                Transform transform = Transform("translateP", time, align, points);
                og_group.transforms.push_back(transform);
            }
            else {
                // Simple translation
                child->QueryFloatAttribute("x", &tx);
                child->QueryFloatAttribute("y", &ty);
                child->QueryFloatAttribute("z", &tz);
                Transform transform = Transform("translate", 0, tx, ty, tz);
                og_group.transforms.push_back(transform);
            }
        }
        else if (strcmp(childName, "rotate") == 0) {

            if (child->QueryFloatAttribute("time", &time) == tinyxml2::XML_SUCCESS) {
                child->QueryFloatAttribute("time", &time);
                child->QueryFloatAttribute("x", &rx);
                child->QueryFloatAttribute("y", &ry);
                child->QueryFloatAttribute("z", &rz);
                Transform transform = Transform("rotateT", time, rx, ry, rz);
                og_group.transforms.push_back(transform);
            }
            else {
                child->QueryFloatAttribute("angle", &angle);
                child->QueryFloatAttribute("x", &rx);
                child->QueryFloatAttribute("y", &ry);
                child->QueryFloatAttribute("z", &rz);
                Transform transform = Transform("rotateA", angle, rx, ry, rz);
                og_group.transforms.push_back(transform);
            }

        }
        else if (strcmp(childName, "scale") == 0) {
            child->QueryFloatAttribute("x", &sx);
            child->QueryFloatAttribute("y", &sy);
            child->QueryFloatAttribute("z", &sz);
            Transform transform = Transform("scale", 0, sx, sy, sz);
            og_group.transforms.push_back(transform);
        }
    }

}

    // guardar em variaveis globais


 
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
 
        if (strcmp(childName, "camera") == 0) {
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

int camara_settings(const char *filename) {
    
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cout << filename << std::endl;
        std::cerr << "Erro ao carregar o arquivo XML." << std::endl;
        return 1;
    }
 
    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "Elemento 'world' não encontrado." << std::endl;
        return 1;
    }    
    
    for (tinyxml2::XMLElement* child = worldElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        const char* childName = child->Name();
 
        if (strcmp(childName, "window") == 0) {
            processWindowElement(child);
            break;
        }

    }
    return 0;
}

int parsexml(const char *filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cout << filename << std::endl;
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

	float fps;
	int time;
	char s[64];

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ,
		lookAtX, lookAtY, lookAtZ,
		upX, upY, upZ);

	// Eixos

    if(draw_axis){
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
    }

    handle_groups(og_group);

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", trunc(fps));
		glutSetWindowTitle(s);
	} 

	// End of frame
	glutSwapBuffers();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            betaB += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}

void processMouseMotion(int xx, int yy)
{
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {

        alphaAux = alpha + deltaX;
        betaAux = betaB + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = betaB;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * sin(betaAux * 3.14 / 180.0);
}

void processKeys(unsigned char c, int xx, int yy) {

    switch (c) {
        case 'a':
        case 'A':   
            draw_curve = !draw_curve;
            break;
        case 'x':
        case 'X':   
            draw_axis = !draw_axis;
            break;

        default:
            break;
    }

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

    camara_settings(argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(widtH,heighT);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    glutTimerFunc(16, update, 0);	

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif
	parsexml(argv[1]);

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
