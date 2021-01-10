
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <vector>
#include "anomaly_detection_util.h"

using namespace std;

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};

/*given a circle and point checking if the point is in the circle.
 * using distance between 2 points - sqrt((x1-x2)^2+(y1-y2)^2)
 * if the distance bigger than radius so point not in circle, return false. else, return true */
bool pointIsInsideCircle(Circle &c, Point p);

/* given a circle and vector of points checking that all points are inside or on the boundary of circle*/
bool isValidCircle(Circle &c, vector<Point> &points);

/* return min circle given 2 points - using the following equation -
 * center (x,y) -
 * x= (x1+x2)/2
 * y= (y1+y2)/2
 * r = sqrt((x1-x2)^2+(y1-y2)^2)/2
 */
Circle twoPointsCircle(Point &a, Point &b);

/* return min circle given 3 points - using the following equation -
 * center (x,y) -
 * x= ((x1^2+y1^2)(y2-y3)+(x2^2+y2^2)(y3-y1)+(x3^2+y3^2)(y1-y2))/(2(x1(y2-y3)-y1(x2-x3)+x2y3-x3y2)
 * y= ((x1^2+y1^2)(x2-x3)+(x2^2+y2^2)(x1-x3)+(x3^2+y3^2)(x2-x1))/(2(x1(y2-y3)-y1(x2-x3)+x2y3-x3y2)
 * r = sqrt((x-x1)^2+(y-y1)^2)
 */
Circle threePointsCircle(Point a, Point b, Point c);


/* return min circle given 1 point - using the following equation -
 * center (x,y) -
 * x= x1
 * y= y1
 * r = 0
 */
Circle onePointCircle(Point &a);

/*given vector of points, and vector with points that sit on boundary of the min circle.
 * this function work recursively until getting min circle. */
Circle findMinWithBoundaryPoints(vector<Point> &points, vector<Point> boundaryPoints, size_t pointsSize);

/*using welzl's algorithm returning minimum circle given array of points.
 * we will pick a random point p, remove it from circle and recursively find a min circle d,
 * if the point p is in that circle, return the circle we found.
 * else, p must be on min circle boundary, adding p to R array of points that is on the circle boundary.
 * base case - if Points array is empty, R size is 3 recursion stops.
 * if the base case is reached - if R size is 1, return circle with radius 0 and the point is in R.
 * R size 2 return min circle with 2 points, radius is half distance and also center is between 2 points.
 * R size 3 return min circle using equation to find circle using 3 points.
 */
Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */


