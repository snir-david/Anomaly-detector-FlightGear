#include "timeseries.h"

/* TimeSeries constructor - opening csv file entering the data to a map
 * first row is the features name after that is the data. */
TimeSeries::TimeSeries(const char *CSVfileName) {
    //fields for time series
    fstream csvFile;
    string featuresName, data;
    map<string, vector<float>>::iterator mapIt;
    csvFileName = CSVfileName;
    //open csv file
    csvFile.open(csvFileName);
    //getting column names - first line - headers
    getline(csvFile, featuresName);
    stringstream featureString(featuresName);
    //getting features name and inserting to list and map
    while (getline(featureString, data, ',')) {
        featuresNameList.push_back(data);
        dataMap.insert(pair<string, vector<float>>(data, {}));
    }
    //getting data to map
    while (getline(csvFile, data)) {
        mapIt = dataMap.begin();
        stringstream s(data);
        while (getline(s, data, ',') && mapIt != dataMap.end()) {
            mapIt->second.push_back(stof(data));
            mapIt++;
        }
    }
    //closing csv file
    csvFile.close();
}

/* Getter for features name */
list<string> TimeSeries::getFeaturesName() const {
    return featuresNameList;
}

/* Getter for map data */
map<string, vector<float>> TimeSeries::getMap() const {
    return dataMap;
}










