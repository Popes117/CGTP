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

#endif

#define _USE_MATH_DEFINES
#include <math.h>

struct Coordenadas{
	double x, y, z;
};

struct Triangle{
	std::vector<Coordenadas> pontos;
};

float alfa = 0.0f, betA = 0.0f, radius = 10.0f;
float camX, camY, camZ;
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

void generate_cone(float radius, float height, int slices,int stacks, const std::string& filename) {

	int i;
	float step;
	float n_stacks;
	vector<vector<Triangle>> cone_parts;
	step = 360.0 / slices;
	n_stacks = height/stacks;

	vector<Triangle> bottom;
	vector<Triangle> stack;
	vector<Triangle> body;
	// bottom
	for (i = 0; i < slices; i++) {

		Triangle triangle;
		
		Coordenadas coordenadas1 = {0, 0, 0};
		triangle.pontos.push_back(coordenadas1);
		Coordenadas coordenadas2 = {cos((i + 1) * step * M_PI / 180.0)*radius, 0, -sin((i + 1) * step *M_PI / 180.0)*radius};
		triangle.pontos.push_back(coordenadas2);
		Coordenadas coordenadas3 = {cos(i * step * M_PI / 180.0)*radius, 0, -sin(i * step *M_PI / 180.0)*radius};
		triangle.pontos.push_back(coordenadas3);
		bottom.push_back(triangle);
	}

	cone_parts.push_back(bottom);

	// body
	for (i = 0; i <= slices; i++) {
		
		//Triângulos da primeira stack

		Triangle triangle;

		Coordenadas coordenadas1 = {0.0f,height,0.0f};
		triangle.pontos.push_back(coordenadas1);
		Coordenadas coordenadas2 = {cos(i * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin(i * step *M_PI / 180.0)*radius/stacks};
		triangle.pontos.push_back(coordenadas2);
		Coordenadas coordenadas3 = {cos((i + 1) * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin((i + 1) * step *M_PI / 180.0)*radius/stacks};
		triangle.pontos.push_back(coordenadas3);
		stack.push_back(triangle);
		
		for(int j = 1; j < stacks; j++){

			//Triângulo da metade da esquerda
			Triangle left_triangle;

			Coordenadas coordenadas1 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j)};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin(i * step *M_PI / 180.0)*((radius/stacks)*(j+1))};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {cos((i + 1) * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin((i + 1) * step *M_PI / 180.0)*((radius/stacks)*(j+1))};
			left_triangle.pontos.push_back(coordenadas3);
			body.push_back(left_triangle);
			
			//Triângulo da metade da direita
			Triangle right_triangle;

			Coordenadas coordenadas4 = {cos((i+1) * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin((i+1) * step *M_PI / 180.0)*((radius/stacks)*j)};
			right_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas5 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j)};
			right_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas6 = {cos((i + 1) * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin((i + 1) * step *M_PI / 180.0)*((radius/stacks)*(j+1))};
			right_triangle.pontos.push_back(coordenadas3);
			body.push_back(right_triangle);	
		}
	}
	cone_parts.push_back(stack);
	cone_parts.push_back(body);

	std::ofstream file("3DFiles/" + filename);

    // Verifica se o arquivo foi aberto corretamente
    if (file.is_open()) {
        // Escreve no arquivo
        file << radius << "\n" << height << "\n" << slices << "\n" << stacks << "\n\n\n";

		for(vector<Triangle> triangles : cone_parts){
			for(Triangle triangle : triangles){
				for(int i = 0; i < triangle.pontos.size(); ++i){
					file << triangle.pontos[i].x << " " << triangle.pontos[i].y << " " << triangle.pontos[i].z;
					    if (i < triangle.pontos.size() - 1) { 
        				    file << " ; ";
        				} else {
        				    file << " "; 
        				}
				}
				file << "\n";
			}
			file << "\n\n";
		}

        // Fecha o arquivo
        file.close();
        std::cout << "Texto escrito com sucesso no arquivo.\n";
    } else {
        // Se houver algum erro ao abrir o arquivo
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

}

void sphere(double radius, int slices, int stacks) {

    int i;
    float step;    float n_stacks;

    step = 2* M_PI / slices; 
    n_stacks = M_PI / stacks;
    vector<tuple<float, float, float>> pontos;
 
    // body
    for (i = 0; i < slices; i++) {
        const double currentSlice = i * step;
        for (int j = 0; j < stacks; j++) {
            const double currentStack = j * n_stacks;
            const double nextStack = currentStack + n_stacks;
            const double nextSlice = currentSlice + step;

            tuple<double, double, double> p0 = { (radius * sin(nextStack) * sin(nextSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(nextSlice))};
            tuple<double, double, double> p1 = { (radius * sin(nextStack) * sin(currentSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(currentSlice)) };
            tuple<double, double, double> p2 = { (radius * sin(currentStack) * sin(nextSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(nextSlice)) };
            tuple<double, double, double> p3 = { (radius * sin(currentStack) * sin(currentSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(currentSlice)) };

            pontos.push_back(p0);
            pontos.push_back(p3);
            pontos.push_back(p1);
            pontos.push_back(p2);
            pontos.push_back(p3);
            pontos.push_back(p0);
            //matrix[i][j] = make_tuple(x, y, z);
        }

    }

    glBegin(GL_TRIANGLES);
    for (auto& element : pontos) {
        glVertex3f(get<0>(element), get<1>(element), get<2>(element));
    }
    glEnd();
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

void generate_plane(int length, int divisions, const std::string& filename){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	vector<Triangle> plane_triangles;

	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){

			//Triângulo da metade da esquerda
			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, 0.0f, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, 0.0f, z + lado_length};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, 0.0f, z + lado_length};
			left_triangle.pontos.push_back(coordenadas3);
			plane_triangles.push_back(left_triangle);
			
			//Triângulo da metade da direita
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x + lado_length, 0.0f, z + lado_length};
			right_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas5 = {x + lado_length, 0.0f, z};
			right_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas6 = {x, 0.0f, z};
			right_triangle.pontos.push_back(coordenadas3);
			plane_triangles.push_back(right_triangle);	

			x += lado_length;
			count += 1;
		}
		z += lado_length;
	}

	std::ofstream file("3DFiles/" + filename);

    // Verifica se o arquivo foi aberto corretamente
    if (file.is_open()) {
        // Escreve no arquivo
        file << length << "\n" << divisions << "\n\n\n";

			for(Triangle triangle : plane_triangles){
				//for(Coordenadas ponto : triangle.pontos){
				for(int i = 0; i < triangle.pontos.size(); ++i){
					file << triangle.pontos[i].x << " " << triangle.pontos[i].y << " " << triangle.pontos[i].z;
					    if (i < triangle.pontos.size() - 1) { 
        				    file << " ; ";
        				} else {
        				    file << " "; 
        				}
				}
				file << "\n";
			}
		
        // Fecha o arquivo
        file.close();
        std::cout << "Texto escrito com sucesso no arquivo.\n";
    } else {
        // Se houver algum erro ao abrir o arquivo
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

}

vector<Triangle> parsePlane(const std::string& filename) {
    std::vector<Triangle> triangles;
	std::ifstream file(filename);
	char separador;
    
    std::string linha;
    while (std::getline(file, linha)) {
        std::istringstream iss(linha);
        Coordenadas ponto;
        Triangle triangle;

        while (iss >> ponto.x >> ponto.y >> ponto.z) {
            triangle.pontos.push_back(ponto);
			iss >> separador;
        }
        triangles.push_back(triangle);
    }

	return triangles;
}

void box(int length, int divisions){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	float y = -((float)length / 2.0f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);

	//Base de baixo
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){
			glVertex3f(x + lado_length, y, z + lado_length); 
			glVertex3f(x, y, z + lado_length);
			glVertex3f(x, y, z);  

			glVertex3f(x, y, z);
			glVertex3f(x + lado_length, y, z);  
			glVertex3f(x + lado_length, y, z + lado_length); 

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

			glVertex3f(x, y + lado_length, z + lado_length); 
			glVertex3f(x, y, z + lado_length); 
			glVertex3f(x, y, z); 

			glVertex3f(x, y, z); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x, y + lado_length, z + lado_length); 

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

			glVertex3f(x + lado_length, y + lado_length, z); 
			glVertex3f(x + lado_length, y, z); 
			glVertex3f(x, y, z); 

			glVertex3f(x, y, z); 
			glVertex3f(x, y + lado_length, z); 
			glVertex3f(x + lado_length, y + lado_length, z); 

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

void generate_box(int length, int divisions, const std::string& filename){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	float y = -((float)length / 2.0f);
	vector<vector<Triangle>> box_parts;

	vector<Triangle> bottom;
	vector<Triangle> top;
	vector<Triangle> side1;
	vector<Triangle> side2;
	vector<Triangle> side3;
	vector<Triangle> side4;

	//Base de baixo
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){

			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas3);
			bottom.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x + lado_length, y, z + lado_length};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x + lado_length, y, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			bottom.push_back(right_triangle);	

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
			
			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas3);
			top.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x + lado_length, y, z + lado_length};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x + lado_length, y, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			top.push_back(right_triangle);	

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

			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y + lado_length, z + lado_length};
			left_triangle.pontos.push_back(coordenadas3);
			side1.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x, y + lado_length, z + lado_length};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			side1.push_back(right_triangle);	

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

			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y + lado_length, z + lado_length};
			left_triangle.pontos.push_back(coordenadas3);
			side2.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x, y + lado_length, z + lado_length};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			side2.push_back(right_triangle);	

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

			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x + lado_length, y, z};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y + lado_length, z};
			left_triangle.pontos.push_back(coordenadas3);
			side3.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x + lado_length, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			side3.push_back(right_triangle);	

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

			Triangle left_triangle;

			Coordenadas coordenadas1 = {x, y, z};
			left_triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x + lado_length, y, z};
			left_triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y + lado_length, z};
			left_triangle.pontos.push_back(coordenadas3);
			side4.push_back(left_triangle);
			
			Triangle right_triangle;

			Coordenadas coordenadas4 = {x + lado_length, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas4);
			Coordenadas coordenadas5 = {x, y + lado_length, z};
			right_triangle.pontos.push_back(coordenadas5);
			Coordenadas coordenadas6 = {x, y, z};
			right_triangle.pontos.push_back(coordenadas6);
			side4.push_back(right_triangle);	

			x += lado_length;
			count += 1;
		}
		y += lado_length;
	}

	box_parts.push_back(bottom);
	box_parts.push_back(top);
	box_parts.push_back(side1);
	box_parts.push_back(side2);
	box_parts.push_back(side3);
	box_parts.push_back(side4);

	std::ofstream file("3DFiles/" + filename);

    // Verifica se o arquivo foi aberto corretamente
    if (file.is_open()) {
        // Escreve no arquivo
        file << length << "\n" << divisions << "\n\n\n";

		for(vector<Triangle> triangles : box_parts){
			for(Triangle triangle : triangles){
				for(int i = 0; i < triangle.pontos.size(); ++i){
					file << triangle.pontos[i].x << " " << triangle.pontos[i].y << " " << triangle.pontos[i].z;
					    if (i < triangle.pontos.size() - 1) { 
        				    file << " ; ";
        				} else {
        				    file << " "; 
        				}
				}
				file << "\n";
			}
			file << "\n\n";
		}

        // Fecha o arquivo
        file.close();
        std::cout << "Texto escrito com sucesso no arquivo.\n";
    } else {
        // Se houver algum erro ao abrir o arquivo
        std::cerr << "Erro ao abrir o arquivo.\n";
    }
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

/*
void draw_patches(const std::string& filename) {
    std::vector<vector<Coordenadas>> patches = parsePatches(filename);

	std::vector<Coordenadas> patch = patches[0];
    glBegin(GL_POINTS);
    for (const auto& ponto : patch) {
        glVertex3f(ponto.x, ponto.y, ponto.z);
    }
    glEnd();
}



void draw_patches(const std::string& filename) {
    std::vector<vector<Coordenadas>> patches = parsePatches(filename);

	std::vector<Coordenadas> patch = patches[0];

    glBegin(GL_TRIANGLES);
    // Iterar sobre os pontos em grupos de três para desenhar triângulos
    for (size_t i = 0; i + 2 < patch.size(); i += 3) {
        const Coordenadas& ponto1 = patch[i];
        const Coordenadas& ponto2 = patch[i + 1];
        const Coordenadas& ponto3 = patch[i + 2];
        glVertex3f(ponto1.x, ponto1.y, ponto1.z);
        glVertex3f(ponto2.x, ponto2.y, ponto2.z);
        glVertex3f(ponto3.x, ponto3.y, ponto3.z);
    }
    glEnd();

}
*/

void draw_patches(const std::string& filename) {
    std::vector<std::vector<Coordenadas>> patches = parsePatches(filename);

    const auto& patch = patches[0]; // Acessa o primeiro patch
    
	glBegin(GL_TRIANGLE_STRIP); 
    
	for (size_t i = 0; i + 2 < patch.size(); ++i) {
        const Coordenadas& ponto = patch[i]; 
        glVertex3f(ponto.x, ponto.y, ponto.z);
        // Verifica se há pontos suficientes para formar um triângulo completo
        if (i + 3 < patch.size()) {
            const Coordenadas& ponto2 = patch[i + 1]; 
            const Coordenadas& ponto3 = patch[i + 2]; 
            glVertex3f(ponto2.x, ponto2.y, ponto2.z);
            glVertex3f(ponto3.x, ponto3.y, ponto3.z);
        }
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
	//plane(2,3);
	draw_patches("build/3DFiles/bezier_10.3d");
	//box(2,3);
	//cone(1, 2, 4, 4);
	//sphere(1,20,20);

	//generate_plane(1,3,"plane.3d");
	//generate_box(2,3,"box.3d");
	//generate_cone(1,2,20,20,"cone.3d");

	//std::vector<Triangle> plane_triangles = parsePlane("3DFiles/plane.3d");
	
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


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
	
	glutFullScreen();

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}