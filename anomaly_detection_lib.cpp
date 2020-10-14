#include <iostream>

// returns the variance of X and Y
float var(float *x, int size);

// returns the covariance of X and Y
float cov(float *x, float *y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size);

class Line {
public:
    const float a, b;

    Line(float a, float b) : a(a), b(b) {}

    float f(float x) {
        return a * x + b;
    }
};

class Point {
public:
    const float x, y;

    Point(float x, float y) : x(x), y(x) {}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

int main() {
    std::cout << "Hello, World !" << std::endl;
    int x, y;
    x = 5;
    y = x + 6;
    int z = x + y;
    std::cout << z;
    return 0;
}