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
	//std::ofstream file("../build/3DFiles/" + filename);
	std::ofstream file("../build/3DFiles/" + filename);

    if (file.is_open()) {
        file << shape << "\n\n";

		for(vector<Square> triangles : parts){
			for(Square triangle : triangles){
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
			file << "\n";
		}

        file.close();
        std::cout << "Texto escrito com sucesso no ficheiro!\n";
    } else {
        std::cerr << "Erro ao abrir o ficheiro!\n";
    }

}

void put_patches(vector<vector<Coordenadas>> patches, string shape,const std::string& filename){
	std::ofstream file("../build/3DFiles/" + filename);

    if (file.is_open()) {
        file << shape << "\n\n";

		for(vector<Coordenadas> triangles : patches){
			for(Coordenadas coordenadas : triangles){
				file << coordenadas.x << " " << coordenadas.y << " " << coordenadas.z << " ; ";
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
			square.pontos.push_back(coordenadas5);

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

			Coordenadas coordenadas1 = {x + lado_length, y, z + lado_length};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y, z};
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
			
			Square square;

			Coordenadas coordenadas1 = {x, y, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y, z + lado_length};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x + lado_length, y, z};
			square.pontos.push_back(coordenadas4);
			
			top.push_back(square);
			
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

			Square square;

			Coordenadas coordenadas1 = {x, y, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y + lado_length, z + lado_length};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x, y + lado_length, z};
			square.pontos.push_back(coordenadas4);
			
			side1.push_back(square);	

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

			Square square;

			Coordenadas coordenadas1 = {x, y + lado_length, z + lado_length};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x, y, z + lado_length};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y, z};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x, y + lado_length, z};
			square.pontos.push_back(coordenadas4);
			
			side2.push_back(square);

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

			Square square;

			Coordenadas coordenadas1 = {x + lado_length, y + lado_length, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x + lado_length, y, z};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x, y, z};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x, y + lado_length, z};
			square.pontos.push_back(coordenadas4);

			side3.push_back(square);
			
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

			Square square;

			Coordenadas coordenadas1 = {x, y, z};
			square.pontos.push_back(coordenadas1);
			Coordenadas coordenadas2 = {x + lado_length, y, z};
			square.pontos.push_back(coordenadas2);
			Coordenadas coordenadas3 = {x + lado_length, y + lado_length, z};
			square.pontos.push_back(coordenadas3);
			Coordenadas coordenadas4 = {x, y + lado_length, z};
			square.pontos.push_back(coordenadas4);

			side4.push_back(square);

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

	put_object(box_parts, "box", filename);

}

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
			Coordenadas coordenadas4 = {cos((i+1) * step * M_PI / 180.0)*((radius/stacks)*j), height - n_stacks*j, -sin((i+1) * step *M_PI / 180.0)*((radius/stacks)*j)};
			triangle.pontos.push_back(coordenadas4);
			
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

std::string readLineinFile(std::string const &fileInput, int lineNumber){
    std::string resultado;
    std::ifstream MyReadFile(fileInput);
    int lineNumberLido = 1;

    while (getline(MyReadFile, resultado))
    {
        if (lineNumber == lineNumberLido)
        {
            break;
        }
        else
        {
            lineNumberLido++;
        }
    }

    return resultado;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

//Recebe apenas 1 linha da matriz de Bezier
Coordenadas multMatrixVector(float c[4], Coordenadas matrixPontos[4])
{
    Coordenadas pontosInt[4], res;
    for (int i = 0; i < 4; i++)
    {
        res = matrixPontos[i];
        res.multByFloat(c[i]);
        pontosInt[i] = res;
    }
    Coordenadas final = Coordenadas(0.0, 0.0, 0.0);
    for (int k = 0; k < 4; final.addCoords(pontosInt[k++]));
    return final;
}

void generate_patches(const std::string& fileInput, int tesselation, const std::string& fileOutput){

    int nPontos = 0;
    int lineNumber;
    int it = 0;
    int column = 0;
    int line = 0;
	std::string newFileInput = "patches/" + fileInput;
    int nPatches = std::stoi(readLineinFile(newFileInput, 1));
    int nControlPoints = std::stoi(readLineinFile(newFileInput, nPatches + 2));
    std::vector<Coordenadas> texturas;
    std::vector<Coordenadas> normais;
    std::vector<Coordenadas> coordenadas;
	std::vector<vector<Coordenadas>> patches;

    Coordenadas arrayVertices[nControlPoints];
    int arrayPatches[nPatches * 4][4];

	// Parse do ficheiro de patches

    std::string myText;
    std::ifstream MyReadFile(newFileInput);

    while (getline(MyReadFile, myText))
    {
        if (it == 0)
        {
            lineNumber = std::stoi(myText);
            it++;
        }
        else if (it >= 1 && it <= lineNumber)
        {
            std::vector<std::string> res = split(myText, ',');
            for (auto i : res)
            {
                if (column < 4)
                {
                    arrayPatches[line][column] = std::stoi(i);
                    column++;
                }
                else
                {
                    line++;
                    column = 0;
                    arrayPatches[line][column] = std::stoi(i);
                    column++;
                }
            }
            it++;
        }
        else if (it > lineNumber + 1)
        {
            std::vector<std::string> res = split(myText, ',');
			Coordenadas coordenadas = {std::stof(res[0]), std::stof(res[1]), std::stof(res[2])};
            arrayVertices[nPontos] = coordenadas;
            nPontos++;
            it++;
        }
        else
        {
            it++;
        }
    }
    MyReadFile.close();

	// Tratamento da informação do parse

    Coordenadas pontosFinais[tesselation+1][tesselation+1], normaisFinais[tesselation+1][tesselation+1];
    float bezierMatrix[4][4] = {{-1, 3, -3, 1},
                              	{3, -6, 3, 0},
                              	{-3, 3, 0, 0},
                              	{1, 0, 0, 0}};
    int linhasPatch = 0;

    while (linhasPatch != nPatches * 4){
        
		Coordenadas pontosPatch[4][4] = {{arrayVertices[arrayPatches[linhasPatch][0]], arrayVertices[arrayPatches[linhasPatch + 1][0]], arrayVertices[arrayPatches[linhasPatch + 2][0]], arrayVertices[arrayPatches[linhasPatch + 3][0]]},
                      					 {arrayVertices[arrayPatches[linhasPatch][1]], arrayVertices[arrayPatches[linhasPatch + 1][1]], arrayVertices[arrayPatches[linhasPatch + 2][1]], arrayVertices[arrayPatches[linhasPatch + 3][1]]},
                      					 {arrayVertices[arrayPatches[linhasPatch][2]], arrayVertices[arrayPatches[linhasPatch + 1][2]], arrayVertices[arrayPatches[linhasPatch + 2][2]], arrayVertices[arrayPatches[linhasPatch + 3][2]]},
                      					 {arrayVertices[arrayPatches[linhasPatch][3]], arrayVertices[arrayPatches[linhasPatch + 1][3]], arrayVertices[arrayPatches[linhasPatch + 2][3]], arrayVertices[arrayPatches[linhasPatch + 3][3]]}};
        Coordenadas matrix_MP[4][4];
	    
		for (auto i = 0; i < 4; i++)
        {
            for (auto j = 0; j < 4; j++)
            {
                matrix_MP[i][j] = multMatrixVector(bezierMatrix[i], pontosPatch[j]);
            }
        }

        Coordenadas matrix_MPM[4][4];
        for (auto i = 0; i < 4; i++)
        {
            for (auto j = 0; j < 4; j++)
            {
                matrix_MPM[i][j] = multMatrixVector(bezierMatrix[j], matrix_MP[i]);
            }
        }
        Coordenadas matrix_MPM_trans[4][4];
        for (auto i = 0; i < 4; i++)
        {
            for (auto j = 0; j < 4; j++)
            {
                matrix_MPM_trans[i][j] = matrix_MPM[j][i];
            }
        }

        for (int u = 0; u <= tesselation; u++)
        {
            float uf = float(u) / tesselation;
            float u_vetor[4] = {uf * uf * uf, uf * uf, uf, 1};
            float u_vetor_deriv[4] = {uf * uf * 3, uf * 2, 1, 0};
            for (int v = 0; v <= tesselation; v++)
            {
                float vf = float(v) / tesselation;
                float v_vetor[4] = {vf * vf * vf, vf * vf, vf, 1};
                float v_vetor_deriv[4] = {vf * vf * 3, vf * 2, 1, 0};

                Coordenadas u_vetorXcalculada[4], puv;
                Coordenadas u_vetorXcalculada_uderiv[4], puv_uderiv, puv_vderiv;
                for (auto i = 0; i < 4; i++)
                {
                    u_vetorXcalculada[i] = multMatrixVector(u_vetor, matrix_MPM_trans[i]);
                    u_vetorXcalculada_uderiv[i] = multMatrixVector(u_vetor_deriv, matrix_MPM_trans[i]);
                }
                puv = multMatrixVector(v_vetor, u_vetorXcalculada);
                pontosFinais[u][v] = puv;
                puv_uderiv = multMatrixVector(v_vetor, u_vetorXcalculada_uderiv);
                puv_vderiv = multMatrixVector(v_vetor_deriv, u_vetorXcalculada);
                Coordenadas normal = puv_vderiv.get_cross_product(puv_uderiv);
                normal.normalize();
                normaisFinais[u][v] = normal;
            }
        }
        
        for (int i = 0; i <= tesselation; i++)
            for (int k = 0; k <= tesselation; k++){
                texturas.push_back(Coordenadas(1-(k/(float)tesselation), (i/(float)tesselation), 0));
                normais.push_back(normaisFinais[i][k]);
                coordenadas.push_back(pontosFinais[i][k]);
            } 
        linhasPatch += 4;
    }

	patches.push_back(coordenadas);
	patches.push_back(normais);
	patches.push_back(texturas);

/*
    for (int k = 0; k < nPatches; k++)
    {
        for (int l = 0; l < tesselation; l++)
        {
            for(int c = 1 ;c < tesselation +1 ;c++){
                Coordenadas indexes = Coordenadas(((c+l*(tesselation+1)) + 1) + k * (tesselation+1)*(tesselation+1),
                                    (c+l*(tesselation+1)) + (tesselation+1) + (k * (tesselation+1)*(tesselation+1)),
                                    ((c+l*(tesselation+1))) + (k * (tesselation+1)*(tesselation+1)));
                m.pushFace(face(
                    vertex_ref(indexes.x, indexes.x, indexes.x),
                    vertex_ref(indexes.y, indexes.y, indexes.y),
                    vertex_ref(indexes.z, indexes.z, indexes.z)
                ));
                indexes = Coordenadas(((c+l*(tesselation+1)) + 1) + k * (tesselation+1)*(tesselation+1),
                               ((c+l*(tesselation+1)) + (tesselation+2)) + k * (tesselation+1)*(tesselation+1),
                               ((c+l*(tesselation+1)) + (tesselation+1)) + k * (tesselation+1)*(tesselation+1));
                m.pushFace(face(
                    vertex_ref(indexes.x, indexes.x, indexes.x),
                    vertex_ref(indexes.y, indexes.y, indexes.y),
                    vertex_ref(indexes.z, indexes.z, indexes.z)
                ));
            }
        }
    }
    */

   put_patches(patches, "patch", fileOutput);
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
		generate_box(std::stoi(argv[2]),std::stoi(argv[3]),argv[4]);
	}
	else if (strcmp(argv[1], "cone") == 0) {
		generate_cone(std::stof(argv[2]), std::stof(argv[3]),std::stoi(argv[4]), std::stoi(argv[5]),argv[6]);
	}
	else if (strcmp(argv[1], "sphere") == 0) {
		generate_sphere(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]),argv[5]);
	}
	else if (strcmp(argv[1], "patch") == 0) {
		generate_patches(argv[2], std::stoi(argv[3]),argv[4]);
	}
	else{
		std::cout << "Main" << "\n";
        std::cout << "Erro ao abrir o ficheiro!\n";
		return 0;
	}

	return 1;
}