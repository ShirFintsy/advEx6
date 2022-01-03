/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
Circle minimum_enclosing_circle(const vector<Point>& P); ///???
bool is_inside(const Circle& c, const Point& p);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */