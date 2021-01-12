#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>>::iterator mapIt1, mapIt2;
    map<string, vector<float>> map = ts.getMap();
    float pearsonResult;
    //initialize mapIt2 iterator and points array
    int arrSize = map.begin()->second.size();
    Point *points[arrSize];
    //checking the first half of the features against all other features
    for (mapIt1 = map.begin(); mapIt1 != map.end(); mapIt1++) {
        for (mapIt2 = next(mapIt1, 1); mapIt2 != map.end(); mapIt2++) {
            //checking if the features are not equals
            if (mapIt1 != mapIt2) {
                pearsonResult = pearsonRes(mapIt1, mapIt2);
                if (isCorrelated(pearsonResult, threshold)) {
                    pointsToArr(points, arrSize, mapIt1, mapIt2);
                    Line regLine = drawLineReg(points, arrSize, mapIt1, mapIt2);
                    //saving threshold as the furthest point from reg line multiply by 1.1
                    float maxThrs = 1.1 * findThreshold(points, arrSize, regLine);
                    //saving the correlated features and pushing it to the list
                    insertCorrelatedFeature(mapIt1->first, mapIt2->first, pearsonResult, regLine, Point(0, 0), maxThrs);
                } else if (isCorrelated(pearsonResult, 0.5)) {
                    pointsToArr(points, arrSize, mapIt1, mapIt2);
                    Circle c = findMinCircle(points, arrSize);
                    insertCorrelatedFeature(mapIt1->first, mapIt2->first, pearsonResult, Line(), c.center,
                                            (c.radius * 1.1));
                }
                //finding and saving correlated features (bigger than 0.9)

            }
        }
    }
    //freeing memory that allocated
    freePoints(points, arrSize);
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    //for every correlated features check it against new data in this features
    for (int i = 0; i < correlatedFeaturesList.size(); i++) {
        if (correlatedFeaturesList[i].corrlation >= 0.9) {
            auto feature1It = ts.dataMap.find(correlatedFeaturesList[i].feature1);
            auto feature2It = ts.dataMap.find(correlatedFeaturesList[i].feature2);
            findAnomalyInCorrelatedFeatures(i, feature1It, feature2It);
        } else {
            Circle c(correlatedFeaturesList[i].circleCenter, correlatedFeaturesList[i].threshold);
            long featureIdx = 0;
            //finding iterators for features in the new data
            auto feature1It = ts.dataMap.find(correlatedFeaturesList[i].feature1);
            auto feature2It = ts.dataMap.find(correlatedFeaturesList[i].feature2);
            //iterate the data and checking to find anomaly
            for (auto it = feature1It->second.begin(); it != feature1It->second.end(); it++, featureIdx++) {
                //checking new data (the point) and calculating distance from line reg
                Point p(feature1It->second[featureIdx], feature2It->second[featureIdx]);
                //if the distance from line reg is bigger than correlated features threshold - anomaly detected
                if (!pointIsInsideCircle(c, p)) {
                    insertAnomaly(featureIdx, correlatedFeaturesList[i].feature1, correlatedFeaturesList[i].feature2);
                }
            }

        }

    }
    return anomalyReport;
}