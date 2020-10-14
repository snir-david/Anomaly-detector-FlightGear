/*
 * animaly_detection_util.cpp
 *
 * Author: Snir David Nahari 205686538
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float sum, avg;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    avg = (1.00 / (float) size) * sum;
    return avg;
}

// returns the variance of X and Y
float var(float* x, int size){
    float variance, varianceSum, mean;
    //calculating the mean
    mean = avg(x, size);
    //calculating the sum in the variance
    for(int i = 0; i < size; ++i) {
        varianceSum += pow((x[i] - mean), 2);
    }
    variance = (1.00/(float) size) * varianceSum;
    return variance;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    return 0;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return 0;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

    return Line(0,0);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    return 0;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    return 0;
}




