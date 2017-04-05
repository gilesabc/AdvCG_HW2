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
	Material mat;
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
		else if (str == "M") {
			inputfile >> str;
			ss << str;
			ss >> mat.RGB[0];
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.RGB[1];
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.RGB[2];
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Ka;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Kd;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Ks;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.exp;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Reflect;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Refract;
			ss.clear();
			inputfile >> str;
			ss << str;
			ss >> mat.Nr;
			ss.clear();
			//cout << mat.RGB[1] << " "<< mat.RGB[1] << " " <<
		}
        else if(str == "S"){
            Sphere sph;
			sph.m.RGB[0] = mat.RGB[0];
			sph.m.RGB[1] = mat.RGB[1];
			sph.m.RGB[2] = mat.RGB[2];
			sph.m.Ka = mat.Ka;
			sph.m.Kd = mat.Kd;
			sph.m.Ks = mat.Ks;
			sph.m.exp = mat.exp;
			sph.m.Reflect = mat.Reflect;
			sph.m.Refract = mat.Refract;
			sph.m.Nr = mat.Nr;
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
			tri.m.RGB[0] = mat.RGB[0];
			tri.m.RGB[1] = mat.RGB[1];
			tri.m.RGB[2] = mat.RGB[2];
			tri.m.Ka = mat.Ka;
			tri.m.Kd = mat.Kd;
			tri.m.Ks = mat.Ks;
			tri.m.exp = mat.exp;
			tri.m.Reflect = mat.Reflect;
			tri.m.Refract = mat.Refract;
			tri.m.Nr = mat.Nr;
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
		else if (str == "L") {
				inputfile >> str;
				for (int i = 0;i < str.size();i++)
				{
					if (str[i] == ',')
					{
						str[i] = ' ';
					}
				}
				ss << str;
				ss >> lightPos_x;
				ss.clear();
				inputfile >> str;
				for (int i = 0;i < str.size();i++)
				{
					if (str[i] == ',')
					{
						str[i] = ' ';
					}
				}
				ss << str;
				ss >> lightPos_y;
				ss.clear();
				inputfile >> str;
				for (int i = 0;i < str.size();i++)
				{
					if (str[i] == ',')
					{
						str[i] = ' ';
					}
				}
				ss << str;
				ss >> lightPos_z;
				ss.clear();
		}
		//cout << "xyz:"<< lightPos_x<<" "<<lightPos_y<<" "<<lightPos_z <<endl;
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

Pixel  Shader::tracing(vec3 pixel){
	vec3 trace = pixel - eye;
	vec3 trace_normal = trace;
	trace_normal.normalize();
	return traceST(pixel,trace);
}

Pixel Shader::traceST(vec3 pixel, vec3 vector) {
	vec3 Ia, Id, Is;
	vec3 trace = pixel - eye;
	vec3 trace_normal = trace;
	trace_normal.normalize();
	vec3 light;
	light[0] = lightPos_x;
	light[1] = lightPos_y;
	light[2] = lightPos_z;
	float t_min = 0;
	Material closerMat;
	for (int i = 0;i < sphere.size();i++) {//tracing sphere
		float a = trace[0] * trace[0] + trace[1] * trace[1] + trace[2] * trace[2];
		float b = 2 * eye[0] * trace[0] - 2 * trace[0] * sphere[i].c[0] + 2 * eye[1] * trace[1] - 2 * trace[1] * sphere[i].c[1] + 2 * eye[2] * trace[2] - 2 * trace[2] * sphere[i].c[2];
		float c = eye[0] * eye[0] + sphere[i].c[0] * sphere[i].c[0] - 2 * eye[0] * sphere[i].c[0] + eye[1] * eye[1] + sphere[i].c[1] * sphere[i].c[1] - 2 * eye[1] * sphere[i].c[1] + eye[2] * eye[2] + sphere[i].c[2] * sphere[i].c[2] - 2 * eye[2] * sphere[i].c[2] - sphere[i].r*sphere[i].r;
		float check = b*b - 4 * a*c;
		if (check >= 0)
		{
			float intersection_1 = ((-1)*b + sqrt(check)) / (2 * a);
			float intersection_2 = ((-1)*b - sqrt(check)) / (2 * a);
			//cout << "intersection1:" << intersection_1 << " " << "intersection2:" << intersection_2 << endl;
			float tmp, t;
			tmp = intersection_1 < intersection_2 ? intersection_1 : intersection_2;
			if (tmp >= 1)
			{
				t = tmp;
				if (t < t_min) 
				{
					t_min = t;
					closerMat = sphere[i].m;
				}
				vec3 normal = (eye + t * trace) - sphere[i].c;
				vec3 Ii = sphere[i].m.RGB;
				Ia = sphere[i].m.RGB;
				vec3 L = light - (eye + t * trace_normal);
				L.normalize();
				float NDotL = normal * L;
				if (NDotL < 0)
				{
					NDotL = 0;
				}
				Id = Ii * NDotL;
				vec3 H = (eye + t * trace)*(-1) + L;
				Is = Ii*pow((normal.normalize()*H.normalize()), sphere[i].m.exp);
			}

			Pixel p = { (sphere[i].m.Ka * Ia[0] + sphere[i].m.Kd * Id[0] + sphere[i].m.Ks * Is[0]) * 255, (sphere[i].m.Ka * Ia[1] + sphere[i].m.Kd * Id[1] + sphere[i].m.Ks * Is[1]) * 255, (sphere[i].m.Ka * Ia[2] + sphere[i].m.Kd * Id[2] + sphere[i].m.Ks * Is[2]) * 255 };
			return p;
		}
	}
	for (int i = 0; i < triangle.size(); i++) {//tracing triangle
		mat3 matrix(triangle[i].v1, triangle[i].v2, trace*-1);
		matrix = matrix.transpose();
		matrix = matrix.inverse();
		vec3 vect = eye - triangle[i].pt;
		vect = matrix*vect;
		vec3 normal;
		vec3 tmp_normal = triangle[i].v1^triangle[i].v2;
		tmp_normal.normalize();
		if (trace_normal*tmp_normal < 0)
		{
			normal = tmp_normal;
		}
		else
			normal = tmp_normal*(-1);
		vec3 Ii = triangle[i].m.RGB;
		Ia = triangle[i].m.RGB;
		float t = vect[2];
		if (t < t_min)
		{
			t_min = t;
			closerMat = triangle[i].m;
		}
		vec3 intersection = eye + t * (trace);
		//vec3 intersection = triangle[i].pt + vect[0] * triangle[i].v1 + vect[1] * triangle[i].v2;
		vec3 L = light - intersection;
		L.normalize();
		vec3 H = trace_normal*(-1) + L;
		Is = Ii*pow((normal.normalize()*H.normalize()), triangle[i].m.exp);
		Id = Ii*(normal*L);
		Pixel p = { (triangle[i].m.Ka * Ia[0] + triangle[i].m.Kd * Id[0] + triangle[i].m.Ks * Is[0]) * 255, (triangle[i].m.Ka * Ia[1] + triangle[i].m.Kd * Id[1] + triangle[i].m.Ks * Is[1]) * 255, (triangle[i].m.Ka * Ia[2] + triangle[i].m.Kd * Id[2] + triangle[i].m.Ks * Is[2]) * 255 };
		if (vect[0]>0 && vect[1] > 0 && vect[2] > 0 && (vect[0] + vect[1]) <= 1)
		{
			return p;
		}
	}
	Pixel p = { 0,0,0 };
	return p;
}
//Reflect
vec3 Shader::reflect(vec3 normal, vec3 incident)
{
	const double cosI = normal * incident;
	return incident - 2 * cosI * normal;
}
//Refract
vec3 Shader::refract(vec3 normal, vec3 incident, double n1, double n2)
{
	const double n = n1 / n2;
	const double cosI = normal * incident;
	const double sinT2 = n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0)
	{
		return 0;
	}
	return n * incident - (n + sqrt(1.0 - sinT2)) * normal;
}
void Shader::outputFile(char *filename){

    Pixel white = {255,255,255}, black = {0,0,0};

    vec3 canvas_x = (canvas[2] - canvas[1])/image.xRes;
    vec3 canvas_y = (canvas[3] - canvas[1])/image.yRes;
    vec3 pixel_coord = canvas[1] + canvas_x/2 + canvas_y/2;

    for (int y=0; y<image.yRes; y++) {
		for (int x=0; x<image.xRes; x++) {
            /*if(tracing(pixel_coord + canvas_x*x + canvas_y*y))
                image.writePixel(x, y, black);
            else
                image.writePixel(x, y, white);*/
			image.writePixel(x, y, tracing(pixel_coord + canvas_x*x + canvas_y*y));
			//image.writePixel(x, y, black);
		}
	}
	image.outputPPM(filename);
}

