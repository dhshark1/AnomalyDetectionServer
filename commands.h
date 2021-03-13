#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <vector>
#include "HybridAnomalyDetector.h"
using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    static void upload(ofstream* file1, DefaultIO* dio) {
        string line1 = dio->read();
        while (line1 != "done"){
            *file1 << line1 << endl;
            line1 = dio->read();
        }
    }
};

class SocketIO:public DefaultIO{
public:
    int serverFD;
    SocketIO(int serverFD):serverFD(serverFD){};
    string read() override{
        string serverInput="";
        char c=0;
        ::read(serverFD,&c,sizeof(char));
        while(c!='\n'){
            serverInput+=c;
            ::read(serverFD,&c,sizeof(char));
        }
        return serverInput;
    }
    void write(string text) override{
        ::write(serverFD,text.c_str(),text.length());
    }
    void write(float f) override{
        stringstream stream;
        stream << f;
        string s = stream.str();
        ::write(serverFD, s.c_str(), s.size());
    }
    void read(float* f) override{
        SocketIO::read();
    }
};

struct ClientData{
    const char* trainFile = "anomalyTrain.csv";
    const char* testFile = "anomalyTest.csv";
    vector<AnomalyReport> anomaliesDetected;
};

class Command{
    friend class CLI;
public:
    explicit Command(DefaultIO* dio, HybridAnomalyDetector* had):dio(dio),had(had){}
    virtual void execute(ClientData &data)=0;
    virtual ~Command(){}
protected:
    DefaultIO* dio;
    HybridAnomalyDetector* had;
    string description;
};

class UploadFile:public Command {
public:
    UploadFile(DefaultIO* dio, HybridAnomalyDetector* had):Command(dio, had){
        description = "1.upload a time series csv file\n";
    }
    void execute(ClientData &data) override {
        dio->write("Please upload your local train CSV file.\n");
        ofstream file1("anomalyTrain.csv");
        ofstream file2("anomalyTest.csv");
        DefaultIO::upload(&file1, dio);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        DefaultIO::upload(&file2, dio);
        file1.close();
        file2.close();
        dio->write("Upload complete.\n");
    }
};

class AlgoSettings:public Command {
public:
    AlgoSettings(DefaultIO* dio, HybridAnomalyDetector* had):Command(dio, had){
        description = "2.algorithm settings\n";
    }
    void execute(ClientData &data) override {
        dio->write("The current correlation threshold is ");
        dio->write(had->getThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");
        while (true) {
            string newCorrelation = dio->read();
            if (0 < stof(newCorrelation) && stof(newCorrelation) < 1){
                had->setThreshold(((stof((newCorrelation))*10)/10));
                break;
            } else {
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class DetectAnomalies:public Command{
public:
    DetectAnomalies(DefaultIO* dio, HybridAnomalyDetector* had):Command(dio, had){
        description = "3.detect anomalies\n";
    }
    void execute(ClientData &data) override{
        had->learnNormal(TimeSeries(data.trainFile));
        data.anomaliesDetected = had->detect(TimeSeries(data.testFile));
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResults:public Command{
public:
    DisplayResults(DefaultIO* dio, HybridAnomalyDetector* had):Command(dio, had){
        description =  "4.display results\n";
    }
    void execute(ClientData &data) override {
        for (auto & i : data.anomaliesDetected) {
            dio->write(i.timeStep);
            dio->write("\t ");
            dio->write(i.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

class AnalyzeResults:public Command {
public:
    AnalyzeResults(DefaultIO* dio, HybridAnomalyDetector* had):Command(dio, had){
        description =  "5.upload anomalies and analyze results\n";
    }
    void execute(ClientData &data) override{
        dio->write("Please upload your local anomalies file.\n");
        float N = TimeSeries(data.testFile).getRowSize();
        vector<pair<string, pair<int, int>>> unifiedAnomalies;
        vector<pair<int, int>> inputVector;
        string number;
        int index;
        string input = dio->read();
        pair<int, int> pairs;
        while (input != "done") {
            stringstream ss(input);
            index = 0;
            while (getline(ss, number, ',')) {
                if (index == 0) {
                    pairs.first = stol(number);
                } else {
                    pairs.second = stol(number);
                }
                index++;
            }
            N -= (pairs.second - pairs.first - 1);
            inputVector.push_back(pairs);
            input = dio->read();
        }
        long j = 0;
        for (int i = 0; i < data.anomaliesDetected.size() - 1; i++) {
            if (data.anomaliesDetected[i].description == data.anomaliesDetected[i + 1].description) {
                j += (data.anomaliesDetected[i + 1].timeStep - data.anomaliesDetected[i].timeStep);
            } else {
                unifiedAnomalies.push_back({data.anomaliesDetected[i].description,
                                            {data.anomaliesDetected[i].timeStep-j,
                                             data.anomaliesDetected[i].timeStep}});
                j = 0;
            }
        }
        if (j!=0) {
            unifiedAnomalies.push_back({data.anomaliesDetected.at(data.anomaliesDetected.size()-1).description,
                                        {data.anomaliesDetected.at(data.anomaliesDetected.size()-1).timeStep-j,
                                         data.anomaliesDetected.at(data.anomaliesDetected.size()-1).timeStep}});
        }
        float FP = 0, TP = 0;
        int flag = 0;
        for (auto & unifiedAnomalie : unifiedAnomalies) {
            for (auto& inputVec : inputVector) {
                if (unifiedAnomalie.second.first <= inputVec.second && unifiedAnomalie.second.second >= inputVec.first) {
                    TP++;
                    flag = 1;
                }
            }
            if (flag == 0) {
                FP++;
            }
            flag = 0;
        }
        dio->write("Upload complete.\n");
        float z = TP / (float) inputVector.size();
        z = floor(z * pow(10, 3)) / (float)pow(10, 3);
        dio->write("True Positive Rate: ");
        dio->write(z);
        dio->write("\n");
        z = FP / N;
        z = floor(z * pow(10, 3)) / (float)pow(10, 3);
        dio->write("False Positive Rate: ");
        dio->write(z);
        dio->write("\n");
    }
};

#endif /* COMMANDS_H_ */