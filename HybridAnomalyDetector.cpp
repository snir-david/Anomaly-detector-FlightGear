#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    HybridAnomalyDetector ad;
    map<string, vector<float>>::iterator mapIt1, mapIt2;
    map<string, vector<float>> map = ts.getMap();
    int arrSize = map.begin()->second.size();
    float pearsonResult;
    //checking the first half of the features against all other features
    for (mapIt1 = map.begin(); mapIt1 != map.end(); mapIt1++) {
        for (mapIt2 = next(mapIt1, 1); mapIt2 != map.end(); mapIt2++) {
            //checking if the features are not equals
            if (mapIt1 != mapIt2) {
                pearsonResult = pearsonRes(mapIt1, mapIt2);
                //if pearson is bigger than |0.9| - if it is the features are correlative
                if (pearsonResult >= 0.9 || pearsonResult <= -0.9) {
                    SimpleAnomalyDetector ad;
                    ad.learnNormal(ts);
                } else if (pearsonResult >= 0.5 || pearsonResult <= -0.5) {
                    findMinCircle()
                }
            }
        }
    }

