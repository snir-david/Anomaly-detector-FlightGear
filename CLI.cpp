#include "CLI.h"
#include <memory>

using namespace std;

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    CommandUtil cu;
    commandsMap[0] = unique_ptr<Command>(new uploadCSV(dio));
    commandsMap[1] = unique_ptr<Command>(new algorithmSetting(dio, &cu));
    commandsMap[2] = unique_ptr<Command>(new detectAnomalies(dio, &cu));
    commandsMap[3] = unique_ptr<Command>(new displayResult(dio, &cu));
    commandsMap[4] = unique_ptr<Command>(new uploadAnomaliesAndAnalyze(dio));
    commandsMap[5] = unique_ptr<Command>(new exitCLI(dio));
}

void CLI::start() {
    int userInput;
    string commandNum;
    commandNum = dio->read();
    while (commandNum != "") {
        cout << "Welcome to the Anomaly Detection Server.\r"
                "Please choose an option:\r"
                "1. upload a time series csv file\r"
                "2. algorithm settings\r"
                "3. detect anomalies\r"
                "4. display results\r"
                "5. upload anomalies and analyze results\r"
                "6. exit" << endl;
        userInput = stoi(commandNum);
        commandsMap[userInput - 1]->execute();
        commandNum = dio->read();
    }
}


CLI::~CLI() {
}

