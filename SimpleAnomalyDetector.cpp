#include <iostream>
#include "SimpleAnomalyDetector.h"

using namespace std;

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}

void SimpleAnomalyDetector::freePoints(Point **points, int arrSize) {
    //freeing memory that allocated
    for (int i = 0; i < arrSize; i++) {
        delete points[i];
    }
}

void SimpleAnomalyDetector::insertCorrelatedFeature(string correlatedFeature1,
                                                    string correlatedFeature2,
                                                    float pearsonResult, Line regLine, Point circleCenter,
                                                    float maxThreshold) {
    struct correlatedFeatures c = {correlatedFeature1, correlatedFeature2, pearsonResult, regLine, circleCenter,
                                   maxThreshold};
    correlatedFeaturesList.push_back(c);
}

float SimpleAnomalyDetector::pearsonRes(std::map<string, vector<float>>::iterator &mapIt1,
                                        std::map<string, vector<float>>::iterator &mapIt2) {
    float pearsonResult;
    //checking pearson for the features
    pearsonResult = pearson(mapIt1->second.data(), mapIt2->second.data(), mapIt1->second.size());
    return pearsonResult;
}

float SimpleAnomalyDetector::findThreshold(Point **points, int arrSize, Line regLine) {
    float threshold = 0;
    //checking distance between line reg and every point - saving the max distance
    for (int i = 0; i < arrSize; i++) {
        float tmp = dev(*points[i], regLine);
        if (tmp > threshold) {
            threshold = tmp;
        }
    }
    return threshold;
}

void SimpleAnomalyDetector::pointsToArr(Point **points, int arrSize, std::map<string, vector<float>>::iterator &mapIt1,
                                        std::map<string, vector<float>>::iterator &mapIt2) {
//first build an array of points for the features
    for (int i = 0; i < arrSize; i++) {
        points[i] = new Point(mapIt1->second[i], mapIt2->second[i]);
    }
}

Line SimpleAnomalyDetector::drawLineReg(Point **points, int arrSize, std::map<string, vector<float>>::iterator &mapIt1,
                                        std::map<string, vector<float>>::iterator &mapIt2) {
    //drawing line reg for the features
    Line regLine = linear_reg(points, mapIt1->second.size());
    return regLine;
}

bool SimpleAnomalyDetector::isCorrelated(float pearsonResult, float pearsonExpected) {
    //if pearson is bigger than |pearsonExpected| - if it is the features are correlative
    if (pearsonResult >= pearsonExpected || pearsonResult <= -pearsonExpected) {
        return true;
    }
    return false;
}


/* learn the normal by getting time series object. checking all features and finding
 * the ones that correlative - saving them in list.*/
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>>::iterator mapIt1, mapIt2;
    map<string, vector<float>> map = ts.getMap();
    float pearsonResult;
    //initialize points array
    int arrSize = map.begin()->second.size();
    Point *points[arrSize];
    //checking the first half of the features against all other features
    for (mapIt1 = map.begin(); mapIt1 != map.end(); mapIt1++) {
        for (mapIt2 = next(mapIt1, 1); mapIt2 != map.end(); mapIt2++) {
            //checking if the features are not equals
            if (mapIt1 != mapIt2) {
                pearsonResult = pearsonRes(mapIt1, mapIt2);
                //finding and saving correlated features (bigger than 0.9)
                if (isCorrelated(pearsonResult, 0.9)) {
                    pointsToArr(points, arrSize, mapIt1, mapIt2);
                    Line regLine = drawLineReg(points, arrSize, mapIt1, mapIt2);
                    //saving threshold as the furthest point from reg line multiply by 1.1
                    float maxThrs = 1.1 * findThreshold(points, arrSize, regLine);
                    //saving the correlated features and pushing it to the list
                    insertCorrelatedFeature(mapIt1->first, mapIt2->first, pearsonResult, regLine, Point(0, 0), maxThrs);
                }
            }
        }
    }
    //freeing memory that allocated
    freePoints(points, arrSize);
}

void SimpleAnomalyDetector::insertAnomaly(int featureIdx, string feature1, string feature2) {
    string description =
            feature1 + "-" + feature2;
    AnomalyReport a(description, featureIdx + 1);
    //pushing anomaly object to the anomaly list
    anomalyReport.push_back(a);
}

void SimpleAnomalyDetector::findAnomalyInCorrelatedFeatures(int i,
                                                            std::map<string, vector<float>>::const_iterator &feature1It,
                                                            std::map<string, vector<float>>::const_iterator &feature2It) {
    float distancePoint;
    int featureIdx = 0;
    //iterate the data and checking to find anomaly
    for (auto it = feature1It->second.begin(); it != feature1It->second.end(); it++, featureIdx++) {
        //checking new data (the point) and calculating distance from line reg
        Point p(feature1It->second[featureIdx], feature2It->second[featureIdx]);
        distancePoint = dev(p, correlatedFeaturesList[i].lin_reg);
        //if the distance from line reg is bigger than correlated features threshold - anomaly detected
        if (distancePoint > correlatedFeaturesList[i].threshold) {
            //making anomaly object
            insertAnomaly(featureIdx, correlatedFeaturesList[i].feature1, correlatedFeaturesList[i].feature2);
        }
    }
}

/* detecting anomaly - giving a correlated features list checking new time series to find any anomaly
 * against the line reg for the features. */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    //for every correlated features check it against new data in this features
    for (int i = 0; i < correlatedFeaturesList.size(); i++) {
        //finding iterators for features in the new data
        auto feature1It = ts.dataMap.find(correlatedFeaturesList[i].feature1);
        auto feature2It = ts.dataMap.find(correlatedFeaturesList[i].feature2);
        findAnomalyInCorrelatedFeatures(i, feature1It, feature2It);
    }
    return anomalyReport;
}

