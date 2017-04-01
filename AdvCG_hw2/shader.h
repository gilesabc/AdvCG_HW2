#include"algebra3.h"
#include"ppm.h"
#include<vector>

struct Triangle{
    vec3 pt, v1, v2;
};

struct Sphere{
    vec3 c;
    float r;
};

class Shader{

    std::vector<Triangle> triangle;
    std::vector<Sphere> sphere;
    vec3 eye, view;
    float angle, ratio;
    vec3 canvas[5];
    ColorImage image;

public:
    void inputFile(char *filename);
    void setCanvas(float eye_distance, vec3 lookUp);
    bool tracing(vec3 pixel);
    void outputFile(char *filename);
};

