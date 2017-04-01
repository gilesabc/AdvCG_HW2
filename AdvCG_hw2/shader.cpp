#include"shader.h"
#include<fstream>
#include <string>
#include <sstream>
#include<iostream>
#include<math.h>

using namespace std;

void Shader::inputFile(char *filename){

    fstream inputfile;
    inputfile.open(filename,fstream::in);
    string str;
    stringstream ss;
    while(inputfile >> str){
        if(str == "E"){
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> eye[i];
                ss.clear();
            }
        }
        else if(str == "V"){
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> view[i];
                ss.clear();
            }
            view.normalize();
        }
        else if(str == "F"){
            inputfile >> str;
            ss << str;
            ss >> angle;
            ss.clear();
        }
        else if(str == "R"){
            int x,y;
            inputfile >> str;
            ss << str;
            ss >> x;
            ss.clear();
            inputfile >> str;
            ss << str;
            ss >> y;
            ss.clear();
            image.init(x,y);
            ratio = (float)x/y;
        }
		/*else if (str == "M") {
			int x, y;
			inputfile >> str;
			ss << str;
			ss >> x;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> y;
			ss.clear();
			image.init(x, y);
			ratio = (float)x / y;
		}*/
        else if(str == "S"){
            Sphere sph;
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> sph.c[i];
                ss.clear();
            }
            inputfile >> str;
            ss << str;
            ss >> sph.r;
            ss.clear();
            sphere.push_back(sph);
        }
        else if(str == "T"){
            Triangle tri;
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> tri.pt[i];
                ss.clear();
            }
            vec3 v;
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> v[i];
                ss.clear();
            }
            tri.v1 = v - tri.pt;
            for(int i=0; i<3; i++){
                inputfile >> str;
                ss << str;
                ss >> v[i];
                ss.clear();
            }
            tri.v2 = v - tri.pt;
            triangle.push_back(tri);
        }
    }
}

void Shader::setCanvas(float eye_distance, vec3 lookUp){

    lookUp.normalize();
    vec3 left = lookUp^view;
    vec3 up = view^left;
    float width = eye_distance*tanh(angle);
    float height = width/ratio;

    canvas[0] = eye + eye_distance * view;
    canvas[1] = canvas[0] + left*width + up*height;
    canvas[2] = canvas[0] - left*width + up*height;
    canvas[3] = canvas[0] + left*width - up*height;
    canvas[4] = canvas[0] - left*width - up*height;
}

bool Shader::tracing(vec3 pixel){

    vec3 trace = pixel - eye;
    vec3 trace_normal = trace;
    trace_normal.normalize();


    for(int i=0;i<sphere.size();i++){//tracing sphere
        float a = trace[0]*trace[0]+trace[1]*trace[1]+trace[2]*trace[2];
        float b = 2*eye[0]*trace[0]-2*trace[0]*sphere[i].c[0]+2*eye[1]*trace[1]-2*trace[1]*sphere[i].c[1]+2*eye[2]*trace[2]-2*trace[2]*sphere[i].c[2];
        float c = eye[0]*eye[0]+sphere[i].c[0]*sphere[i].c[0]-2*eye[0]*sphere[i].c[0]+eye[1]*eye[1]+sphere[i].c[1]*sphere[i].c[1]-2*eye[1]*sphere[i].c[1]+eye[2]*eye[2]+sphere[i].c[2]*sphere[i].c[2]-2*eye[2]*sphere[i].c[2]-sphere[i].r*sphere[i].r;
        float check = b*b - 4*a*c;
        if(check>=0)
		{
			float intersection_1 = ((-1)*b + sqrt(check)) / (2 * a);
			float intersection_2 = ((-1)*b - sqrt(check)) / (2 * a);
			//cout << "intersection1:" << intersection_1 << " " << "intersection2:" << intersection_2 << endl;
			float tmp, t;
			tmp = intersection_1 < intersection_2 ? intersection_1 : intersection_2;
			if (tmp >= 1)
			{
				t = tmp;
				cout << t << endl;
			}

			return true;
		}
    }

    for(int i=0; i<triangle.size(); i++){//tracing triangle

        mat3 matrix(triangle[i].v1, triangle[i].v2, trace*-1);
        matrix = matrix.transpose();
        matrix = matrix.inverse();

        vec3 vect = eye - triangle[i].pt;
        vect = matrix*vect;

        if(vect[0]>0 && vect[1]>0 && vect[2]>1 && (vect[0]+vect[1])<=1)
            return true;
    }
    return false;
}

void Shader::outputFile(char *filename){

    Pixel white = {255,255,255}, black = {0,0,0};

    vec3 canvas_x = (canvas[2] - canvas[1])/image.xRes;
    vec3 canvas_y = (canvas[3] - canvas[1])/image.yRes;
    vec3 pixel_coord = canvas[1] + canvas_x/2 + canvas_y/2;

    for (int y=0; y<image.yRes; y++) {
		for (int x=0; x<image.xRes; x++) {
            if(tracing(pixel_coord + canvas_x*x + canvas_y*y))
                image.writePixel(x, y, black);
            else
                image.writePixel(x, y, white);
		}
	}
	image.outputPPM(filename);
}

