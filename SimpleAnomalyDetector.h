/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "minCircle.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <iostream>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Circle minCircle = Circle(Point(0,0),0);
    float threshold;
    bool isCircleDetect;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
public:
    float user_threshold = 100;

    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

    void setUserThreshold(float num) {
        user_threshold = num;
    }

    void create_cor_feature_sim(float pear, int feature1, int feature2,const TimeSeries &ts);
    virtual correlatedFeatures create_cor_feature_cir(float pear, int feature1, int feature2,const TimeSeries &ts);
    virtual vector<AnomalyReport> check_if_inside(const TimeSeries &ts, const correlatedFeatures &i);
};

Point** find_points_of_correlated_features (const TimeSeries &ts, string feat1, string feat2);
float* from_vector_to_array(vector<float> feature);
Line find_linear_reg (const TimeSeries &ts, string feat1, string feat2);
float max_threshold (Point** points, Line line, int size);
//AnomalyReport check_if_detect(const TimeSeries &ts, const correlatedFeatures &cor);
vector<AnomalyReport> check_if_detect(const TimeSeries &ts, const correlatedFeatures &cor);
#endif /* SIMPLEANOMALYDETECTOR_H_ */