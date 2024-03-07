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
	double p1, p2, p3;
};

struct Triangle{
	std::vector<Coordenadas> pontos;
};

float alfa = 0.0f, betaa = 0.0f, radius = 5.0f;
float camX, camY, camZ;
using namespace std;

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
					file << triangle.pontos[i].p1 << " " << triangle.pontos[i].p2 << " " << triangle.pontos[i].p3;
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
					file << triangle.pontos[i].p1 << " " << triangle.pontos[i].p2 << " " << triangle.pontos[i].p3;
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
					file << triangle.pontos[i].p1 << " " << triangle.pontos[i].p2 << " " << triangle.pontos[i].p3;
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

void main(int argc, char** argv){

	generate_plane(1,3,"plane.3d");
	generate_box(2,3,"box.3d");
	generate_cone(1,2,20,20,"cone.3d");

}