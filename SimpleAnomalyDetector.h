

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Point circleCenter;
    float threshold;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
public:

    vector<correlatedFeatures> correlatedFeaturesList;
    vector<AnomalyReport> anomalyReport;

    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    float pearsonRes(std::map<string, vector<float>>::iterator &mapIt1,
                     std::map<string, vector<float>>::iterator &mapIt2);

    float findThreshold(Point **points, int arrSize, Line regLine);

    Line drawLineReg(Point **points, int arrSize, std::map<string, vector<float>>::iterator &mapIt1,
                     std::map<string, vector<float>>::iterator &mapIt2);

    bool isCorrelated(float pearsonResult, float pearsonExpected);

    void pointsToArr(Point **points, int arrSize, map<string, vector<float>>::iterator &mapIt1,
                     map<string, vector<float>>::iterator &mapIt2);

    void insertCorrelatedFeature(string correlatedFeature1,
                                 string correlatedFeature2,
                                 float pearsonResult, Line regLine, Point circleCenter, float maxThrs);

    virtual void learnNormal(const TimeSeries &ts);

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return correlatedFeaturesList;
    }

    void freePoints(Point **points, int arrSize);

    void insertAnomaly(int featureIdx, string feature1, string feature2);

    void findAnomalyInCorrelatedFeatures(int i, map<string, vector<float>>::const_iterator &feature1It,
                                         map<string, vector<float>>::const_iterator &feature2It);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
