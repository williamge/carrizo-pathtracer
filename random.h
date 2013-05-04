//
//  random.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#ifndef carrizo_pathtracer_random_h
#define carrizo_pathtracer_random_h

#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<double> dis_zero_to_one(0.0,1.0);
static std::uniform_real_distribution<double> dis_minus_one_to_one(-1.0,1.0);

#endif
