#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    CommandUtil commandUtil;
    this->cu = commandUtil;

    //insert command into array
    this->commandsArray[0] = new uploadCSV(dio);
    this->commandsArray[1] = new algorithmSetting(dio, &cu);
    this->commandsArray[2] = new detectAnomalies(dio, &cu);
    this->commandsArray[3] = new displayResult(dio, &cu);
    this->commandsArray[4] = new uploadAnomaliesAndAnalyze(dio, &cu);
    this->commandsArray[5] = new exitCLI(dio);
}

void CLI::start() {
    int userInput;
    bool exitCLI = false;
    string commandNum,
     menu =  "Welcome to the Anomaly Detection Server.\n"
                   "Please choose an option:\n"
                   "1.upload a time series csv file\n"
                   "2.algorithm settings\n"
                   "3.detect anomalies\n"
                   "4.display results\n"
                   "5.upload anomalies and analyze results\n"
                   "6.exit\n";

    while (!exitCLI) {
        this->dio->write(menu);
        commandNum = this->dio->read();
        userInput = stoi(commandNum);
        userInput -= 1;
        if (commandNum == "6") {
            exitCLI = true;
        }
        else if (userInput >= 0 && userInput <=4) {
            this->commandsArray[userInput]->execute();
        }
    }
}

CLI::~CLI() {
    for (int i = 0; i < 6; i++){
        delete commandsArray[i];
    }
}

