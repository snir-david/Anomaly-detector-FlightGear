#include "timeseries.h"
#include "anomaly_detection_util.h"
#include <iostream>
#include <vector>
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;

// this is a simple test to put you on the right track
void generateTrainCSV(float a1, float b1, float a2, float b2, float a3, float b3) {
    ofstream out("trainFile1.csv");
    out << "A,B,C,D,E,F" << endl;
    Line ac(a1, b1);
    Line bd(a2, b2);
    Line ef(a3, b3);
    for (int i = 1; i <= 100; i++) {
        float a = i;
        float b = rand() % 40;
        float e = -1 * a;
        if (i % 2 == 0) {
            out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
                << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << "," << e << "," << ef.f(e) - 0.02 + (rand() % 40) / 100.0f
                << endl;
        } else {
            out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
                << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << "," << a << "," << ef.f(a) - 0.02 + (rand() % 40) / 100.0f
                << endl;
        }
    }
    out.close();
}

void generateTestCSV(float a1, float b1, float a2, float b2, float a3, float b3, int anomaly) {
    ofstream out("testFile1.csv");
    out << "A,B,C,D,E,F" << endl;
    Line ac(a1, b1);
    Line bd(a2, b2);
    Line ef(a3, b3);
    for (int i = 1; i <= 100; i++) {
        float a = i;
        float b = rand() % 40;
        float e = -1 * i;
        if (i != anomaly) {
            if (i % 2 == 0) {
                out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
                    << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << "," << e << ","
                    << ef.f(e) - 0.02 + (rand() % 40) / 100.0f << endl;
            } else {
                out << a << "," << b << "," << ac.f(a) - 0.02 + (rand() % 40) / 100.0f << ","
                    << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << "," << a << ","
                    << ef.f(a) - 0.02 + (rand() % 40) / 100.0f << endl;
            }
        } else
            out << a << "," << b << "," << ac.f(a) + 1 << "," << bd.f(b) - 0.02 + (rand() % 40) / 100.0f << "," << e
                << "," << ef.f(e) + 1 << endl;
    }
    out.close();
}

void checkCorrelationTrain(correlatedFeatures c, string f1, string f2, float a, float b) {
    if (c.feature1 == f1) {
        if (c.feature2 != f2)
            cout << "wrong correlated feature of " << f1 << " (-20)" << endl;
        else {
            if (c.corrlation < 0.99)
                cout << f1 << "-" << f2 << " wrong correlation detected (-5)" << endl;
            if (c.lin_reg.a < a - 0.5f || c.lin_reg.a > a + 0.5f)
                cout << f1 << "-" << f2 << " wrong value of line_reg.a (-5)" << endl;
            if (c.lin_reg.b < b - 0.5f || c.lin_reg.b > b + 0.5f)
                cout << f1 << "-" << f2 << " wrong value of line_reg.b (-5)" << endl;
            if (c.threshold > 0.3)
                cout << f1 << "-" << f2 << " wrong threshold detected (-5)" << endl;
        }
    }
}

int main() {
    for (int i = 0; i < 100; ++i) {

        srand(time(NULL));
        float a1 = 1 + rand() % 10, b1 = -50 + rand() % 100;
        float a2 = 1 + rand() % 20, b2 = -50 + rand() % 100;
        float a3 = 1 + rand() % 30, b3 = -50 + rand() % 100;

        /*
        cout<<"a1: " << a1 << endl;
        cout<<"b1: " << b1 << endl;
        cout<<"a2: " << a2 << endl;
        cout<<"b2: " << b2 << endl;
    */
        // test the learned model: (40 points)
        // expected correlations:
        //	A-C: y=a1*x+b1
        //	B-D: y=a2*x+b2

        generateTrainCSV(a1, b1, a2, b2, a3, b3);
        TimeSeries ts("trainFile1.csv");

        SimpleAnomalyDetector ad;
        ad.learnNormal(ts);
        vector<correlatedFeatures> cf = ad.getNormalModel();
        //cout<< "size of cf: " <<cf.size()<<endl;

        if (cf.size() != 3)
            cout << "wrong size of correlated features (-40)" << endl;
        else
            for_each(cf.begin(), cf.end(), [&a1, &b1, &a2, &b2, &a3, &b3](correlatedFeatures c) {
                checkCorrelationTrain(c, "A", "C", a1, b1); // 20 points
                checkCorrelationTrain(c, "B", "D", a2, b2); // 20 points
                checkCorrelationTrain(c, "E", "F", a3, b3);
            });

        // test the anomaly detector: (60 points)
        // one simply anomaly is injected to the data
        int anomaly = 5 + rand() % 90; // one anomaly injected in a random time step
        generateTestCSV(a1, b1, a2, b2, a3, b3, anomaly);
        TimeSeries ts2("testFile1.csv");
        vector<AnomalyReport> r = ad.detect(ts2);

        bool anomlyDetectedAC = false;
        bool anomalyDetectedEF = false;
        int falseAlarms = 0;
        for_each(r.begin(), r.end(), [&anomaly, &anomlyDetectedAC, &anomalyDetectedEF, &falseAlarms](AnomalyReport ar) {
            if (ar.description == "A-C" && ar.timeStep == anomaly)
                anomlyDetectedAC = true;
            else if (ar.description == "E-F" && ar.timeStep == anomaly)
                anomalyDetectedEF = true;
            else
                falseAlarms++;
        });

        if (!anomlyDetectedAC)
            cout << "the anomaly (of A-C) was not detected (-30)" << endl;
        if (!anomalyDetectedEF)
            cout << "the anomaly (of E-F) was not detected (-30)" << endl;

        if (falseAlarms > 0)
            cout << "you have " << falseAlarms << " false alarms (-" << min(30, falseAlarms * 3) << ")" << endl;

        cout << "done" << endl;
    }
    return 0;
}
