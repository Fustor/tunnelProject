#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

void *car(void *arg);

void *tunnel(void *arg);

 int done = 0;
static int direction;
static int maxCars;
static int totalCars = 0;
static int currentCars;
static pthread_mutex_t trafficLock;
static pthread_cond_t wb_can = PTHREAD_COND_INITIALIZER;
static pthread_cond_t bb_can = PTHREAD_COND_INITIALIZER;
int main()
{

    string line;
    ifstream input;
    input.open("input30.txt");
    getline(input,line, '\n');
    maxCars = stoi(line);
    // << maxCars << endl;
    pthread_t tid;
    pthread_create(&tid,NULL,tunnel,(void*) NULL);
    while(getline(input,line,' ')) {
        totalCars++;

        int vals[3];

        sleep(stoi(line));

        //cout << line;

        //sleep(stoi(line));
        getline(input,line,' ');
        if(line == "WB") {
            vals[0] = 1;
        }
        else {
            vals[0] = 2;
        }
        getline(input,line,'\n');
        //cout << line << endl;
        vals[1] = stoi(line);
        vals[2] = totalCars;
        pthread_t tid;

        pthread_create(&tid,NULL,car, (void *) vals);
    }





    sleep(5);
    return 0;
}
void *car(void *arg) {
    int i;
    //must cast the argument
    int *val = (int*) arg;
    int heading = val[0];
    int travelTime = val[1];
    int carNum = val[2];
    cout << "Car " << carNum << " heading " << val[0] << " taking " << val[1] << endl;
} //car

void *tunnel(void *arg) {
    //int carMax = (int) arg;

    while (done == 0) {
        pthread_mutex_lock(&trafficLock);
        direction = 1;
        cout << "The tunnel is now open to Whittier-bound traffic.\n";
        pthread_cond_broadcast(&wb_can);
        pthread_mutex_unlock(&trafficLock);
        sleep(5);
        pthread_mutex_lock(&trafficLock);
        direction = 0;
        cout << "The tunnel is now closed to ALL traffic.\n";
        pthread_mutex_unlock(&trafficLock);
        sleep(5);
        pthread_mutex_lock(&trafficLock);
        direction = 2;
        cout << "The tunnel is now open to Bear Valley-bound traffic.\n";
        pthread_cond_broadcast(&bb_can);
        pthread_mutex_unlock(&trafficLock);
        sleep(5);
        pthread_mutex_lock(&trafficLock);
        direction = 0;
        cout << "The tunnel is now closed to ALL traffic.\n";
        pthread_mutex_unlock(&trafficLock);
        sleep(5);

    }

}
