/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#include "SimpleAnomalyDetector.h"


SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

/**
 * This function responsible about the learn part (learn the normal)
 * @param ts - time series with the normal info
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    /*
     * find the correlated features in the time series- checking every couple of features.
     */
    for (int i = 0; i < ts.get_num_columns(); ++i) {
        // for case the client ask for new threshold in command 3:
        float m = user_threshold == 100 ? 0.9 : user_threshold;

        float h = 0.5;

        // create array from info in feature 1:
        float *arrayI = from_vector_to_array(ts.get_column_by_loc(i));
        for (int j = i + 1; j < ts.get_num_columns(); ++j) {

            // create array from info in feature 2:
            float *arrayJ = from_vector_to_array(ts.get_column_by_loc(j));

            // find the pearson (correlation) of the features together: (the last arg is the size of the feature vector)
            float pear = abs(pearson(arrayI, arrayJ, ts.get_column_by_loc(j).size()));

            if (pear >= m) {
                m = pear;
                create_cor_feature_sim(pear, i, j, ts);
            } else if (pear > h && pear < m) {
                h = pear;
                correlatedFeatures c = create_cor_feature_cir(pear, i, j, ts);
                cf.push_back(c);
            }
        }
    }
}

/**
 * This function create a correlatedFeatures object by simple detect (linear reg).
 * @param pear - the correlation of the correlated features.
 * @param feature1 - the index of feature 1 in time series.
 * @param feature2 - the index of feature 2 in time series.
 * @param ts - time series that contain all info about the features.
 */
void SimpleAnomalyDetector::create_cor_feature_sim(float pear, int feature1, int feature2,const TimeSeries &ts){
    struct correlatedFeatures cor;
    // init names of features:
    cor.feature1 = ts.get_head_line_by_loc(feature1);
    cor.feature2 = ts.get_head_line_by_loc(feature2);
    // create array of points' pointers:
    Point** p = find_points_of_correlated_features(ts, cor.feature1, cor.feature2);

    cor.corrlation = pear;
    cor.lin_reg = find_linear_reg(ts, cor.feature1, cor.feature2);
    cor.threshold = max_threshold(p, cor.lin_reg, ts.get_column_by_head(cor.feature1).size());
    cor.isCircleDetect = false; // notify this is the linear reg detect (not a circle detect)
    cf.push_back(cor); // add the correlatedFeatures we created to the vector of all correlatedFeatures.
}

/**
 * This function return the linear reg of the points of the correlated features.
 * @param ts - time series that contain all info about the features.
 * @param feat1 - the title of first feature in correlatedFeatures.
 * @param feat2 - the title of second feature in correlatedFeatures.
 * @return linear reg of these correlatedFeatures.
 */
Line find_linear_reg (const TimeSeries &ts, string feat1, string feat2) {
    int sizeOfFeature = ts.get_column_by_head(feat1).size();
    Point** pointsArray = find_points_of_correlated_features(ts, feat1, feat2);
    Line l = linear_reg(pointsArray, sizeOfFeature);
    return l;
}

/**
 * This function convert vector to array (so we can use it in "pearson")
 * @param feature - the vector that contains the feature info
 * @return array of info of these correlatedFeatures.
 */
float* from_vector_to_array(vector<float> feature) {
    float* array = new float[feature.size()];
    for (int i = 0; i < feature.size();i++) {
        array[i] = feature[i];
    }
    return array;
}

 /**
  * This function create the points from the info in the correlated features.
  * @param ts - time series that contain all info about the features
  * @param feat1 - the title of first feature in correlatedFeatures.
  * @param feat2 - the title of second feature in correlatedFeatures.
  * @return
  */
Point** find_points_of_correlated_features (const TimeSeries &ts, string feat1, string feat2) {
    int sizeOfFeature = ts.get_column_by_head(feat1).size();
    Point** pointsArray = new Point*[sizeOfFeature];
    for (int i = 0; i < sizeOfFeature; i++) {
        // create point from line i in feature 1 and line i in feature 2.
        Point *p = new Point(ts.get_column_by_head(feat1)[i],ts.get_column_by_head(feat2)[i]);
        pointsArray[i] = p;
    }
    return pointsArray;
}

/**
 * This function return the maximum threshold.
 * @param points - the points in the graph of the correlated features.
 * @param line - the linear reg of the graph.
 * @param size - the size of the points array.
 * @return maximum threshold value
 */
float max_threshold (Point** points, Line line, int size) {
    float maxThreshold = 0;
    for (int i = 0; i < size; ++i) {
        float d = dev(*points[i], line);
        if (d > maxThreshold) {
            maxThreshold = d;
        }
    }
    maxThreshold *= 1.1;
    return maxThreshold;

}

/**
 * This function responsible about the test part (detect exceptions)
 * @param ts -time series
 * @return a vector contain all exceptions (AnomalyReport)
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reports;

    for (const correlatedFeatures &i : this->cf) {

        if (i.isCircleDetect) { // the case of circle detect
            vector<AnomalyReport> reportsList = check_if_inside(ts, i);
            for (AnomalyReport s: reportsList) {
                if (s.timeStep != 0) {
                    reports.push_back(s);
                }
            }
        }

        else { // not a circle detector
            vector<AnomalyReport> reportsList = check_if_detect(ts, i);
            for (AnomalyReport s: reportsList){
                if (s.timeStep != 0) {
                    reports.push_back(s);
                }
            }
        }
    }
    return reports;
}
/**
 * This function check the detection for linear reg detect
 * @param ts - time series
 * @param cor - current correlatedFeatures we check the new test info.
 * @return an AnomalyReport if there is an exception, an empty AnomalyReport if there isn't.
 */
vector<AnomalyReport> check_if_detect(const TimeSeries &ts, const correlatedFeatures &cor) {
    vector<AnomalyReport> reports;
    // get size of vector of current correlatedFeatures:
    int sizeOfFeature = ts.get_column_by_head(cor.feature1).size();
    // create line of new info from test ts:
    Line line = cor.lin_reg;

    for (int i = 0; i < sizeOfFeature; i++) {
        // create point from line cor in feature 1 and line cor in feature 2.
        Point p = Point(ts.get_column_by_head(cor.feature1)[i],
                        ts.get_column_by_head(cor.feature2)[i]);

        if (cor.threshold < dev(p, line)) {
            AnomalyReport a(cor.feature1 + "-" + cor.feature2, i + 1);
            reports.push_back(a);
        }
    } return reports;
}


correlatedFeatures SimpleAnomalyDetector::create_cor_feature_cir(float pear, int feature1, int feature2, const TimeSeries &ts) {
    correlatedFeatures c;
    return c;
}

vector<AnomalyReport> SimpleAnomalyDetector::check_if_inside(const TimeSeries &ts, const correlatedFeatures &i) {
    vector<AnomalyReport> nullVec;
    AnomalyReport a("", 0);
    nullVec.push_back(a);
    return nullVec;
}
