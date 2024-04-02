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
#include <cstring>
#include "Includes/common.hpp"

#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, betaa = 0.0f, radius = 5.0f;
float camX, camY, camZ;
using namespace std;

void put_object(vector<vector<Square>> parts, string shape,const std::string& filename){
	std::ofstream file("../build/3DFiles/" + filename);

    if (file.is_open()) {
        file << shape << "\n\n";

		for(vector<Square> triangles : parts){
			for(Square triangle : triangles){
				for(int i = 0; i < triangle.pontos.size(); ++i){
					file << triangle.pontos[i].p1 << " " << triangle.pontos[i].p2 << " " << triangle.pontos[i].p3;
					    if (i < triangle.pontos.size() - 1) { 
        				    file << " ; ";
        				} else {
        				    file << " "; 
        				}
				}
				file << "\n";
			}
			file << "\n";
		}

        file.close();
        std::cout << "Texto escrito com sucesso no ficheiro!\n";
    } else {
        std::cerr << "Erro ao abrir o ficheiro!\n";
    }

}

void generate_plane(int length, int divisions, const std::string& filename){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	vector<Square> plane_triangles;

	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){

			//Triângulo da metade da esquerda
			Square square;

			Coordenadas coordenadas1 = {x, 0.0f, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, 0.0f, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, 0.0f, z + lado_length};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas5 = {x + lado_length, 0.0f, z};

			plane_triangles.push_back(square);

			x += lado_length;
			count += 1;
		}
		z += lado_length;
	}
	vector<vector<Square>> squared;
	squared.push_back(plane_triangles);
	put_object(squared, "plane", filename);
}
/*
void generate_box(int length, int divisions, const std::string& filename){

	float lado_length = (float)length / divisions;
	float z = -((float)length / 2.0f);
	float y = -((float)length / 2.0f);
	vector<vector<Square>> box_parts;

	vector<Square> bottom;
	vector<Square> top;
	vector<Square> side1;
	vector<Square> side2;
	vector<Square> side3;
	vector<Square> side4;

	//Base de baixo
	for(int i = 0; i < divisions; i++){
		float x = -((float)length / 2.0f);
		int count = 0;

		while(count < divisions){

			Square square;

			Coordenadas coordenadas1 = {x, y, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y, z + lado_length};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x + lado_length, y, z};
			square.pontos.push_back(coordenadas4);

			bottom.push_back(square);			

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

    if (file.is_open()) {
        file << "box" << "\n\n";

		for(vector<Triangle> triangles : box_parts){
			for(Triangle triangle : triangles){
				for(int i = 0; i < triangle.pontos.size(); ++i){
					file << triangle.pontos[i].p1 << " " << triangle.pontos[i].p2 << " " << triangle.pontos[i].p3;
					    if (i < triangle.pontos.size() - 1) { 
        				    file << " ; ";
        				} else {
        				    file << " "; 
        				}
				}
				file << "\n";
			}
			file << "\n";
		}

        file.close();
        std::cout << "Texto escrito com sucesso no ficheiro!.\n";
    } else {
        std::cerr << "Erro ao abrir o ficheiro!.\n";
    }
}
*/
void generate_cone(float radius, float height, int slices,int stacks, const std::string& filename) {

	int i;
	float step;
	float n_stacks;
	vector<vector<Square>> cone_parts;
	step = 360.0 / slices;
	n_stacks = height/stacks;

	vector<Square> bottom;
	vector<Square> stack;
	vector<Square> body;
	// bottom
	for (i = 0; i < slices; i++) {

		Square triangle;
		
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

		Square triangle;

		Coordenadas coordenadas1 = {0.0f,height,0.0f};
		triangle.pontos.push_back(coordenadas1);
		Coordenadas coordenadas2 = {cos(i * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin(i * step *M_PI / 180.0)*radius/stacks};
		triangle.pontos.push_back(coordenadas2);
		Coordenadas coordenadas3 = {cos((i + 1) * step * M_PI / 180.0)*radius/stacks, height - n_stacks, -sin((i + 1) * step *M_PI / 180.0)*radius/stacks};
		triangle.pontos.push_back(coordenadas3);
		stack.push_back(triangle);
		
		for(int j = 1; j < stacks; j++){

			//Triângulo da metade da esquerda
			Square triangle;

			Coordenadas coordenadas1 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j)};
			triangle.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin(i * step *M_PI / 180.0)*((radius/stacks)*(j+1))};
			triangle.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {cos((i + 1) * step * M_PI / 180.0)*((radius/stacks)*(j+1)), height - n_stacks*(j+1), -sin((i + 1) * step *M_PI / 180.0)*((radius/stacks)*(j+1))};
			triangle.pontos.push_back(coordenadas3);
			Coordenadas coordenadas5 = {cos(i * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin(i * step *M_PI / 180.0)*((radius/stacks)*j)};
			triangle.pontos.push_back(coordenadas5);

			body.push_back(triangle);	
		}
	}
	cone_parts.push_back(stack);
	cone_parts.push_back(body);

	put_object(cone_parts, "cone", filename);

}

void generate_sphere(double radius, int slices, int stacks, const std::string& filename){
    int i;
    float step;
    float n_stacks;

    step = 2* M_PI / slices; 
    n_stacks = M_PI / stacks;
    vector<Square> sphere_triangles;
 
    // body
    for (i = 0; i < slices; i++) {
    const double currentSlice = i * step;
    for (int j = 0; j < stacks; j++) {
        const double currentStack = j * n_stacks;
        const double nextStack = currentStack + n_stacks;
        const double nextSlice = currentSlice + step;

            Coordenadas p0 = { (radius * sin(nextStack) * sin(nextSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(nextSlice))};
            Coordenadas p1 = { (radius * sin(nextStack) * sin(currentSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(currentSlice)) };
            Coordenadas p2 = { (radius * sin(currentStack) * sin(nextSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(nextSlice)) };
            Coordenadas p3 = { (radius * sin(currentStack) * sin(currentSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(currentSlice)) };

            Square square;

            square.pontos.push_back(p0);
            square.pontos.push_back(p3);
            square.pontos.push_back(p1);
            square.pontos.push_back(p2);

			sphere_triangles.push_back(square);	
        }
    }

	vector<vector<Square>> squared;
	squared.push_back(sphere_triangles);
	put_object(squared, "sphere", filename);
}

int main(int argc, char** argv){

    if (argc < 2) {
        std::cout << "Falta de Argumentos!";
        return 1;
    }

    if (strcmp(argv[1], "plane") == 0) {
		generate_plane(std::stoi(argv[2]),std::stoi(argv[3]),argv[4]);
	}
	else if (strcmp(argv[1], "box") == 0) {
		//generate_box(std::stoi(argv[2]),std::stoi(argv[3]),argv[4]);
	}
	else if (strcmp(argv[1], "cone") == 0) {
		generate_cone(std::stof(argv[2]), std::stof(argv[3]),std::stoi(argv[4]), std::stoi(argv[5]),argv[6]);
	}
	else if (strcmp(argv[1], "sphere") == 0) {
		generate_sphere(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]),argv[5]);
	}
	else{
        std::cout << "Erro ao abrir o ficheiro!\n";
		return 0;
	}

	return 1;
}