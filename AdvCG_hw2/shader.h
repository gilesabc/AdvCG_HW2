#include"algebra3.h"
#include"ppm.h"
#include<vector>

struct Material {
	vec3 RGB;
	float Ka;
	float Kd;
	float Ks;
	float exp;
	float Reflect;
	float Refract;
	float Nr;
};
struct Triangle{
    vec3 pt, v1, v2;
	Material m;
};

struct Sphere{
    vec3 c;
	Material m;
    float r;
};

class Shader{
    std::vector<Triangle> triangle;
    std::vector<Sphere> sphere;
    vec3 eye, view;
	float lightPos_x;
	float lightPos_y;
	float lightPos_z;
    float angle, ratio;
    vec3 canvas[5];
    ColorImage image;

public:
    void inputFile(char *filename);
    void setCanvas(float eye_distance, vec3 lookUp);
	Pixel tracing(vec3 pixel);
	Pixel traceST(vec3 pixel, vec3 vector);
	vec3 reflect(vec3 normal, vec3 incident);
	vec3 refract(vec3 normal, vec3 incident, double n1, double n2);
    void outputFile(char *filename);
};

