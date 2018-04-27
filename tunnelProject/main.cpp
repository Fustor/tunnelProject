#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

void *car(void *arg);

void *tunnel(void *arg);

 int done = 0;
static int direction;
static int maxCars;
static int totalCars = 0;
static int wbTotal = 0;
static int bbTotal = 0;
static int delayTotal = 0;
static int currentCars = 0;
static pthread_mutex_t trafficLock;
static pthread_mutex_t curNumLock;
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
    pthread_t tunnelid;
    pthread_create(&tunnelid,NULL,tunnel,(void*) NULL);
    vector<pthread_t> cars;
    while(getline(input,line,' ')) {
        totalCars++;

        int vals[3];

        sleep(stoi(line));

        //cout << line;

        //sleep(stoi(line));
        getline(input,line,' ');
        if(line == "WB") {
            vals[0] = 1;
            wbTotal++;
        }
        else {
            vals[0] = 2;
            bbTotal++;
        }
        getline(input,line,'\n');
        //cout << line << endl;
        vals[1] = stoi(line);
        vals[2] = totalCars;
        pthread_t tid;
        pthread_create(&tid,NULL,car, (void *) vals);
        cars.push_back(tid);
    }



    for(int i = 0;i<totalCars;i++) {
        pthread_join(cars.at(i),NULL);
    }

    //while(done == 0)
        //sleep(5);
    cout << wbTotal << " cars going to Whittier arrived at the tunnel." << endl;
    cout << bbTotal << " cars going to Bear Valley arrived at the tunnel." << endl;
    cout << delayTotal << " cars were delayed due to a full tunnel." << endl;
    return 0;
}
void *car(void *arg) {
    int i;
    bool delay = false;
    //must cast the argument
    int *val = (int*) arg;
    int heading = val[0];
    int travelTime = val[1];
    int carNum = val[2];
    if(heading == 1) {
        cout << "Car # " << carNum << " going to Whittier arrives at the tunnel." << endl;
        //pthread_mutex_lock(&curNumLock);
        while(true) {

               //pthread_cond_wait(&wb_can,&curNumLock);
               if(currentCars<maxCars && direction == 1)
                    break;
                else if(currentCars==maxCars && direction == 1)
                        delay = true;

        }
        if(delay) delayTotal++;
        cout << "Car # " << carNum << " going to Whittier enters the tunnel." << endl;
        pthread_mutex_lock(&curNumLock);
        currentCars++;
        pthread_mutex_unlock(&curNumLock);
        //cout << travelTime << endl;
        sleep(travelTime);
        cout << "Car # " << carNum << " going to Whittier exits the tunnel." << endl;
        pthread_mutex_lock(&curNumLock);
        currentCars--;
        pthread_mutex_unlock(&curNumLock);
    }
    else {
        cout << "Car # " << carNum << " going to Bear Valley arrives at the tunnel." << endl;

        while(true) {

                //pthread_cond_wait(&bb_can,&curNumLock);
                if(currentCars<maxCars && direction == 2)
                break;
                else if(currentCars==maxCars && direction == 2)
                        delay = true;

        }
        if(delay) delayTotal++;
        cout << "Car # " << carNum << " going to Bear Valley enters the tunnel." << endl;
        pthread_mutex_lock(&curNumLock);
        currentCars++;
        pthread_mutex_unlock(&curNumLock);
       // cout << travelTime << endl;
        sleep(travelTime);
        cout << "Car # " << carNum << " going to Bear Valley exits the tunnel." << endl;
        pthread_mutex_lock(&curNumLock);
        currentCars--;
        pthread_mutex_unlock(&curNumLock);
    }


} //car

void *tunnel(void *arg) {
    //int carMax = (int) arg;
    //sleep(1);
    while (done == 0) {

        pthread_mutex_lock(&trafficLock);


        cout << "The tunnel is now open to Whittier-bound traffic.\n";
        direction = 1;
        pthread_cond_broadcast(&wb_can);
        pthread_mutex_unlock(&trafficLock);
        sleep(5);
        pthread_mutex_lock(&trafficLock);
        direction = 0;
        cout << "The tunnel is now closed to ALL traffic.\n";
        pthread_mutex_unlock(&trafficLock);
        sleep(5);
        pthread_mutex_lock(&trafficLock);

        cout << "The tunnel is now open to Bear Valley-bound traffic.\n";
        direction = 2;
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
