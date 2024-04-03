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

struct Transform{
	string type;
	float angle, x, y, z;
};