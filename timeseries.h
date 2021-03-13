

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iostream>
#include <map>
using namespace std;

class TimeSeries{
    vector<string> attributes;
    size_t dataRowSize;
    map<string, vector<float>> m_result;
public:
    TimeSeries(const char* CSVfileName);
    map<string, vector<float>> getMap() const {
        return m_result;
    }
    const vector<string>& gettAttributes()const{
        return attributes;
    }
    size_t getRowSize()const{
        return dataRowSize;
    }
    const vector<float>& getAttributeData(string name)const{
        return m_result.at(name);
    }
};

#endif /* TIMESERIES_H_ */
