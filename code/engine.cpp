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

struct Coordenadas{
	double p1, p2, p3;
};

struct Triangle{
	std::vector<Coordenadas> pontos;
};

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

    std::cout << "Length: " << length << "\n";
    std::cout << "Divisions: " << divisions << "\n";
    std::cout << "Bottom triangles: " << triangles.size() << "\n";

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

    std::cout << radius << "\n";
    std::cout << height << "\n";
    std::cout << slices << "\n";
    std::cout << stacks << "\n";
    std::cout << "Bottom triangles: " << triangles[0].size() << "\n";
    std::cout << "Top triangles: " << triangles[1].size() << "\n";
    std::cout << "Side1 triangles: " << triangles[2].size() << "\n";

    return triangles;
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

int main(int argc, char** argv){

	//std::vector<Triangle> plane_triangles = parsePlane("build/3DFiles/plane.3d");
	//std::vector<std::vector<Triangle>> box_triangles = parseBox("build/3DFiles/box.3d");
	std::vector<std::vector<Triangle>> cone_triangles = parseCone("build/3DFiles/cone.3d");
    //printMuitosTriangles(cone_triangles);

    return 1;
}