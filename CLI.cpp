#include "CLI.h"
#include <memory>

using namespace std;

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    CommandUtil commandUtil;
    commandsMap[0] = unique_ptr<Command>(new uploadCSV(dio));
    commandsMap[1] = unique_ptr<Command>(new algorithmSetting(dio, &commandUtil));
    commandsMap[2] = unique_ptr<Command>(new detectAnomalies(dio, &commandUtil));
    commandsMap[3] = unique_ptr<Command>(new displayResult(dio, &commandUtil));
    commandsMap[4] = unique_ptr<Command>(new uploadAnomaliesAndAnalyze(dio, &commandUtil));
    commandsMap[5] = unique_ptr<Command>(new exitCLI(dio, &commandUtil));
}

void CLI::start() {
    int userInput;
    string commandNum,
     menu =  "Welcome to the Anomaly Detection Server.\r"
                   "Please choose an option:\r"
                   "1.upload a time series csv file\r"
                   "2.algorithm settings\r"
                   "3.detect anomalies\r"
                   "4.display results\r"
                   "5.upload anomalies and analyze results\r"
                   "6.exit\r";

    while (commandNum != "6") {
        dio->write(menu);
        commandNum = dio->read();
        userInput = stoi(commandNum);
        commandsMap[userInput - 1]->execute();
    }
}


CLI::~CLI() {
}

