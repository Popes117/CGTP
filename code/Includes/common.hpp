#include <vector>
#include <cmath>
#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <string>

float dark[4] = {0.2, 0.2, 0.2, 1.0};
float white[4] = {1.0, 1.0, 1.0, 1.0};
float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};

struct Coordenadas{
	double x, y, z;

    Coordenadas(float x, float y, float z) : x(x), y(y), z(z) {
    }
    Coordenadas(){

    }
    void addCoords(Coordenadas w){
        x += w.x;
        y += w.y;
        z += w.z;
    }
    void multByFloat(float w){
        x *= w;
        y *= w;
        z *= w;
    }
    void normalize(){
        float vLen = sqrtf(x*x+y*y+z*z);
        x = x / vLen;
        y = y / vLen;
        z = z / vLen;
    };
    Coordenadas get_cross_product(Coordenadas& v){
        this->normalize();
        v.normalize();
        return Coordenadas((this->y*v.z) - (this->z*v.y), (this->z*v.x) - (this->x*v.z), (this->x*v.y) - (this->y*v.x));
    };	
};

struct Triangle{
	std::vector<Coordenadas> pontos;
};

struct Square{
	std::vector<Coordenadas> pontos;
};

struct Matrix {
    GLfloat data[16];
};

struct Transform {
    std::string type;
    float angle, x, y, z, time;
    bool align;
    std::vector<Coordenadas> pontos;

    Transform(std::string type, float angle, float x, float y, float z) :type(type), angle(angle), x(x), y(y), z(z) {}
    Transform(std::string type, float time, bool align, std::vector<Coordenadas> pontos) :type(type), time(time), align(align), pontos(pontos) {}

};

class Texture{
    public:
        GLuint texture_id;
        unsigned int t, tw, th;
        unsigned int texID;
        unsigned char *texData;
        std::string path;
    public:
        Texture(){};

        Texture(std::string pathName){
            path = pathName;
        }

    void prep(){

        ilInit();
	    ilEnable(IL_ORIGIN_SET);
	    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        ilGenImages(1,&t);
        ilBindImage(t);
        //ilLoadImage(path.data());
        if (!ilLoadImage(path.data())) {
            std::cerr << "Erro ao carregar a imagem: " << path << std::endl;
            return;
        }
        std::cout << path << std::endl;
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        GLenum err;
        glGenTextures(1, &texID);
        if ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Erro ao gerar textura: " << gluErrorString(err) << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, texID);
        if ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Erro ao vincular textura: " << gluErrorString(err) << std::endl;
        }

        //glGenTextures(1,&tex);
        //glBindTexture(GL_TEXTURE_2D,tex);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        //glGenerateMipmap(GL_TEXTURE_2D);

        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Erro ao configurar parâmetros de textura: " << gluErrorString(err) << std::endl;
            return;
        }
        std::cout << "Textura carregada com sucesso" << std::endl; //Não faz este print

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Erro ao carregar dados da textura: " << gluErrorString(err) << std::endl;
            return;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Erro ao gerar mipmap: " << gluErrorString(err) << std::endl;
            return;
        }
    }

    void apply(){
        glBindTexture(GL_TEXTURE_2D, texID);
    }

};

#endif

class Color{
    public:
        Coordenadas diffuse, ambient, specular, emissive;
        bool hasDiffuse, hasAmbient, hasSpecular, hasEmissive, hasShininess;
        float shininess;
    public:
        Color(){
            hasDiffuse = false;
            hasAmbient = false;
            hasSpecular = false;
            hasEmissive = false;
        }

    void apply(){
        float diffuseArr[4] = {diffuse.x/255, diffuse.y/255, diffuse.z/255, 1.0};
        float ambientArr[4] = {ambient.x, ambient.y, ambient.z, 1.0};
        float specularArr[4] = {specular.x, specular.y, specular.z, 1.0};
        float emissiveArr[4] = {emissive.x, emissive.y, emissive.z, 1.0};
        if (hasDiffuse) glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
        if (hasAmbient) glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
        if (hasSpecular) glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
        if (hasEmissive) glMaterialfv(GL_FRONT, GL_EMISSION, emissiveArr);
        if (hasShininess) glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }        
};

class Light{
    public:
    Coordenadas position;
    Coordenadas direction;
    float cutoff;
    bool hasPosition, hasDirection;
    GLenum gl_light;

    Light(){
        hasPosition = false;
        hasDirection = false;
    }

    void prep(){
        glEnable(gl_light);
        glLightfv(gl_light, GL_AMBIENT, white);
        glLightfv(gl_light, GL_DIFFUSE, white);
        glLightfv(gl_light, GL_SPECULAR, white);
    }

    void apply(){
        if (hasDirection && hasPosition) {
            float posArr[4] = {position.x, position.y, position.z, 1};
            glLightfv(gl_light, GL_POSITION, posArr);
            float dirArr[3] = {direction.x, direction.y, direction.z};
            glLightfv(gl_light, GL_SPOT_DIRECTION, dirArr);
            glLightfv(gl_light, GL_SPOT_CUTOFF, &cutoff);
        }
        else if (hasPosition){
            float posArr[4] = {position.x, position.y, position.z, 1};
            glLightfv(gl_light, GL_POSITION, posArr);
        }
        else if (hasDirection){
            float dirArr[4] = {direction.x, direction.y, direction.z, 0};
            glLightfv(gl_light, GL_POSITION, dirArr);
        }
    }
};

class Model{
    public:
        bool hasTexture, hasColor;
        GLuint vbo_ids[3];
        GLuint count;
        std::vector<std::vector<float>> coords;
        Texture texture;
        Color color;
        Model():vbo_ids(), coords(), count(), texture(), color(){}
};
  
class Group{
	
	public:

	std::vector<Transform> transforms;
	std::vector<std::string> model_paths;
    std::vector<Model> models;
	std::vector<Group> groups;

	Group():transforms(), model_paths(), models(), groups(){}

};
