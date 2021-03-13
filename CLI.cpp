#include "CLI.h"

void CLI::start(){
    string temp = "";
    auto* ad = new HybridAnomalyDetector();
    auto* data = new ClientData;
    vector<Command*> commands;
    UploadFile uploadCommand(dio, ad);
    AlgoSettings settingsCommand(dio, ad);
    DetectAnomalies detectCommand(dio, ad);
    DisplayResults displayCommand(dio, ad);
    AnalyzeResults analyzeCommand(dio, ad);

    commands.push_back(&uploadCommand);
    commands.push_back(&settingsCommand);
    commands.push_back(&detectCommand);
    commands.push_back(&displayCommand);
    commands.push_back(&analyzeCommand);
    do{
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i<commands.size(); i++) {
            dio->write(commands.at(i)->description);
        }
        dio->write("6.exit\n");
        temp = dio->read();
        switch (stoi(temp)){
            case 1:
                commands.at(0)->execute(*data);
                break;
            case 2:
                commands.at(1)->execute(*data);
                break;
            case 3:
                commands.at(2)->execute(*data);
                break;
            case 4:
                commands.at(3)->execute(*data);
                break;
            case 5:
                commands.at(4)->execute(*data);
                break;
            default:
                break;
        }
    }while (stoi(temp) !=6);
    delete ad;
    delete data;
}
