#include "HybridAnomalyDetector.h"

void HybridAnomalyDetector::learnHelper(const TimeSeries& ts, string f1, string f2,Point** pointArr, float* maxCorrelation, int size){
    if (*maxCorrelation >= 0.9) {
        SimpleAnomalyDetector::learnHelper(ts, f1, f2, pointArr, maxCorrelation, size);
    }
    if ((0.5 < *maxCorrelation) && (*maxCorrelation <= 0.9)) {
        correlatedFeatures c;
        c.feature1 = move(f1);
        c.feature2 = move(f2);
        Circle circ = findMinCircle(pointArr, size);
        c.center = circ.center;
        c.corrlation = *maxCorrelation;
        c.threshold = circ.radius;
        cf.push_back(c);
    }
}

bool HybridAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
    if (c.corrlation >= 0.9) {
        return SimpleAnomalyDetector::isAnomalous(x, y, c);
    } else {
        return (distance(Point(x, y), c.center) > c.threshold*1.1);
    }
}
