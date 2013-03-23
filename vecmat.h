#ifndef VECMAT_H
#define VECMAT_H

#include <cmath>

typedef struct vec3_t
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
	

    
    vec3_t(float ax, float ay, float az);
    vec3_t();
	
    void normalize();
    vec3_t vecCross (const vec3_t b);
	
} vec3;

typedef vec3 point3;
typedef vec3 col3;






vec3 operator + (const vec3 a, const vec3 b);
vec3 operator + (const float a, const vec3 b);
vec3 operator + (const vec3 a, const float b);
vec3 operator - (const vec3 a, const vec3 b);
vec3 operator - (const vec3 a, const float b);
vec3 operator - (const vec3 a) ;
float operator * (const vec3 a, const vec3 b);
vec3 operator * (const float a, const vec3 b);
vec3 operator * (const vec3 b, const float a);


#endif