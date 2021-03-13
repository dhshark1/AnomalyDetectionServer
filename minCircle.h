
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <math.h>

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------



float distance(const Point& a, const Point& b);

Circle form_circle_2points(const Point& a, const Point& b);

Circle form_circle_3points(const std::vector<Point*>& supportPoints);

int validate_circle(Circle c, const std::vector<Point*>& supportPoints);

Circle minCircle_helper(Point** points, std::vector<Point*> supportPoints, size_t size);

Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */
