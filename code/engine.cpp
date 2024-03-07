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

std::vector<Triangle> parsePlane(const std::string& filename) {
    std::vector<Triangle> triangles;
	std::ifstream file(filename);
	char separador;
    
    std::string linha;
    while (std::getline(file, linha)) {
        std::istringstream iss(linha);
        Coordenadas ponto;
        Triangle triangle;

        while (iss >> ponto.p1 >> ponto.p2 >> ponto.p3) {
            triangle.pontos.push_back(ponto);
			iss >> separador;
        }
        triangles.push_back(triangle);
    }

	return triangles;
}

int main(int argc, char** argv){

	std::vector<Triangle> plane_triangles = parsePlane("3DFiles/plane.3d");

    return 1;
}