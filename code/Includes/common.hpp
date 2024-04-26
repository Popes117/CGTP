#include <vector>
#include <cmath>

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
class Model{
    public:
    GLuint vbo_id;
    GLuint count;
    std::vector<float> coords;
    Model():vbo_id(), coords(){}
};
  
class Group{
	
	public:

	std::vector<Transform> transforms;
	std::vector<std::string> model_paths;
    std::vector<Model> models;
	std::vector<Group> groups;

	Group():transforms(), model_paths(), models(), groups(){}

};
