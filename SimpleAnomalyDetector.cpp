#include "SimpleAnomalyDetector.h"
#include <string>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}

void SimpleAnomalyDetector::init_line_reg(Point** pointArr, float* x, float* y, int size) {
    for (int i = 0; i < size; i++) {
        pointArr[i] = new Point(x[i], y[i]);
    }
}

float SimpleAnomalyDetector::init_threshold(Point** points, Line line, int size) {
    float max = 0;
    for (int i =0; i < size; i++) {
        if (max < dev(*points[i], line)){
            max = dev(*points[i], line);
        }
    }
    return max;
}

bool SimpleAnomalyDetector::contains(string feature){
    for (int i = 0; i < cf.size(); ++i)
        if (cf[i].feature1 == feature)
            return true;
    return false;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries&  ts){
    map<string, vector<float>> timeSeriesMap = ts.getMap();
    float *x;
    float *y, *y1;
    float possibleCorrelation;
    string f2;
    for (auto feature1 = timeSeriesMap.begin(); feature1 != timeSeriesMap.end(); feature1++) {
        if (!(contains(feature1->first))) {
            float m = 0;
            float* maxCorrelation = &m;
            for (auto feature2 = next(feature1); feature2 != timeSeriesMap.end(); feature2++) {
                if (feature1 != feature2) {
                    x = &(feature1->second[0]);
                    y = &(feature2->second[0]);
                    possibleCorrelation = fabs(pearson(x, y, (feature1->second).size()));
                    if(*maxCorrelation < possibleCorrelation) {
                        f2 = move(feature2->first);
                        y1 = y;
                        *maxCorrelation = possibleCorrelation;
                    }
                }
            }
            Point* pointArr[ts.getRowSize()];
            init_line_reg(pointArr, x, y1, ts.getRowSize());
            learnHelper(ts, feature1->first, f2, pointArr, maxCorrelation, ts.getRowSize());
            for(size_t k=0;k<ts.getRowSize();k++)
                delete pointArr[k];
        }
    }
}

void SimpleAnomalyDetector::learnHelper(const TimeSeries& ts, string f1, string f2,Point** pointArr, float* maxCorrelation, int size){
    if (*maxCorrelation >= 0.9) {
        correlatedFeatures c;
        c.feature1 = move(f1);
        c.feature2 = move(f2);
        c.lin_reg = linear_reg(pointArr, size);
        c.threshold = init_threshold(pointArr, c.lin_reg, size);
        c.corrlation = *maxCorrelation;
        cf.push_back(c);
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    map<string, vector<float>> timeSeriesMap = ts.getMap();
    vector<AnomalyReport> reporterArr;
    for_each(cf.begin(),cf.end(),[&reporterArr,&ts,this](correlatedFeatures c){
        vector<float> x=ts.getAttributeData(c.feature1);
        vector<float> y=ts.getAttributeData(c.feature2);
        for(size_t i=0;i<x.size();i++){
            if(isAnomalous(x[i],y[i],c)){
                string d=c.feature1 + "-" + c.feature2;
                reporterArr.push_back(AnomalyReport(d,(i+1)));
            }
        }
    });
    return reporterArr;
}

bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
    return (abs(y - c.lin_reg.f(x))>c.threshold*1.1);
}
