/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual
    correlatedFeatures create_cor_feature_cir(float pear, int feature1, int feature2, const TimeSeries &ts) override;
    virtual vector<AnomalyReport> check_if_inside(const TimeSeries &ts, const correlatedFeatures &cor) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */