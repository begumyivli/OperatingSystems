#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <pthread.h>
#include <chrono>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace std::chrono;

vector<int> myvector;
int arr[10001];
int N;

typedef struct thread_data {
    int tmin;
    int tmax;
    int trange;
    int tmode;
    double tmedian;
    int tsum;
    double tarithmetic_mean;
    double tharmonic_mean;
    double tdeviation;
    double tinterquartile;

} thread_data;

void* min_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    int mini = 10001;
    for (int i=0; i < N; i++){
        mini = min(mini, myvector[i]);
    }
    tdata->tmin=mini;
    return NULL;
}

void* max_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    int maxi = -10001;
    for (int i=0; i < N; i++){
        maxi = max(maxi, myvector[i]);
    }
    tdata->tmax=maxi;
    return NULL;
}

void* range_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    int mini = 10001;
    int maxi = -10001;
    for (int i=0; i < N; i++){
        mini = min(mini, myvector[i]);
        maxi = max(maxi, myvector[i]);
    }
    tdata->trange=maxi-mini;
    return NULL;
}

void* mode_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    int howmany = 0;
    int mode = 0;
    for (int i=0; i < 10001; i++){ 
        if (arr[i] > howmany){
            howmany = arr[i];
            mode = i;
        }
    }
    tdata->tmode=mode;
    return NULL;
}

void* median_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    double median = 0.0;
    if(N%2 == 1){
        median = myvector[N/2];
    }
    else{
        median = double(myvector[N/2]+myvector[N/2-1]) / (double)2.0;
    }
    tdata->tmedian=median;
    return NULL;
}

void* sum_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    int sum = 0;
    for (int i=0; i < N; i++){
        sum += myvector[i];
    }
    tdata->tsum=sum;
    return NULL;
}

void* arithmetic_mean_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    double arithmetic_mean = 0.0;
    int sum = 0;
    for (int i=0; i < N; i++){
        sum += myvector[i];
    }
    arithmetic_mean = (double)sum / (double)N;
    tdata->tarithmetic_mean=arithmetic_mean;
    return NULL;
}

void* harmonic_mean_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    double harmonic_sum = 0.0;
    double harmonic_mean = 0.0;
    for (int i=0; i < N; i++){
        harmonic_sum += 1.0/(double)(myvector[i]);
    }
    harmonic_mean = (double)N/(double)harmonic_sum;
    tdata->tharmonic_mean=harmonic_mean;
    return NULL;
}

void* deviation_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    double deviation = 0.0;

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
    tdata->tdeviation=deviation;
    return NULL;

}

void* interquartile_fnc(void *param){
    thread_data *tdata=(thread_data *)param;
    double interquartile = 0.0;

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
    tdata->tinterquartile=interquartile;
    return NULL;
}

int main(int argc, char *argv[]) {
    srand (time(0));  //random num generation
    N = stoi(argv[1]);
    timespec ts1, ts2;

    for (int i=0; i < N; i++){
        int element = (rand()%9001)+1000;
        myvector.push_back(element);
        arr[element] += 1;
    }
    sort(myvector.begin(), myvector.end());
    auto start = high_resolution_clock::now();
    
    thread_data tdata;
    pthread_t tid_1;
    pthread_t tid_2;
    pthread_t tid_3;
    pthread_t tid_4;
    pthread_t tid_5;
    pthread_t tid_6;
    pthread_t tid_7;
    pthread_t tid_8;
    pthread_t tid_9;
    pthread_t tid_10;
	pthread_create(&tid_1, NULL, min_fnc, (void *)&tdata);
    pthread_create(&tid_2, NULL, max_fnc, (void *)&tdata);
    pthread_create(&tid_3, NULL, range_fnc, (void *)&tdata);
    pthread_create(&tid_4, NULL, mode_fnc, (void *)&tdata);
    pthread_create(&tid_5, NULL, median_fnc, (void *)&tdata);
    pthread_create(&tid_6, NULL, sum_fnc, (void *)&tdata);
    pthread_create(&tid_7, NULL, arithmetic_mean_fnc, (void *)&tdata);
    pthread_create(&tid_8, NULL, harmonic_mean_fnc, (void *)&tdata);
    pthread_create(&tid_9, NULL, deviation_fnc, (void *)&tdata);
    pthread_create(&tid_10, NULL, interquartile_fnc, (void *)&tdata);
    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
    pthread_join(tid_3, NULL);
    pthread_join(tid_4, NULL);
    pthread_join(tid_5, NULL);
    pthread_join(tid_6, NULL);
    pthread_join(tid_7, NULL);
    pthread_join(tid_8, NULL);
    pthread_join(tid_9, NULL);
    pthread_join(tid_10, NULL);
    
    auto stop = high_resolution_clock::now();
    ofstream myfile;
    myfile.open ("output2.txt");
    myfile << tdata.tmin << endl;
    myfile << tdata.tmax << endl;
    myfile << tdata.trange << endl;
    myfile << tdata.tmode << endl;
    myfile << tdata.tmedian << endl;
    myfile << tdata.tsum << endl;
    myfile << fixed << setprecision(5) << tdata.tarithmetic_mean << endl;
    myfile << fixed << setprecision(5) << tdata.tharmonic_mean << endl;
    myfile << fixed << setprecision(5) << tdata.tdeviation << endl;
    myfile << tdata.tinterquartile << endl;

    auto duration = duration_cast<nanoseconds>(stop - start);
    myfile << duration.count()/1e9 << endl;
    myfile.close();
}