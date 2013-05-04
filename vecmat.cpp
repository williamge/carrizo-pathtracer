#include "vecmat.h"	

vecmat::vec3::vec3_t(double ax, double ay, double az) {
    data[0] = ax; data[1] = ay; data[2] = az;
}
vecmat::vec3::vec3_t(double a) {
    data[0] = a; data[1] = a; data[2] = a;
}
vecmat::vec3::vec3_t(){
    data[0] = 0.0; data[1] = 0.0; data[2] = 0.0;
}

void vecmat::vec3::normalize()
{
    double length = 1.0/sqrt(x*x + y*y + z*z);
    x *= length;
    y *= length;
    z *= length;		
}

vecmat::vec3_t vecmat::vec3::vecCross (const vec3_t b)
{
    vec3_t n;
    n.data[0] = data[1]*b.data[2] - data[2]*b.data[1];
    n.data[1] = data[2]*b.data[0] - data[0]*b.data[2];
    n.data[2] = data[0]*b.data[1] - data[1]*b.data[0];
    return n;
}

void vecmat::vec3_t::apply (const vec3_t b)
{
    data[0] = data[0]*b.data[0];
    data[1] = data[1]*b.data[1];
    data[2] = data[2]*b.data[2];
}

vecmat::vec3_t vecmat::vec3_t::apply_r (const vec3_t b)
{
    return vec3_t (data[0]*b.data[0], data[1]*b.data[1], data[2]*b.data[2]);
}


vecmat::vec3& vecmat::vec3::operator+= (const vec3 b)
{
    data[0] += b.data[0];
    data[1] += b.data[1];
    data[2] += b.data[2];
    return *this;
}

vecmat::vec3& vecmat::vec3::operator-= (const vec3 b)
{
    data[0] -= b.data[0];
    data[1] -= b.data[1];
    data[2] -= b.data[2];
    return *this;
}


vecmat::vec3 vecmat::operator + (const vecmat::vec3& a, const vecmat::vec3& b)
{
    return vecmat::vec3(a.data[0]+b.data[0],    a.data[1]+b.data[1],    a.data[2]+b.data[2]);
}
vecmat::vec3 vecmat::operator + (const double& a, const vecmat::vec3& b)
{
    return vecmat::vec3(a+b.data[0],    a+b.data[1],    a+b.data[2]);
}
vecmat::vec3 vecmat::operator + (const vecmat::vec3& a, const double& b)
{
    return vecmat::vec3(a.data[0]+b,    a.data[1]+b,    a.data[2]+b);
}

vecmat::vec3 vecmat::operator - (const vecmat::vec3& a, const vecmat::vec3& b)
{
    return vecmat::vec3(a.data[0]-b.data[0],    a.data[1]-b.data[1],    a.data[2]-b.data[2]);
}
vecmat::vec3 vecmat::operator - (const vecmat::vec3& a, const double& b)
{
    return vecmat::vec3(a.data[0]-b,    a.data[1]-b,    a.data[2]-b);
}
vecmat::vec3 vecmat::operator - (const vecmat::vec3& a)
{
    return vecmat::vec3(-a.data[0],     -a.data[1],     -a.data[2]);
}

double vecmat::operator * (const vecmat::vec3 &a, const vecmat::vec3 &b)
{
    return a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2];
}
vecmat::vec3 vecmat::operator * (const double& a, const vecmat::vec3& b)
{
    return vecmat::vec3(a*b.data[0], a*b.data[1], a*b.data[2]);
}
vecmat::vec3 vecmat::operator * (const vecmat::vec3& b, const double& a)
{
    return vecmat::vec3(a*b.data[0], a*b.data[1], a*b.data[2]);
}

bool vecmat::operator == (const vecmat::vec3& a, const vecmat::vec3& b)
{
    return (a.data[0] == b.data[0]
            && a.data[1] == b.data[1]
            && a.data[2] == b.data[2]);
}

bool vecmat::operator != (const vecmat::vec3& a, const vecmat::vec3& b)
{
    return (a.data[0] != b.data[0]
            || a.data[1] != b.data[1]
            || a.data[2] != b.data[2]);
}