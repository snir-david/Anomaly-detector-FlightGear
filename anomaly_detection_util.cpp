/*
 * animaly_detection_util.cpp
 *
 * Author: Snir David Nahari 205686538
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float avg, sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    avg = (1.00 / (float) size) * sum;
    return avg;
}

// returns the variance of X and Y
float var(float* x, int size){
    //initialize variables for the function
    float variance,  mean, varianceSum = 0;
    //calculating the mean
    mean = avg(x, size);
    //calculating the sum in the variance
    for (int i = 0; i < size; ++i) {
        varianceSum += pow((x[i] - mean), 2);
    }
    //calculating the variance using the equation - Var(X)= (1/N) * SUM- from i=0 to N of (x_i-u)
    variance = (1.00 / (float) size) * varianceSum;
    return variance;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    //initialize variables for the function
    float eX, eY, eXY, covXY;
    //initialize array for the E[XY]
    float xy[size - 1];
    for (int i = 0; i < size; ++i) {
        xy[i] = x[i] * y[i];
    }
    //calculating the variables E[XY], E[X], E[Y]
    eXY = avg(xy, size);
    eX = avg(x, size);
    eY = avg(y, size);
    //calculating the covariance using the equation - cov(X,Y) = E(XY)-E(X)E(Y)
    covXY = eXY - eX * eY;
    return covXY;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    //initialize variables for the function
    float pearson, covXY, sigmaX, sigmaY;
    //calculating sigma x,y using square of the variance
    sigmaX = sqrt(var(x, size));
    sigmaY = sqrt(var(y, size));
    //calculating covariance using cov function
    covXY = cov(x, y, size);
    //calculating pearson using the equation - pearson(X,Y) = cov(X,Y)/(sqrt(var(X) * sqrt(var(Y)))
    pearson = covXY/(sigmaX * sigmaY);
    return pearson;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    //initialize variables for the function
    float a, b;
    //initialize arrays for the x and y
    float x[size - 1], y[size - 1];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    //calculating the linear regression a and b for the equation - Y = a*X + b
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - (a * avg(x, size));
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l = linear_reg(points, size);
    float dev = l.f(p.x) - p.y;
    if(dev < 0 ) {
        return -dev;
    }
    return dev;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float dev = l.f(p.x) - p.y;
    if(dev < 0 ) {
        return -dev;
    }
    return dev;}




