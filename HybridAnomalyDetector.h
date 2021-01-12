

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    double threshold = 0.9;

    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    void learnNormal(const TimeSeries &ts);

    vector<AnomalyReport> detect(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
