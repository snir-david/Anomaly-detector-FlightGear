#include <iostream>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

/* learn the normal by getting time series object. checking all features and finding
 * the ones that correlative - saving them in list.*/
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>>::iterator mapIt1, mapIt2;
    map<string, vector<float>> map = ts.getMap();
    float pearsonResult, threshold = 0;
    int arrSize = map.begin()->second.size(), mapSize = map.size(), idx = 0;
    //checking the first half of the features against all other features
    for (mapIt1 = map.begin(); mapIt1 != map.end(); mapIt1++, idx++) {
        for (mapIt2 = next(mapIt1,1); mapIt2 != map.end(); mapIt2++) {
            //checking if the features are not equals
            if (mapIt1 != mapIt2) {
                //checking pearson for the features
                pearsonResult = pearson(mapIt1->second.data(), mapIt2->second.data(), mapIt1->second.size());
                //if pearson is bigger than |0.9| - if it is the features are correlative
                if (pearsonResult >= 0.9 || pearsonResult <= -0.9) {
                    //checking line reg for the features
                    //first build an array of points for the features
                    Point *points[arrSize];
                    for (int i = 0; i < arrSize; i++) {
                        points[i] = new Point(mapIt1->second[i], mapIt2->second[i]);
                    }
                    Line regLine = linear_reg(points, mapIt1->second.size());
                    //checking distance between line reg and every point - saving the max distance
                    for (int i = 0; i < arrSize; i++) {
                        float tmp = dev(*points[i], regLine);
                        if (tmp > threshold) {
                            threshold = tmp;
                        }
                    }
                    //saving threshold as the furthest point from reg line multiply by 1.1
                    float maxThrs = 1.1 * threshold;
                    //saving the correlated features and pushing it to the list
                    struct correlatedFeatures c = {mapIt1->first, mapIt2->first, pearsonResult, regLine, maxThrs};
                    correlatedFeaturesList.push_back(c);
                }
            }
        }
    }
}

/* detecting anomaly - giving a correlated features list checking new time series to find any anomaly
 * against the line reg for the features. */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyReport;
    float distancePoint;
    //for every correlated features check it against new data in this features
    for (int i = 0; i < correlatedFeaturesList.size(); i++) {
        long featureIdx = 0;
        //finding iterators for features in the new data
        auto feature1It = ts.dataMap.find(correlatedFeaturesList[i].feature1);
        auto feature2It = ts.dataMap.find(correlatedFeaturesList[i].feature2);
        //iterate the data and checking to find anomaly
        for (auto it = feature1It->second.begin(); it != feature1It->second.end(); it++, featureIdx++) {
            //checking new data (the point) and calculating distance from line reg
            Point p(feature1It->second[featureIdx], feature2It->second[featureIdx]);
            distancePoint = dev(p, correlatedFeaturesList[i].lin_reg);
            //if the distance from line reg is bigger than correlated features threshold - anomaly detected
            if (distancePoint > correlatedFeaturesList[i].threshold) {
                //making anomaly object
                string description = correlatedFeaturesList[i].feature1 + "-" + correlatedFeaturesList[i].feature2;
                AnomalyReport a(description, featureIdx + 1);
                //pushing anomaly object to the anomaly list
                anomalyReport.push_back(a);
            }
        }

    }
    return anomalyReport;
}

