

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point center;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector() {}

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    float getThreshold() {
        return threshold;
    }

    void setThreshold(float t){
        threshold = t;
    }

protected:
    virtual void learnHelper(const TimeSeries& ts, string f1, string f2,Point** ps, float* maxCorrelation, int size);
    virtual float init_threshold(Point** points, Line line, int size);
    virtual void init_line_reg(Point** pointArr, float* x, float* y, int size);
    virtual bool contains(string feature);
    virtual bool isAnomalous(float x, float y,correlatedFeatures c);
    vector<correlatedFeatures> cf;
    float threshold;


};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
