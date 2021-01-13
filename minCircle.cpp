//205686538

#include "minCircle.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>

using namespace std;

/*given a circle and point checking if the point is in the circle.
 * using distance between 2 points - sqrt((x1-x2)^2+(y1-y2)^2)
 * if the distance bigger than radius so point not in circle, return false. else, return true */
bool pointIsInsideCircle(Circle &c, Point p) {
    Point circleCenter = c.center;
    float x1MinusX2 = pow((circleCenter.x - p.x), 2);
    float y1MinusY2 = pow((circleCenter.y - p.y), 2);
    float pointFromCenter = sqrt((x1MinusX2 + y1MinusY2));
    return c.radius >= pointFromCenter;
};

/* given a circle and vector of points checking that all points are inside or on the boundary of circle*/
bool isValidCircle(Circle &c, vector<Point> &points) {
    //iterate all points in vector checking it is inside circle
    for (Point &p: points) {
        if (!pointIsInsideCircle(c, p)) {
            return false;
        }
    }
    return true;
}

/* return min circle given 2 points - using the following equation -
 * center (x,y) -
 * x= (x1+x2)/2
 * y= (y1+y2)/2
 * r = sqrt((x1-x2)^2+(y1-y2)^2)/2
 */
Circle twoPointsCircle(Point &a, Point &b) {
    float x = (a.x + b.x) / 2;
    float y = (a.y + b.y) / 2;
    float radius = sqrt((pow((a.x - b.x), 2) + pow((a.y - b.y), 2))) / 2;
    return Circle(Point(x, y), radius);
}

/* return min circle given 3 points - using the following equation -
 * center (x,y) -
 * x= ((x1^2+y1^2)(y2-y3)+(x2^2+y2^2)(y3-y1)+(x3^2+y3^2)(y1-y2))/(2(x1(y2-y3)-y1(x2-x3)+x2y3-x3y2)
 * y= ((x1^2+y1^2)(x2-x3)+(x2^2+y2^2)(x1-x3)+(x3^2+y3^2)(x2-x1))/(2(x1(y2-y3)-y1(x2-x3)+x2y3-x3y2)
 * r = sqrt((x-x1)^2+(y-y1)^2)
 */
Circle threePointsCircle(Point a, Point b, Point c) {
    vector<Point> points = {a, b, c};
    //for all pairs trying to make circle without third point, if succeed return this circle,
    //else return circle with 3 points
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; ++j) {
            Circle circle     = twoPointsCircle(points[i], points[j]);
            if (isValidCircle(circle, points)) {
                return circle;
            }
        }
    }
    //calculating 3 points circle
    float denominator = 2 * (a.x * (b.y - c.y) - a.y * (b.x - c.x) + b.x * c.y - c.x * b.y);
    float xNumerator = (pow(a.x, 2) + pow(a.y, 2)) * (b.y - c.y) + (pow(b.x, 2) + pow(b.y, 2)) * (c.y - a.y)
                       + (pow(c.x, 2) + pow(c.y, 2)) * (a.y - b.y);
    float yNumerator = (pow(a.x, 2) + pow(a.y, 2)) * (c.x - b.x) + (pow(b.x, 2) + pow(b.y, 2)) * (a.x - c.x)
                       + (pow(c.x, 2) + pow(c.y, 2)) * (b.x - a.x);
    float x = xNumerator / denominator;
    float y = yNumerator / denominator;
    float radius = sqrt((pow((x - a.x), 2) + pow((y - a.y), 2)));
    return Circle(Point(x, y), radius);
}


/* return min circle given 1 point - using the following equation -
 * center (x,y) -
 * x= x1
 * y= y1
 * r = 0
 */
Circle onePointCircle(Point &a) {
    return Circle(a, 0);
}

/*given vector of points, and vector with points that sit on boundary of the min circle.
 * this function work recursively until getting min circle. */
Circle findMinWithBoundaryPoints(vector<Point> &points, vector<Point> boundaryPoints, size_t pointsSize) {
    int randIdx = 0;
    //base case pointsSize == 0 || R.pointsSize ==3
    if (pointsSize == 0 || boundaryPoints.size() == 3) {
        switch (boundaryPoints.size()) {
            case 0:
                return Circle(Point(0, 0), 0);
            case 1:
                return onePointCircle(boundaryPoints[0]);
                break;
            case 2:
                return twoPointsCircle(boundaryPoints[0], boundaryPoints[1]);
            case 3:
                return threePointsCircle(boundaryPoints[0], boundaryPoints[1], boundaryPoints[2]);
                break;
            default:
                break;

        }
    }
    //getting random Point from array
    if (pointsSize > 0) {
        randIdx = rand() % pointsSize;
    }
    Point randPoint(points[randIdx].x, points[randIdx].y);
    //sending the random point to the end of the vector
    // so in next call it we be out of boundary of the vector size
    swap(points[randIdx], points[pointsSize-1]);
    //calculating min circle minCircle
    Circle minCircle = findMinWithBoundaryPoints(points, boundaryPoints, pointsSize - 1);
    //checking if the random point in the circle - if it is return the circle
    if (pointIsInsideCircle(minCircle, randPoint)) {
        return minCircle;
    } else { // else, point not in the circle, so it must be on the boundary
        //insert point to boundary points vector
        boundaryPoints.push_back(randPoint);
        //calling function with the new boundary point
        return findMinWithBoundaryPoints(points, boundaryPoints, pointsSize - 1);
    }
}

/*using welzl's algorithm returning minimum circle given array of points.
 * we will pick a random point p, remove it from circle and recursively find a min circle d,
 * if the point p is in that circle, return the circle we found.
 * else, p must be on min circle boundary, adding p to R array of points that is on the circle boundary.
 * base case - if Points array is empty, R size is 3 recursion stops.
 * if the base case is reached - if R size is 1, return circle with radius 0 and the point is in R.
 * R size 2 return min circle with 2 points, radius is half distance and also center is between 2 points.
 * R size 3 return min circle using equation to find circle using 3 points.
 */
Circle findMinCircle(Point **points, size_t size) {
    vector<Point> vectorPoints, boundaryPoints;
    //base cases - size == 0 || 1 || 2 || 3
    switch (size) {
        case 0:
            return Circle(Point(0, 0), 0);
            break;
        case 1:
            return onePointCircle(*points[0]);
            break;
        case 2:
            return twoPointsCircle(*points[0], *points[1]);
        case 3:
            return threePointsCircle(*points[0], *points[1], *points[2]);
            break;
        default:
            break;
    }
    //inserting the array points into vector
    for (int i = 0; i < size; i++) {
        vectorPoints.push_back(*points[i]);
    }
    return findMinWithBoundaryPoints(vectorPoints, boundaryPoints, size);
}

