

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    void learnNormal(const TimeSeries &ts);

    vector<AnomalyReport> detect(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
