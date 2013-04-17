#include "vecmat.h"	

	vec3::vec3_t(float ax, float ay, float az) {
		data[0] = ax; data[1] = ay; data[2] = az;
	}
	vec3::vec3_t(){
		data[0] = 0.0; data[1] = 0.0; data[2] = 0.0;
	}
	
	void vec3::normalize()
	{
		float length = 1.0/sqrt(x*x + y*y + z*z);
		x *= length;
		y *= length;
		z *= length;		
	}
    
    vec3_t vec3::vecCross (const vec3_t b)
    {
        vec3_t n;
        n.data[0] = data[1]*b.data[2] - data[2]*b.data[1];
        n.data[1] = data[2]*b.data[0] - data[0]*b.data[2];
        n.data[2] = data[0]*b.data[1] - data[1]*b.data[0];
        return n;
    }

    void vec3_t::apply (const vec3_t b)
    {
        data[0] = data[0]*b.data[0];
        data[1] = data[1]*b.data[1];
        data[2] = data[2]*b.data[2];
    }


vec3 operator + (const vec3& a, const vec3& b)
{
    return vec3(a.data[0]+b.data[0],    a.data[1]+b.data[1],    a.data[2]+b.data[2]);
}
vec3 operator + (const float& a, const vec3& b)
{
    return vec3(a+b.data[0],    a+b.data[1],    a+b.data[2]);
}
vec3 operator + (const vec3& a, const float& b)
{
    return vec3(a.data[0]+b,    a.data[1]+b,    a.data[2]+b);
}
vec3 operator - (const vec3& a, const vec3& b)
{
    return vec3(a.data[0]-b.data[0],    a.data[1]-b.data[1],    a.data[2]-b.data[2]);
}
vec3 operator - (const vec3& a, const float& b)
{
    return vec3(a.data[0]-b,    a.data[1]-b,    a.data[2]-b);
}
vec3 operator - (const vec3& a)
{
    return vec3(-a.data[0],     -a.data[1],     -a.data[2]);
}
float operator * (const vec3 &a, const vec3 &b)
{
    return a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2];
}
vec3 operator * (const float& a, const vec3& b)
{
    return vec3(a*b.data[0], a*b.data[1], a*b.data[2]);
}
vec3 operator * (const vec3& b, const float& a)
{
    return vec3(a*b.data[0], a*b.data[1], a*b.data[2]);
}

vec3& vec3::operator+= (const vec3 b)
{
    data[0] += b.data[0];
    data[1] += b.data[1];
    data[2] += b.data[2];
    return *this;
}

bool operator == (const vec3& a, const vec3& b)
{
    return (a.data[0] == b.data[0]
            && a.data[1] == b.data[1]
            && a.data[2] == b.data[2]);
}