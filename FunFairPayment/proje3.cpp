#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <chrono>
using namespace std;

ofstream outputfile;  //for printing files

int num_consumer;    //number of total customers read from file's first line
int num_machines = 10;  //vending machine number is fixed
int num_payment = 0; //using for detecting how many prepayment are done

// these variables for printing total payment
int kevin = 0;
int bob = 0;
int stuart = 0;
int otto = 0;
int dave = 0;

pthread_t machine_thread[10]; //array for machine threads

// these mutexes are for communication between customer and the machine which customer wants to pay
pthread_mutex_t consumer_mutex[10];
pthread_mutex_t machine_mutex[10];
// this mutex is for preventing machine's from writing at the same time
pthread_mutex_t writing = PTHREAD_MUTEX_INITIALIZER;

// Stores every consumers information
typedef struct {
    pthread_t thread_id;
    string name; 
    int sleep;    
    int machine;   
    string company;     
    int amount;
}consumer;

// this vector behaves like a queue, if a customer wakes up (s)he is added here with her/him struct
vector<consumer> machine_q[10];

// consumer thread function, it takes consumer data as param
void *consumer_fnc(void *param){

    consumer *my_data = (consumer *)param;    // stores consumer data
    usleep(my_data->sleep*1000);  // sleeps consumer nanosec for fast result, then multiply by 1000 for prevent inconsistent data
    int m = my_data->machine;     //number of machine which consumer wants to make payment
    
    pthread_mutex_lock(&consumer_mutex[m]);  // if consumer wakes up, (s)he is ready to added to queue, but if there are 2 person
                                            // who wakes up at the same time, there will be an inconsistency so i lock consumer mutex
    machine_q[my_data->machine].push_back(*my_data);
    pthread_mutex_unlock(&machine_mutex[m]); // if consumer is added to queue, then (s)he is ready to make payment so, i unlock the
                                            // machine mutex which (s)he wants to make payment
}

// machine thread function, it takes consumer queue as param
void *machine_fnc(void *param){

    vector<consumer> *my_queue = (vector<consumer> *)param;
    
    while (num_payment != num_consumer){ //these fnc works as long as all consumers made their payment
        while (!my_queue->empty()){     // there should be a consumer who wants to make payment
            // assing consumer's info to variables
            consumer current_consumer = my_queue->at(0);
            int m = current_consumer.machine;
            string name = current_consumer.name;
            int a = current_consumer.amount;
            string c = current_consumer.company;

            // if there is a consumer who wants to make payment the machine (s)he chose is locked, this machine is in critical section
            pthread_mutex_lock(&machine_mutex[m]);

            // writing the output file current transaction and incrementing variables
            if (current_consumer.company == "Kevin")  {
                kevin += current_consumer.amount;
                pthread_mutex_lock(&writing);
                outputfile << "[vtm" << m << "]: " << name << "," << a << "TL," << c << endl;
                pthread_mutex_unlock(&writing);
            }
            if (current_consumer.company == "Bob") {
                bob += current_consumer.amount;    
                pthread_mutex_lock(&writing);
                outputfile << "[vtm" << m << "]: " << name << "," << a << "TL," << c << endl;
                pthread_mutex_unlock(&writing);
            }
            if (current_consumer.company == "Stuart"){
                stuart += current_consumer.amount;
                pthread_mutex_lock(&writing);
                outputfile << "[vtm" << m << "]: " << name << "," << a << "TL," << c << endl;
                pthread_mutex_unlock(&writing);
            }
            if (current_consumer.company == "Otto"){
                otto += current_consumer.amount;
                pthread_mutex_lock(&writing);
                outputfile << "[vtm" << m << "]: " << name << "," << a << "TL," << c << endl;
                pthread_mutex_unlock(&writing);
            }
            if (current_consumer.company == "Dave"){
                dave += current_consumer.amount;
                pthread_mutex_lock(&writing);
                outputfile << "[vtm" << m << "]: " << name << "," << a << "TL," << c << endl;
                pthread_mutex_unlock(&writing);
            }
            num_payment += 1;
            // the consumer is erased, the transaction is done
            my_queue->erase(my_queue->begin());
            // consumer mutex is unlocked because transaction is done
            pthread_mutex_unlock(&consumer_mutex[m]);
        }
    }
}

int main(int argc, char *argv[]) {
    fstream newfile;
    newfile.open(argv[1],ios::in);
    string input;
    input = argv[1];
    int dot = input.find(".");
    string input2 = input.substr(0, dot);
    string output =  input2 + "_log.txt";
    outputfile.open(output);

    string str;
    getline(newfile, str);
    num_consumer = stoi(str);

    // initializing mutexes
    for (int i = 0; i < 10; i++) {
        consumer_mutex[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        machine_mutex[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&machine_mutex[i]); // firstly all machine mutexes are locked if there is a ready payment, unlocked
    }

    pthread_t consumer_thread[num_consumer]; // array for consumer threads
    consumer consumer_data[num_consumer];   // array for consumer structs

    // input file is read and consumer info is assigned to consumer's struct variables
    for(int i = 0; i < num_consumer; i++){
        string sleep, machine, company, amount, name, temp;
        getline(newfile, str);
        stringstream delimstr(str);
        getline(delimstr,sleep,',');
        consumer_data[i].sleep = stoi(sleep);
        getline(delimstr,machine,',');
        consumer_data[i].machine = stoi(machine)-1;
        getline(delimstr,company,',');
        consumer_data[i].company = company;
        getline(delimstr,amount,',');
        consumer_data[i].amount = stoi(amount);
        temp = to_string(i+1);
        name = "Consumer" + temp;
        consumer_data[i].name = name;
    }
    newfile.close();
    // creating threads
    for (int i=0; i<10; i++){
        pthread_create(&machine_thread[i], NULL, machine_fnc, (void *)&machine_q[i]);
    }

    for (int i=0; i<num_consumer; i++){
        pthread_create(&consumer_thread[i], NULL, consumer_fnc, (void *)&consumer_data[i]);
    }
    // threads are done and join
    for (int i = 0; i < num_consumer; i++){
        pthread_join(consumer_thread[i], NULL);      
    }

    for (int i = 0; i < 10; i++){
        pthread_join(machine_thread[i], NULL);        
    }

    outputfile << "[Main]: All payments are completed" << endl;
    outputfile << "[Main]: Kevin: " << kevin << endl;
    outputfile << "[Main]: Bob: " << bob << endl;
    outputfile << "[Main]: Stuart: " << stuart << endl;
    outputfile << "[Main]: Otto: " << otto << endl;
    outputfile << "[Main]: Dave: " << dave << endl;

    outputfile.close();
    return 0;
}