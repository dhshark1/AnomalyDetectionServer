#include "timeseries.h"
using namespace std;

TimeSeries::TimeSeries(const char *CSVfileName) {
    string line, colname;
    ifstream file(CSVfileName);
    vector<string> features;
    // Make sure the file is open
    if (!file.is_open()) throw runtime_error("Could not open file");
    // Read the column names
    if (file.good()) {
        // Extract the first line in the file
        getline(file, line);
        // Create a stringstream from line
        stringstream ss(line);
        // Extract each column name
        while (getline(ss, colname, ',')) {
            // Initialize and add <colname, float vector> pairs to result
            m_result.insert({colname, vector<float>{}});
            features.push_back(colname);
        }
    }
    string col;
    // Read data, line by line
    while (getline(file, line)) {
        // Create a stringstream of the current line
        stringstream ss(line);
        // Extract each float
        for (const auto &feature : features) {
            if (ss.good()) {
                getline(ss, col, ',');
                m_result[feature].push_back(stof(col));
                // If the next token is a comma, ignore it and move on
                if(ss.peek() == ',') ss.ignore();
            }
        }
    }
    attributes = features;
    dataRowSize = m_result[features[0]].size();
    // Close the file:
    file.close();
}
