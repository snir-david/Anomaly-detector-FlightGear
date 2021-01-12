

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    float threshold = 0.9;
    vector<correlatedFeatures> correlatedFeaturesList;

    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    void learnNormal(const TimeSeries &ts);

    vector<AnomalyReport> detect(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
