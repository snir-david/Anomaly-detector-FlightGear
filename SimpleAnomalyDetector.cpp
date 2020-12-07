#include <iostream>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>>::iterator mapIt1, mapIt2;
    map<string, vector<float>> map = ts.getMap();
    float pearsonResult, threshold = 0;
    int arrSize = map.begin()->second.size(), mapSize = map.size(), idx = 0;
    for (mapIt1 = map.begin(); mapIt1 != map.end() && idx < mapSize / 2; mapIt1++, idx++) {
        for (mapIt2 = map.begin(); mapIt2 != map.end(); mapIt2++) {
            if (mapIt1 != mapIt2) {
                pearsonResult = pearson(mapIt1->second.data(), mapIt2->second.data(), mapIt1->second.size());
                if (pearsonResult >= 0.9 || pearsonResult <= -0.9) {
                    Point *points[arrSize];
                    for (int i = 0; i < arrSize; i++) {
                        points[i] = new Point(mapIt1->second[i], mapIt2->second[i]);
                    }
                    Line regLine = linear_reg(points, mapIt1->second.size());
                    for (int i = 0; i < arrSize; i++) {
                        float tmp = dev(*points[i], regLine);
                        if (tmp > threshold) {
                            threshold = tmp;
                        }
                    }
                    float maxThrs = 1.1 * threshold;
                    struct correlatedFeatures c = {mapIt1->first, mapIt2->first, pearsonResult, regLine, maxThrs};
                    correlatedFeaturesList.push_back(c);
                }
            }
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyReport;
    float distancePoint;
    for (int i = 0; i < correlatedFeaturesList.size(); i++) {
        long featureIdx = 0;
        auto feature1It = ts.dataMap.find(correlatedFeaturesList[i].feature1);
        auto feature2It = ts.dataMap.find(correlatedFeaturesList[i].feature2);
        for (auto it = feature1It->second.begin(); it != feature1It->second.end(); it++, featureIdx++) {
            Point p(feature1It->second[featureIdx], feature2It->second[featureIdx]);
            distancePoint = dev(p, correlatedFeaturesList[i].lin_reg);
            if (distancePoint > correlatedFeaturesList[i].threshold) {
                string description = correlatedFeaturesList[i].feature1 + "-" + correlatedFeaturesList[i].feature2;
                AnomalyReport a(description, featureIdx + 1);
                anomalyReport.push_back(a);
            }
        }

    }
    return anomalyReport;
}

