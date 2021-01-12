

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
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

// you may edit this class
class CommandUtil {
public:
    float newThreshold;
    HybridAnomalyDetector had;
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
        string inputData;
        trainCSV.open("anomalyTrain.csv", ios::out);
        cout << "Please upload your local train CSV file" << endl;
        inputData = dio->read();
        while (inputData != "done") {
            trainCSV << inputData << endl;
            inputData = dio->read();
        }
        cout << "Upload complete" << endl;

        testCSV.open("anomalyTest.csv", ios::out);
        cout << "Please upload your local test CSV file" << endl;
        inputData = dio->read();
        while (inputData != "done") {
            testCSV << inputData << endl;
            inputData = dio->read();
        }
        cout << "Upload complete" << endl;
    }
};

class algorithmSetting : public Command {
public:
    algorithmSetting(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        cout << "The current correlation threshold is 0.9\r"
                "Type a new threshold" << endl;
        cu->newThreshold = stof(dio->read());
        while (cu->newThreshold < 0 || cu->newThreshold > 1) {
            cout << "please choose a value between 0 and 1." << endl;
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
        TimeSeries trainCSV("anomalyTrain.csv"), testCSV("anomalyTest.csv");
        cu->had.threshold = cu->newThreshold;
        cu->had.learnNormal(trainCSV);
        cu->had.detect(testCSV);
        cout << "anomaly detection complete." << endl;
    }

};

class displayResult : public Command {
public:
    displayResult(DefaultIO *dio, CommandUtil *cu) : Command(dio) {
        this->cu = cu;
    };

    void execute() override {
        for (int i = 0; i <cu->had.anomalyReport.size(); i++){
            cout << cu->had.anomalyReport[i].timeStep + "\t" + cu->had.anomalyReport[i].description<<endl;
        }
        cout <<"Done."<<endl;
    }

};

class uploadAnomaliesAndAnalyze : public Command {
public:
    uploadAnomaliesAndAnalyze(DefaultIO *dio) : Command(dio) {};

    void execute() override {}

};

class exitCLI : public Command {
public:
    exitCLI(DefaultIO *dio) : Command(dio) {};

    void execute() override {
        exit(0);
    }

};

#endif /* COMMANDS_H_ */
