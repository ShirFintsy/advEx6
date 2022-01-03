/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

/**
 * This check if all points in current correlatedFeatures are in the correlatedFeatures we found in learn normal
 * @param ts - time series with the tested info
 * @param cor - correlatedFeatures object we found in learn normal part.
 * @return an AnomalyReport if there is an exception, an empty AnomalyReport if there isn't.
 */
vector<AnomalyReport> HybridAnomalyDetector::check_if_inside(const TimeSeries &ts, const correlatedFeatures &cor) {
    // get size of vector of current correlatedFeatures:
    int sizeOfFeature = ts.get_column_by_head(cor.feature1).size();
    // create circle of new info from test ts:
    Circle cir = Circle(cor.minCircle.center, cor.threshold);

    vector<AnomalyReport> reports;
    for (int i = 0; i < sizeOfFeature; i++) {
        // create point from line cor in feature 1 and line cor in feature 2.
        Point p = Point(ts.get_column_by_head(cor.feature1)[i], ts.get_column_by_head(cor.feature2)[i]);

        if (!is_inside(cir, p)) {
            AnomalyReport a(cor.feature1 + "-" + cor.feature2, i + 1);
            reports.push_back(a);
        }
    } return reports;
}

/**
 * This function create a correlatedFeatures object by hybrid detect (linear reg).
 * @param pear - the correlation of the correlated features.
 * @param feature1 - the index of feature 1 in time series.
 * @param feature2 - the index of feature 2 in time series.
 * @param ts - time series that contain all info about the features.
 * @return a correlatedFeatures object we need to add to vector of all correlatedFeatures in SimpleAnomalyDetector.
 */
correlatedFeatures HybridAnomalyDetector::create_cor_feature_cir(float pear, int feature1, int feature2, const TimeSeries &ts) {
    struct correlatedFeatures cor;
    // init names of features:
    cor.feature1 = ts.get_head_line_by_loc(feature1);
    cor.feature2 = ts.get_head_line_by_loc(feature2);
    // create array of points' pointers:
    Point** p = find_points_of_correlated_features(ts, cor.feature1, cor.feature2);

    cor.corrlation = pear;
    cor.minCircle = findMinCircle(p, ts.get_column_by_head(cor.feature1).size());
    cor.threshold = cor.minCircle.radius * 1.1;
    cor.isCircleDetect = true; // notify this is the circle detect (not a linear reg detect)
    return cor;
}