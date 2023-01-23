#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <chrono>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace std::chrono;

vector<int> myvector;
int arr[10001];
int N;
int mini = 10001;
int maxi = -10001;
int sum = 0;
int howmany = 0;
int mode = 0;
int range = 0;
double harmonic_sum = 0.0;
double median = 0.0;
double arithmetic_mean = 0.0;
double harmonic_mean = 0.0;
double deviation = 0.0;
double interquartile = 0.0;

void min_fnc(){
    for (int i=0; i < N; i++){
        mini = min(mini, myvector[i]);
    }
}

void max_fnc(){
    for (int i=0; i < N; i++){
        maxi = max(maxi, myvector[i]);
    }
}

void range_fnc(){
    for (int i=0; i < N; i++){
        mini = min(mini, myvector[i]);
        maxi = max(maxi, myvector[i]);
    }
    range=maxi-mini;
}

void mode_fnc(){
    for (int i=0; i < 10001; i++){ 
        if (arr[i] > howmany){
            howmany = arr[i];
            mode = i;
        }
    }
}

void median_fnc(){
    if(N%2 == 1){
        median = myvector[N/2];
    }
    else{
        median = double(myvector[N/2]+myvector[N/2-1]) / (double)2.0;
    }
}

void sum_fnc(){
    for (int i=0; i < N; i++){
        sum += myvector[i];
    }
}

void arithmetic_mean_fnc(){
    sum = 0;
    for (int i=0; i < N; i++){
        sum += myvector[i];
    }
    arithmetic_mean = (double)sum / (double)N;
}

void harmonic_mean_fnc(){
    harmonic_sum = 0.0;
    for (int i=0; i < N; i++){
        harmonic_sum += 1.0/(double)(myvector[i]);
    }
    harmonic_mean = (double)N/(double)harmonic_sum;
}

void deviation_fnc(){

    double arithmetic_mean = 0.0;
    int sum = 0;
    for (int i=0; i < N; i++){
        sum += myvector[i];
    }
    arithmetic_mean = (double)sum / (double)N;

    for (int i = 0; i < N; i++) {
        deviation += pow(myvector[i] - arithmetic_mean, 2);
    }
    deviation = sqrt(deviation / (N-1));

}

void interquartile_fnc(){
    int mid = N/2;
    int lower = mid/2;
    int upper = mid+lower;
    double lowerqt;
    double upperqt;
    
    if (mid % 2 == 0){
        lowerqt = (double)(myvector[lower] + myvector[lower-1]) / 2.0;
        if (N%2 == 0){
            upperqt = (double)(myvector[upper] + myvector[upper-1]) / 2.0;
        }
        else{
            upperqt = (double)(myvector[upper] + myvector[upper+1]) / 2.0;
        }
    }
    else{
        lowerqt = myvector[lower];
        if (N%2 == 0){
            upperqt = myvector[upper];
        }
        else{
            upperqt = myvector[upper+1];
        }
    }
    interquartile = upperqt-lowerqt;
}

int main(int argc, char *argv[]) {
    srand (time(0));  //random num generation
    N = stoi(argv[1]);

    for (int i=0; i < N; i++){ 
        int element = (rand()%9001)+1000;
        myvector.push_back(element);
        arr[element]++;
    }

    sort(myvector.begin(), myvector.end());
    auto start = high_resolution_clock::now();

    min_fnc();
    max_fnc();
    range_fnc();
    mode_fnc();
    median_fnc();
    sum_fnc();
    arithmetic_mean_fnc();
    harmonic_mean_fnc();
    deviation_fnc();
    interquartile_fnc();

    auto stop = high_resolution_clock::now();
    //file operations
    ofstream myfile;
    myfile.open ("output1.txt");
    myfile << mini << endl;
    myfile << maxi << endl;
    myfile << range << endl;
    myfile << mode << endl;
    myfile << median << endl;
    myfile << sum << endl;
    myfile << fixed << setprecision(5) << arithmetic_mean << endl;
    myfile << fixed << setprecision(5) << harmonic_mean << endl;
    myfile << fixed << setprecision(5) << deviation << endl;
    myfile << interquartile << endl;

    auto duration = duration_cast<nanoseconds>(stop - start);
    myfile << duration.count()/1e9 << endl;
    myfile.close();
}