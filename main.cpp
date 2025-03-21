#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//LOAD DATA FUNCTION
//stringstream code found online
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

//LEAVING ONE OUT CV FUNC
//leaves 1 obj out at a time, find its nn using selected features
//checks if classification is correct and calculates accuracy

double leave_one_out_cross_validation(vector<vector<double>> data, vector<int> current_set, int feature_to_add) {
    int num_objects = data.size(); //total num of obj
    int correct_count = 0; //number of correctly classified 

    for (int i = 0; i < num_objects; i++) {
        vector<double> test_object; //iterate through to get test objects
        
        for (int f : current_set) test_object.push_back(data[i][f]);
        test_object.push_back(data[i][feature_to_add]); //use subset of features to compare

        int actual_label = data[i][0]; 
        double best_distance = numeric_limits<double>::infinity(); //best distance is smallest distance found
        int nn_label = -1; //predicted class of nn

        for (int k = 0; k < num_objects; k++) {
            if (k == i) continue;

            vector<double> neighbor_features; //neighbor should only have imp features
            for (int f : current_set) neighbor_features.push_back(data[k][f]);
            neighbor_features.push_back(data[k][feature_to_add]);

            double distance = euclidean_dist(test_object, neighbor_features);
            if (distance < best_distance) {
                best_distance = distance;
                nn_label = data[k][0]; 
            }
        }

        if (actual_label == nn_label) correct_count++; //if nn class = actual class, incr correct classification
    }
    //return accuracy = num correctly classified / num objects
    return (double)correct_count / num_objects; 
}

//feature search demo is the forward selection func
//adds one feature at a time, make sure to add accuracy 
void forward_selection(vector<vector<double>>& data) {
    vector<int> chosen_features;  // features chosen so far
    vector<int> best_feature_set; 
    int num_features = data[0].size() - 1;  //features - label
    double best_accuracy_so_far = 0.0;  

    cout << "Beginning search\n" << endl;

    for (int i = 1; i <= num_features; i++) {
        int best_feature = -1;
        double best_accuracy = 0.0; 

        //continue including unchosen features by looping through
        for (int f = 1; f <= num_features; f++) {
            if (find(chosen_features.begin(), chosen_features.end(), f) != chosen_features.end()) 
                continue;

            double accuracy = leave_one_out_cross_validation(data, chosen_features, f); //will test accuracy when feature added

            //show accuracy
            cout << "Using feature(s) {";
            for (int feat : chosen_features) cout << feat << ",";
            cout << f << "} - Accuracy: " << accuracy * 100 << "%" << endl;

            if (accuracy > best_accuracy) { //if that feature gives best accuracy, update accuracy/best feature
                best_feature = f;
                best_accuracy = accuracy;
            }
        }

        //remember to do if no improvements
        if (best_feature == -1) {
            cout << "No improvement found. Stopping search.\n" << endl;
            break;
        }

        chosen_features.push_back(best_feature); //add in best geature

        //update if accuracy improves more than best so far
        if (best_accuracy > best_accuracy_so_far) {
            best_accuracy_so_far = best_accuracy;
            best_feature_set = chosen_features;
        }

        cout << "\nBest feature set so far: {";
        for (size_t j = 0; j < chosen_features.size(); j++) {
            cout << chosen_features[j];
            if (j < chosen_features.size() - 1) cout << ", ";
        }
        cout << "} - Accuracy: " << best_accuracy * 100 << "%" << endl;
    }

    cout << "\nFinal best feature set: {";
    for (size_t j = 0; j < best_feature_set.size(); j++) {
        cout << best_feature_set[j];
        if (j < best_feature_set.size() - 1) cout << ",";
    }
    cout << "} - Accuracy: " << best_accuracy_so_far * 100 << "%" << endl;
}


//CS170 DEMO USED TO FIND NN FOR EACH DATA POINT
double cs170_demo(const vector<vector<double> >& data){

    int num_objects = data.size(); //write accuracy: correct predictions/ total objects
    int num_correctly_classified = 0;

    for (int i = 0; i < num_objects; i++) {
        vector<double> object_to_classify(data[i].begin() + 1, data[i].end());
        int label_object = data[i][0]; //class label

        double nn_distance = numeric_limits<double>::infinity();
        int nn_location = -1;
        int nn_label = -1;

        for (int k = 0; k < num_objects; k++) {
            if (k!= i) { //if point is not equal to itself, ask if its nn w nxt pt
            cout << "Ask if " << i+1 << " is nearest neigbor with " << k+1 << endl;

            double dist = euclidean_dist(object_to_classify, vector<double>(data[k].begin() + 1, data[k].end()));

            if (dist < nn_distance) {
                nn_distance = dist;
                nn_location = k;
                nn_label = data[k][0];
            }

        }
    }
    cout << "Object " << i + 1 << " is class " << label_object << endl;
    cout << "Its nearest neighbor is " << nn_location
             << " which is in class " << nn_label << endl;
    // now do the accuracy check 
    //if ur object class equal to nn's class increment the correct classified
    if (label_object == nn_label) {
        num_correctly_classified++;
    }
}
 double accuracy = static_cast<double>(num_correctly_classified) / num_objects;
    cout << "Accuracy: " << accuracy * 100 << "%\n";
    return accuracy;
}


//so now for backward elimination
//starts off w full dataset & then removes features one by one

void backward_elimination(vector<vector<double>>& data) {
    vector<int> current_features; //for backward has all data currently
    vector<int> best_feature_set; 
    int num_features = data[0].size() - 1;

    for (int i = 1; i <= num_features; i++) {
        current_features.push_back(i);
    }

    cout << "Beginning search\n" << endl;

    //have to have initial accuracy before removing any features
    double best_accuracy_so_far = leave_one_out_cross_validation(data, current_features, -1);
    best_feature_set = current_features;

    cout << "Initial Set {";

    for (size_t j = 0; j < current_features.size(); j++) {
        cout << current_features[j];
        if (j <current_features.size() - 1) cout << ",";
    }
    cout << "} - Accuracy: " << best_accuracy_so_far * 100 << "%" << endl;

    for (int i = num_features; i >= 1; i--) {
        int worst_feature = -1;
        double best_accuracy = 0.0; //stores best accuracy after removing the feature

        //iterate through to remove one feature at a time
        for (size_t j = 0; j < current_features.size(); j++) {
            vector<int> temp_features = current_features; //where current features is temp feature
            temp_features.erase(temp_features.begin() + j);  //test to see what sccuracy would be like after removing a feature in temp features

            double accuracy = leave_one_out_cross_validation(data, temp_features, -1);

            cout << "Removing feature(s) {";
            for (int feat : temp_features) cout << feat << " ";
            cout << "} - Accuracy: " << accuracy * 100 << "%" << endl;
            
            //after removal it improves accuracy, mark it as a worst feature
             if (accuracy > best_accuracy) {
                best_accuracy = accuracy;
                worst_feature = current_features[j];  
            }
        }

        if (worst_feature == -1) {
            cout << "No improvement found. Stopping elimination.\n" << endl;
            break;
        }

        //remove it 
        current_features.erase(remove(current_features.begin(), current_features.end(), worst_feature), current_features.end());

        //continue to update accuracy
        if (best_accuracy > best_accuracy_so_far) {
            best_accuracy_so_far = best_accuracy;
            best_feature_set = current_features;
        }
        
        cout << "\nBest feature set so far: {";
        for (size_t j = 0; j < current_features.size(); j++) {
            cout << current_features[j]; 
            if (j < current_features.size() - 1) cout << ",";
        }
        cout << "} - Accuracy: " << best_accuracy * 100 << "%" << endl;
    }

    cout << "\nFinal best feature set: {";
    for (size_t j = 0; j < best_feature_set.size(); j++) {
        cout << best_feature_set[j];
        if (j < best_feature_set.size() - 1) cout << ",";
    }
    cout << "} - Accuracy: " << best_accuracy_so_far * 100 << "%" << endl;
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
    cout << "2) Backward Elimination" << endl;

    int answer;
    cin>> answer;

    if (answer == 1) {
        forward_selection(data);
    }
    else if (answer == 2) {
        backward_elimination(data);
    }
    else {
        cout << "Invalid choice. Input 1 or 2." << endl;
    }
    return 0;
}
