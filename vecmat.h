#ifndef VECMAT_H
#define VECMAT_H

#include <cmath>

typedef struct vec3
{
	union{
		struct{
			float x,y,z;
		};
		struct {
			float r,g,b;
		};
		float data[3];
	};
	
	vec3(float ax, float ay, float az) {
		data[0] = ax; data[1] = ay; data[2] = az;
	}
	vec3(){
		data[0] = 0.0; data[1] = 0.0; data[2] = 0.0;
	}
	
	void normalize()
	{
		float length = 1.0/sqrt(x*x + y*y + z*z);
		x *= length;
		y *= length;
		z *= length;		
	}
	
} vec3;

typedef vec3 point3;
typedef vec3 col3;

vec3 cross (const vec3 a, const vec3 b)
{
	vec3 n;
	n.data[0] = a.data[1]*b.data[2] - a.data[2]*b.data[1];
	n.data[1] = a.data[2]*b.data[0] - a.data[0]*b.data[2];
	n.data[2] = a.data[0]*b.data[1] - a.data[1]*b.data[0];
	return n;
}

vec3 operator + (const vec3 a, const vec3 b){ return vec3(a.data[0]+b.data[0], a.data[1]+b.data[1], a.data[2]+b.data[2]);}
vec3 operator + (const float a, const vec3 b){ return vec3(a+b.data[0], a+b.data[1], a+b.data[2]);}
vec3 operator + (const vec3 a, const float b){ return vec3(a.data[0]+b,a.data[1]+b,a.data[2]+b);}
vec3 operator - (const vec3 a, const vec3 b){ return vec3(a.data[0]-b.data[0], a.data[1]-b.data[1], a.data[2]-b.data[2]);}
vec3 operator - (const vec3 a, const float b){ return vec3(a.data[0]-b,a.data[1]-b,a.data[2]-b);}
vec3 operator - (const vec3 a) { return vec3(-a.data[0],-a.data[1],-a.data[2]);}
float operator * (const vec3 a, const vec3 b){ return (a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2]);}
vec3 operator * (const float a, const vec3 b){ return vec3(a*b.data[0], a*b.data[1], a*b.data[2]);}
vec3 operator * (const vec3 b, const float a){ return vec3(a*b.data[0], a*b.data[1], a*b.data[2]);}


#endif