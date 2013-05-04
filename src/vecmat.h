#ifndef VECMAT_H
#define VECMAT_H

#include <cmath>

namespace vecmat_constant {
    const double pi = 3.1415926535897932384626433;
}

namespace vecmat {

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
        vec3_t(double a);
        vec3_t();
        
        void normalize();
        vec3_t vecCross (const vec3_t b); //returns the cross product of two vectors
        void apply(const vec3_t b); //applies vector b to the 'this' vector by multiplying each component in 'this' to its counterpart in b
        vecmat::vec3_t apply_r (const vec3_t b); //same as apply but returns the result
        
        //vector compound assignment
        vec3_t& operator += (const vec3_t b);
        vec3_t& operator -= (const vec3_t b);
        
    } vec3;

    typedef vec3 point3;
    typedef vec3 col3;

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
    
}

#endif