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
    vec3_t vecCross (const vec3_t b); //returns the cross product of two vectors
    void apply(const vec3_t b); //applies vector b to the 'this' vector by multiplying each component in 'this' to its counterpart in b
    
    //vector compound assignment
    vec3_t& operator += (const vec3_t b);
	
} vec3;

typedef vec3 point3;
typedef vec3 col3;



//TODO: these should probably pass in references to the parameters for speed

//standard vector addition
vec3 operator + (const vec3& a, const vec3& b);
vec3 operator + (const float& a, const vec3& b);
vec3 operator + (const vec3& a, const float& b);
//standard vector subtraction
vec3 operator - (const vec3& a, const vec3& b);
vec3 operator - (const vec3& a, const float& b);
vec3 operator - (const vec3& a) ;
//vector dot product
float operator * (const vec3 &a, const vec3 &b);
//vector-scalar multiplication
vec3 operator * (const float& a, const vec3& b);
vec3 operator * (const vec3& b, const float& a);

bool operator == (const vec3& a, const vec3& b);

#endif