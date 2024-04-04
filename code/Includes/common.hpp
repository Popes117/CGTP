#include <vector>

struct Coordenadas{
	double p1, p2, p3;
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

struct Transform{
	std::string type;
	float angle, x, y, z;

	Transform(std::string type, float angle, float x, float y, float z):type(type), angle(angle), x(x), y(y), z(z){}
};

class Group{
	
	public:

	std::vector<Transform> transforms;
	std::vector<std::string> model_paths;
	std::vector<Group> groups;

	Group():transforms(), model_paths(), groups(){}

};
