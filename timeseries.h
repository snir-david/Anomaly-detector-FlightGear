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
    const char* csvFileName;
    list<string> featuresNameList;
    map<string, vector<float>> dataMap;
    TimeSeries(const char *CSVfileName);
    list<string> getFeaturesName();
    map<string, vector<float>> getMap();
    float dataInSpecificTime(map<string, vector<float>>);
    void addingDataToMap();

};


#endif /* TIMESERIES_H_ */
