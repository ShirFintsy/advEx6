/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#include "timeseries.h"

/**
 * Constructor.
 * @param CSVfileName - file name
 */
TimeSeries::TimeSeries(const char* CSVfileName) : csvName(CSVfileName){
    // open the file and check if valid:
    ifstream myFile(csvName);
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");
    string stringHeadLine;
    // get the first line of the file (the headlines):
    getline(myFile, stringHeadLine);
    // convert the string to stream:
    istringstream streamHeadline(stringHeadLine);
    string item;
    // insert each headline to the corresponding column:
    while (getline(streamHeadline, item, ',')) {
        set_headlines(item);
    }
    set_num_columns(get_headlines().size()); // check it.
    string line;
    // create the specific number of columns:
    for (int i = 0; i < num_columns; i ++) {
        set_columns_size();
    }
    // read each line in the file:
    while (std::getline(myFile, line)) {
        istringstream streamLine(line);
        // check the line is valid:
        if(count(line.begin(), line.end(), ',') != get_num_columns() - 1) {
            throw runtime_error("table is invalid");
        }
        // insert value to each column:
        for (unsigned int i = 0; i < get_num_columns(); i++) {
            getline(streamLine, item, ',');
            float num = stof(item);
            set_columns_by_loc(num, i);
        }
    }
}

/**
 * This function find vector of feature by the feature name.
 * @param headLine - feature name (column in file)
 * @return vector of feature.
 */
vector<float> TimeSeries::get_column_by_head(string headLine) const {
    for (int i = 0; i < this->num_columns; i++)
        if (this->headLines[i] == headLine)
            return this->columns[i];
    throw runtime_error("this headline is not in the file");
}

/**
 * This function find vector of feature by location in all vectors.
 * @param num - location
 * @return vector of feature.
 */
vector<float> TimeSeries::get_column_by_loc(int num) const{
    if (num < 0 || num > this->num_columns)
        throw runtime_error("the number of the column is invalid");
    return this->columns[num];
}

/**
 * This function return the name of the feature by location in all vectors.
 * @param num - location
 * @return name of require feature
 */
string TimeSeries::get_head_line_by_loc(int num) const{
    if (num < 0 || num > this->num_columns)
        throw runtime_error("the number of the column is invalid");
    return this->headLines[num];
}


