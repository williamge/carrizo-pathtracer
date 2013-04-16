#include "gtest/gtest.h"
#include "vecmat.h"

TEST(Vector, Construct) {
    vec3 testVector (1.0,0.5,0.2);
	ASSERT_FLOAT_EQ(1.0, testVector.x);
	ASSERT_FLOAT_EQ(0.5, testVector.y);
    ASSERT_FLOAT_EQ(0.2, testVector.z);
}
