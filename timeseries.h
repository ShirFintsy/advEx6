/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
using namespace std;

class TimeSeries{
private:
    // the array with headlines of each column:
    vector<string> headLines;
    // 2D array of vectors (columns):
    vector<vector<float>> columns;
    unsigned int num_columns;
    string csvName;

public:
    // consturctor:
    TimeSeries(const char* CSVfileName);
    // getters:
    vector<vector<float>> get_columns() const {
        return this->columns;
    }
    vector<string> get_headlines() const {
        return this->headLines;
    }
    /**
     * this function returns the number of columns in the table.
     * @return
     */
    int get_num_columns() const {
        return this->num_columns;
    }
    // stters:
    void set_columns_size(){
        this->columns.emplace_back();
    }
    void set_headlines(string item) {
        this->headLines.push_back(item);
    }
    void set_num_columns (unsigned int size) {
        this->num_columns = size;
    }
    void set_columns_by_loc (float num, unsigned int i) {
        this->columns[i].push_back(num);
    }
    /**
     * this function return a specific column by its headline.
     * @param headLine.
     * @return specific column.
     */
    vector<float> get_column_by_head(string headLine) const;

    /**
     * this function return a specific headline by its number.
     * @param num .
     * @return headline.
     */
    string get_head_line_by_loc(int num) const;
    /**
     * this function return a specific column by its place.
     * @param headLine.
     * @return specific column.
     */
    vector<float> get_column_by_loc(int num) const;
};



#endif /* TIMESERIES_H_ */