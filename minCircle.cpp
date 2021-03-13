#include "minCircle.h"

float distance(const Point& a, const Point& b) {
    //according to formula found on internet
    return sqrt(pow(a.x - b.x, 2)
                + pow(a.y - b.y, 2));
}

Circle form_circle_2points(const Point& a, const Point& b) {
    //according to formula found on internet
    Point center = {(a.x+b.x)/2, (a.y+b.y)/2};
    float radius = distance(a, b)/2;
    return Circle(center, radius);
}

Circle form_circle_3points(const std::vector<Point*>& supportPoints) {
    //according to formula found on internet
    float yDelta_a = supportPoints[1]->y - supportPoints[0]->y;
    float xDelta_a = supportPoints[1]->x - supportPoints[0]->x;
    float yDelta_b = supportPoints[2]->y - supportPoints[0]->y;
    float xDelta_b = supportPoints[2]->x - supportPoints[0]->x;

    float point1 = xDelta_a * xDelta_a + yDelta_a * yDelta_a;
    float point2 = xDelta_b * xDelta_b + yDelta_b  * yDelta_b ;
    float point3 = xDelta_a * yDelta_b  - yDelta_a * xDelta_b;
    Point center =  Point((yDelta_b  * point1 - yDelta_a * point2) / (2 * point3),
                          ((xDelta_a * point2 - xDelta_b * point1) / (2 * point3)));
    center.x += supportPoints[0]->x;
    center.y += supportPoints[0]->y;
    return Circle(center, distance(center, *supportPoints[0]));
}

int validate_circle(Circle c, const std::vector<Point*>& supportPoints) {
    for (Point *p : supportPoints) {
        if (distance(c.center, *p) > c.radius) {
            return 0;
        }
    }
    return 1;
}

Circle minCircle_helper(Point** points, std::vector<Point*> supportPoints, size_t size) {
    if(size==0 || supportPoints.size() == 3) {
        switch(supportPoints.size()) {
            case 0:
                return Circle(Point(0,0), 0);
            case 1:
                return Circle(*supportPoints[0], 0);
            case 2:
                return form_circle_2points(*supportPoints[0], *supportPoints[1]);
            case 3: {
                for (Point *p1 : supportPoints) {
                    for (Point *p2 : supportPoints) {
                        if (p1!=p2) {
                            Circle minCircle = form_circle_2points(*p1, *p2);
                            if (validate_circle(minCircle, supportPoints))
                                return minCircle;
                        }
                    }
                }
                return form_circle_3points(supportPoints);
            }
        }
    }
    Circle potentialCircle = minCircle_helper(points, supportPoints, size - 1);
    Point* p = points[size - 1];
    int pointInsideCircle = distance(potentialCircle.center, *p) < potentialCircle.radius;
    if (pointInsideCircle) {
        return potentialCircle;
    }
    supportPoints.push_back(p);
    return minCircle_helper(points, supportPoints, size - 1);
}

Circle findMinCircle(Point** points, size_t size) {
    //source: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/welzl-r2484/
    //goal: to grow a circle to contain all the input points.
    //Every time the circle is grown to contain a new point, the circle is updated.
    //Once a new point P is introduced, the circle should grow to contain the new point.
    //algorithm defines a support set that contains points (called support points).
    // Supporting points are defined as points that lie on the perimeter.
    //After the circle has grown we have to add point P to the support points
    //then we compute the minimum circle from the new set
    //Any points from the set not on the circle perimeter after
    //the new sphere is computed are removed from the set After the update
    //The algorithm stops only when a circle is found that includes all points from the input set
    //update process attempts to find the smallest circle by
    //computing all combinations of circles possible from the set plus the new point P
    vector<Point*> supportPoints;
    return minCircle_helper(points, supportPoints, size);
}
