

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <memory>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

class CommandUtil {
public:
    double newThreshold;
    shared_ptr<HybridAnomalyDetector> had;
};

class Command {
protected:
    DefaultIO *dio;
    CommandUtil *cu;
public:
    Command(DefaultIO *dio) : dio(dio) {}


    virtual void execute() = 0;

    virtual ~Command() {}
};

class uploadCSV : public Command {
public:
    uploadCSV(DefaultIO *dio) : Command(dio) {
    };

    void execute() override {
        fstream trainCSV, testCSV;
        string inputData, uploadTestCSV =  "Please upload your local test CSV file.\n",
        uploadTrainCSV = "Please upload your local train CSV file.\n", complete = "Upload complete.\n";
        trainCSV.open("anomalyTrain.csv", ios::out);
        dio->write(uploadTrainCSV);
        inputData = dio->read();
        while (inputData != "done") {
            trainCSV << inputData << endl;
            inputData = dio->read();
        }
        dio->write(complete);
        testCSV.open("anomalyTest.csv", ios::out);
        dio->write(uploadTestCSV);
        inputData = dio->read();
        while (inputData != "done") {
            testCSV << inputData << endl;
            inputData = dio->read();
        }
        dio->write(complete);
    }
};

class algorithmSetting : public Command {
public:
    algorithmSetting(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        string currentCorrelation = "The current correlation threshold is 0.9\n"
                "Type a new threshold\n";
        string chooseValue = "please choose a value between 0 and 1.\n";
        dio->write(currentCorrelation);
        cu->newThreshold = stof(dio->read());
        while (cu->newThreshold < 0 || cu->newThreshold > 1) {
            dio->write(chooseValue);
            cu->newThreshold = stof(dio->read());
        }
    }

};

class detectAnomalies : public Command {
public:
    detectAnomalies(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        string detectCompl = "anomaly detection complete.\n";
        shared_ptr<HybridAnomalyDetector> had(new HybridAnomalyDetector());
        TimeSeries trainCSV("anomalyTrain.csv"), testCSV("anomalyTest.csv");
        had->learnNormal(trainCSV);
        had->detect(testCSV);
        cu->had = had;
        dio->write(detectCompl);
    }

};

class displayResult : public Command {
public:
    displayResult(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        string done = "Done.\n";
        for (int i = 0; i < cu->had->anomalyReport.size(); i++) {
            string s = to_string(cu->had->anomalyReport[i].timeStep) + " \t" + cu->had->anomalyReport[i].description + "\n";
            dio->write(s);
        }
        dio->write(done);
    }

};

class uploadAnomaliesAndAnalyze : public Command {
public:
    uploadAnomaliesAndAnalyze(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        string inputData, uploadAnomalies = "Please upload your local anomalies file.\n", complete = "Upload complete.\n",
                TPstring, FPstring;
        stringstream  TPs, FPs;
        vector<pair<int, int>> timeStamps, anomalyRange;
        int positive = 0, negative = TimeSeries("anomalyTest.csv").getMapSize(), FP = 0, TP = 0;
        long firstTimeStamp;
        float truePositiveRate, falseAlarmRate;
        dio->write(uploadAnomalies);
        for (int i = 0; i < cu->had->anomalyReport.size() - 1; i++) {
            firstTimeStamp = cu->had->anomalyReport[i].timeStep;
            while (cu->had->anomalyReport[i].description == cu->had->anomalyReport[i + 1].description &&
                   (cu->had->anomalyReport[i].timeStep + 1) == cu->had->anomalyReport[i + 1].timeStep) {
                i++;
            }
            pair<int, int> pair(firstTimeStamp, cu->had->anomalyReport[i].timeStep);
            anomalyRange.push_back(pair);
        }

        inputData = dio->read();
        while (inputData != "done") {
            int pos = inputData.find(",");
            string s1 = inputData.substr(0, pos);
            string s2 = inputData.substr(pos + 1);
            pair<int, int> pair(stoi(s1), stoi(s2));
            timeStamps.push_back(pair);
            negative -= (stoi(s2) - stoi(s1));
            inputData = dio->read();
            positive++;
        }
        dio->write(complete);
        for (int i = 0; i < anomalyRange.size(); i++) {
            bool cut = false;
            for (int j = 0; j < timeStamps.size(); j++) {
                if ((timeStamps[j].second > anomalyRange[i].first && anomalyRange[i].second > timeStamps[j].second) ||
                    (timeStamps[j].first > anomalyRange[i].first && anomalyRange[i].second > timeStamps[j].first) ||
                        (timeStamps[j].first < anomalyRange[i].first && anomalyRange[i].second < timeStamps[j].second)) {
                    TP++;
                    cut = true;
                }
            }
            if (!cut) {
                FP++;
            }
        }
        //setting up numbers for printing 3 numbers after the point
        truePositiveRate = (TP * 1000 / positive) ;
        falseAlarmRate = (FP * 1000 / negative) ;
        truePositiveRate = floor(truePositiveRate);
        truePositiveRate = truePositiveRate/1000;
        TPs << truePositiveRate;
        TPstring = TPs.str();
        falseAlarmRate = floor(falseAlarmRate);
        falseAlarmRate = falseAlarmRate/1000;
        FPs << falseAlarmRate;
        FPstring = FPs.str();
        string truePositive = "True Positive Rate: " + TPstring +"\n";
        string falsePositive = "False Positive Rate: " + FPstring +"\n";
        dio->write(truePositive);
        dio->write(falsePositive);
    }

};

class exitCLI : public Command {
public:
    exitCLI(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
    }

};

#endif /* COMMANDS_H_ */
