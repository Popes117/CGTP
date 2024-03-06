#include <stdio.h>
#include <stdlib.h>

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

void sphere(double radius, int slices, int stacks) {

	int i;
	float step;
	float n_stacks;
	//tuple<float, float, float>** matrix = new tuple<float, float, float>* [stacks];
	//for (int i = 0; i < stacks; ++i)
		//matrix[i] = new tuple<float, float, float>[slices];
		
	
	step = 360.0 / slices; 
	n_stacks = 180 / stacks;
	vector<tuple<float, float, float>> pontos;
 
	// body
	for (i = 0; i < stacks; i++) {
		float lat = n_stacks*i;
		for (int j = 0; j < slices; j++) {
			double lon = j * step;

			const double currentStack = i * lat;
			const double currentSlice = j * lon;
			const double nextStack = currentStack + lat;
			const double nextSlice = currentSlice + lon;

			tuple<double, double, double> p0 = { (radius * sin(nextStack) * sin(nextSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(nextSlice))};
			tuple<double, double, double> p1 = { (radius * sin(nextStack) * sin(currentSlice)), (radius * cos(nextStack)), (radius * sin(nextStack) * cos(currentSlice)) };
			tuple<double, double, double> p2 = { (radius * sin(currentStack) * sin(nextSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(nextSlice)) };
			tuple<double, double, double> p3 = { (radius * sin(currentStack) * sin(currentSlice)), (radius * cos(currentStack)), (radius * sin(currentStack) * cos(currentSlice)) };

			pontos.push_back(p0);
			pontos.push_back(p1);
			pontos.push_back(p3);
			pontos.push_back(p2);
			pontos.push_back(p3);
			pontos.push_back(p0);
			//matrix[i][j] = make_tuple(x, y, z);
		}

	}


	for (i = 0; i < stacks-1; i++) {
		glBegin(GL_TRIANGLES);
		for (int j = 1; j < slices-1; j++) {

			//Triângulo da metade da esquerda
			//glVertex3f(get<0>(matrix[i+1][j+1]), get<1>(matrix[i+1][j+1]), get<2>(matrix[i+1][j+1]));
			//Triângulo da metade da direita
			//glVertex3f(get<0>(matrix[i][j]), get<1>(matrix[i][j]), get<2>(matrix[i][j]));
			//glVertex3f(get<0>(matrix[i+1][j]), get<1>(matrix[i+1][j]), get<2>(matrix[i+1][j]));
			//glVertex3f(get<0>(matrix[i][j+1]), get<1>(matrix[i][j+1]), get<2>(matrix[i][j+1]));
			//glVertex3f(get<0>(matrix[i][j]), get<1>(matrix[i][j]), get<2>(matrix[i][j]));
			//glVertex3f(get<0>(matrix[i + 1][j + 1]), get<1>(matrix[i + 1][j + 1]), get<2>(matrix[i + 1][j + 1]));
			}
		glEnd();
	}

	glBegin(GL_TRIANGLES);
	for (auto& element : pontos) {
		glVertex3f(get<0>(element), get<1>(element), get<2>(element));
	}
	glEnd();
	//for (int i = 0; i < slices; ++i)
		//delete[] matrix[i];
	//delete[] matrix;
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
	//cone(1, 2, 20, 20);
	sphere(1,30,30);

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
