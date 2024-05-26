#include <vector>
#include <cmath>
#include <string>
#include "common.hpp"


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

void renderCatmullRomCurve(float* pos, float* deriv, Transform transform, int tesselation) {

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
    
    float gt = 0;
	while (gt < 1) {
		getGlobalCatmullRomPoint(gt, pos, deriv, transform);
		glVertex3f(pos[0], pos[1], pos[2]);
		gt += 1.0 / tesselation;
	}
	glEnd();
}

void processCatmullRomTranslation(Transform transform, bool draw_curve, int tesselation){

    float pos[3], deriv[3];
    float gt = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) / transform.time;


    if(draw_curve)
        renderCatmullRomCurve(pos, deriv, transform, tesselation);
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
