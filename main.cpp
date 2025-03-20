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

int main() {

    cout << "Welcome to Annabelle Le's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: "; 
    string filename;
    cin >> filename;

    vector<vector<double> > data = load_data(filename);
    //if (data.empty()) {
      //  cout << "Could not load data.";
        //return 1;
    //}

   // cout << endl << "Starting Feature Search Demo..." << endl;
   
    cout << "Type the number of the algorithm you want to run:" << endl;
    cout << "1) Forward Selection" << endl;
    cout << "2> Backward Elimination" << endl;

    int answer;
    cin>> answer;

    if (answer == 1) {
        feature_search_demo(data);
    }
    else if (answer == 2) {
        backward_elimination(data);
    }
    else {
        cout << "Invalid choice. Input 1 or 2." << endl;
    }
    return 0;
}

