#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//LOAD DATA FUNCTION
vector<vector<double> > load_data(const string& filename) {
    ifstream file(filename);
    vector<vector<double>> data;
    string line;

    if (!file){
        cout << "Failed to open file." << endl;
        return data; //return empty if file can't open
    }

        while (getline(file, line)) {
            vector<double> row;
            double value;
            stringstream ss(line); //stringstream allows for extraction

            while (ss >> value) {
                row.push_back(value); 
            }
            data.push_back(row); //add row to vector
        }
        file.close();
        return data;
    }


//EUCLIDEAN DISTANCE FUNC
//do a for loop for x and y 
//square the difference and continue summing it together
//finally square root the total 
double euclidean_dist(const vector<double>& x, const vector<double>& y) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        sum += pow(x[i] - y[i], 2);
    }
    return sqrt(sum);
}
