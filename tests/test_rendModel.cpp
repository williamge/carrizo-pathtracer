#include "gtest/gtest.h"
#include "../rendModel.h"

TEST(TestMin, normal) {
	EXPECT_EQ(1.0,min(1.0, 2.0));
	EXPECT_EQ(1.0,min(2.0, 1.0));
	EXPECT_EQ(1.0,min(1.0, 1.0));
	EXPECT_EQ(-1.0,min(-1.0, 1.0));
}

TEST(TestMax, normal){
    EXPECT_EQ(1.0, max(1.0, 0.0));
    EXPECT_EQ(1.0, max(0.0, 1.0));
	EXPECT_EQ(1.0, max(1.0, 1.0));
    EXPECT_EQ(50.0, max(1.0, 50.0));
    EXPECT_EQ(1.0, max(1.0, -1.0));
    
}