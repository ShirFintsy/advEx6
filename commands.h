/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include <iomanip>
#include "HybridAnomalyDetector.h"

using namespace std;

int* get_range(string s);
int get_N(vector<int*> clientRange);

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}
    int saveFile(string csvFile) {// need to check this func

        ofstream out(csvFile);
        string s="";
        int numOfLines = -2;
        while((s=read())!="done"){
            out<<s<<endl;
            numOfLines++;
        }
        out.close();
        return numOfLines;
    }
};

struct SerialTimeStep {
    int start;
    int end;
    string description;
};

struct commonInfo {
    float threshold = 0.9;
    vector<AnomalyReport> detectReports;
    vector<SerialTimeStep> stp;
    int fileSize;
    commonInfo(){};
};

class Command {
protected:
    DefaultIO *dio;
public:
    string description;

    Command(DefaultIO *d, string des) {
        dio = d;
        description = des;
    }

    //Command(DefaultIO* dio, string des):dio(dio),description(des){}
    virtual void execute(commonInfo *info) = 0;

    virtual ~Command() {}

    int* get_range(string s){
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token; // first
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + delimiter.length());
        }
        int start = stoi(token);
        int end = stoi(s);
        static int range[2];
        range[0] = start;
        range[1] = end;
        return range;
    }
    int get_N(vector<int> clientBegins, vector<int> clientEnds){
        int sum = 0;
        for (int i = 0; i < clientEnds.size(); i++) {
            sum += clientEnds[i] - clientBegins[i] + 1;
        }return sum;
    }
};



class UploadCVS:public Command {
    // Constructor:
public:
    UploadCVS(DefaultIO* dio):Command(dio,"upload a time series csv file"){}
    virtual void execute(commonInfo* info) {
        dio->write("Please upload your local train CSV file.\n");
        dio->saveFile("tsTrain.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        int size = dio->saveFile("tsTest.csv");
        info->fileSize = size;
        dio->write("Upload complete.\n");
    }
};
class Settings:public Command {
public:
    // Constructor:
    Settings(DefaultIO* dio): Command(dio, "algorithm settings"){}
    virtual void execute(commonInfo* info) {
        dio->write("The current correlation threshold is: ");
        dio->write(info->threshold);
        dio->write("\n");
        float newThreshold;
        while (true) {
            dio->write("Type a new threshold\n");
            dio->read(&newThreshold);
            if (newThreshold <= 1 && newThreshold >= 0) break;
            dio->write("please choose a value between 0 and 1.\n");
        }
        info->threshold = newThreshold;
    }
};
class DetectAnomalies:public Command {
public:
    // Constructor:
    DetectAnomalies(DefaultIO* dio): Command(dio, "detect anomalies"){}
    virtual void execute(commonInfo* info) {
        HybridAnomalyDetector detector;
        //where did we get this file from?
        TimeSeries tsTrain("tsTrain.csv");
        TimeSeries tsTest("tsTest.csv");
        detector.setUserThreshold(info->threshold);
        detector.learnNormal(tsTrain);
        info->detectReports = detector.detect(tsTest);
        dio->write("anomaly detection complete.\n");

        SerialTimeStep steps;
        steps.description = "";
        steps.start = 0;
        steps.end = 0;

        for (AnomalyReport report : info->detectReports) {
            if (steps.description != report.description) {
                if (steps.description != "") {info->stp.push_back(steps);}
                steps.description = report.description;
                steps.start = report.timeStep;
                steps.end = report.timeStep;
            } else {
                if (report.timeStep == steps.end + 1) {
                    steps.end = report.timeStep;
                } else if (report.timeStep != steps.end) {
                    info->stp.push_back(steps);
                    steps.start = report.timeStep;
                    steps.end = report.timeStep;
                }
            }
        }
        info->stp.push_back(steps); // insert the last one
    }
};
class Result:public Command {
public:
    // Constructor:
    Result(DefaultIO* dio): Command(dio, "display results"){}
    virtual void execute(commonInfo* info) {
        for (AnomalyReport report : info->detectReports) {
            const long timeStep = report.timeStep;
            const string description = report.description;
            dio->write((float)timeStep);
            dio->write("\t");
            dio->write(description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};
class UploadAnomalies:public Command {
public:
    // Constructor:
    UploadAnomalies(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    virtual void execute(commonInfo* info) {
        dio->write("Please upload your local anomalies file.\n");
        string lineInFile = "";

        vector<int> clientBegins;
        vector<int> clientEnds;
        int p = 0;
        string s;
        while ((s = dio->read()) != "done") {
            int* range = get_range(s);
            clientBegins.push_back(range[0]);
            clientEnds.push_back(range[1]);
            p++;
        }
        dio->write("Upload complete.\n");

        int FP=0, TP=0;
        for (SerialTimeStep time: info->stp){
            int flag = 0;
            for (int i = 0; i < clientEnds.size(); i++) {
                if ((clientBegins[i] <= time.end && clientEnds[i] >= time.end) ||
                    (clientBegins[i] <= time.start && clientEnds[i] >= time.start) ||
                    (clientBegins[i] <= time.start && clientEnds[i] >= time.end) ||
                    (clientBegins[i] >= time.start && clientEnds[i] <= time.end)) {
                    TP++;
                    flag = 1;
                    break;
                }

            } if (flag == 0) {
                FP++;
            }
        }

        float positiveRate = (float) TP / (float) p;
        positiveRate = (floor(positiveRate * 1000)) / 1000;
        float N = (float) info->fileSize - (float) get_N(clientBegins, clientEnds);
        float falseAlarm = (float) FP / N;
        falseAlarm = (floor(falseAlarm * 1000)) / 1000;

        dio->write("True Positive Rate: ");
        string pos = to_string(positiveRate);
        pos.erase ( pos.find_last_not_of('0') + 1, string::npos );
        dio->write(pos);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        string fal = to_string(falseAlarm);
        fal.erase ( fal.find_last_not_of('0') + 1, string::npos );
        dio-> write(fal);
        dio->write("\n");

    }
};
class Exit:public Command {
public:
    // Constructor:
    Exit(DefaultIO* dio): Command(dio, "exit"){}
    virtual void execute(commonInfo* info) {
        return;
    }

};



#endif /* COMMANDS_H_ */