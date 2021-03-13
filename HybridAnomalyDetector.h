

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector(){};
    virtual ~HybridAnomalyDetector(){};
    virtual void learnHelper(const TimeSeries& ts, string f1, string f2,Point** ps, float* maxCorrelation, int size) override;
    virtual bool isAnomalous(float x, float y,correlatedFeatures c) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
