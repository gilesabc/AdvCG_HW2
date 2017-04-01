#include"shader.h"
#include<iostream>

using namespace std;

int main(int argc, char* argv[])
{
	Shader shader;
	shader.inputFile("input.txt");
	vec3 up = {0,1,0};
	shader.setCanvas(0.5,up);
	shader.outputFile("AdvCG.ppm");
	return 0;
}

