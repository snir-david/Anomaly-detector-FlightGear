#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include <vector>
#include <list>
#include <sstream>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
public:
    //fields of Time Series class
    const char *csvFileName;
    list<string> featuresNameList;
    map<string, vector<float> > dataMap;

    TimeSeries(const char *CSVfileName);

    map<string, vector<float> > getMap() const;

    int getMapSize();
};

#endif /* TIMESERIES_H_ */
