/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#include "CLI.h"

// Constructor:
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new UploadCVS(dio));
    commands.push_back(new Settings(dio));
    commands.push_back(new DetectAnomalies(dio));
    commands.push_back(new Result(dio));
    commands.push_back(new UploadAnomalies(dio));
    commands.push_back(new Exit(dio));
}

void CLI::start(){
    commonInfo info;
    int index = 0;
    while (index != 5) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 1; i <= 6; i++) {
            dio->write(to_string(i));
            dio->write(".");
            dio->write(commands[i-1]->description);
            dio->write("\n");
        }
        string opt = dio->read();
        index= opt[0] - '0' - 1;
        if(index >= 0 && index <= 6)
            commands[index]->execute(&info);
    }
    return;
}

CLI::~CLI() {
    for(int i = 0; i < commands.size(); i++) {
        delete commands[i];
    }
}
