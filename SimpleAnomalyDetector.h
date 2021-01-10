

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> correlatedFeaturesList;
public
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
    float pearsonRes(std::map<string, vector<float>>::iterator &mapIt1,
                     std::map<string, vector<float>>::iterator &mapIt2);
    float findThreshold(Point **points, int arrSize, Line regLine);
    Line drawLineReg(Point **points, int arrSize, std::map<string, vector<float>>::iterator &mapIt1,
                     std::map<string, vector<float>>::iterator &mapIt2);
    virtual void findCorrelated(std::map<string, vector<float>> &map, std::map<string, vector<float>>::iterator &mapIt1, std::map<string, vector<float>>::iterator &mapIt2);
    virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel(){
		return correlatedFeaturesList;
	}
    void pointsToArr(Point **points, int arrSize, map<string, vector<float>>::iterator &mapIt1,
                     map<string, vector<float>>::iterator &mapIt2);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
