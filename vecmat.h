#ifndef VECMAT_H
#define VECMAT_H

#include <cmath>

typedef struct vec3_t
{
	union{
		struct{
			double x,y,z;
		};
		struct {
			double r,g,b;
		};
		double data[3];
	};
	

    
    vec3_t(double ax, double ay, double az);
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
vec3 operator + (const double& a, const vec3& b);
vec3 operator + (const vec3& a, const double& b);
//standard vector subtraction
vec3 operator - (const vec3& a, const vec3& b);
vec3 operator - (const vec3& a, const double& b);
vec3 operator - (const vec3& a) ;
//vector dot product
double operator * (const vec3 &a, const vec3 &b);
//vector-scalar multiplication
vec3 operator * (const double& a, const vec3& b);
vec3 operator * (const vec3& b, const double& a);

bool operator == (const vec3& a, const vec3& b);
bool operator != (const vec3& a, const vec3& b);

#endif