#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <vector>
#include <semaphore.h>

using namespace std;

// Define "constant" values here
#define UNIDIRECTIONAL 0
#define WORLDEND 30
#define NUM_LIZARDS 20
#define NUM_CATS 2
#define MAX_LIZARD_CROSSING 4
#define MAX_LIZARD_SLEEP 3
#define MAX_CAT_SLEEP 3
#define MAX_LIZARD_EAT 5
#define CROSS_SECONDS 2

// Declare global variables here
int numCrossingSago2MonkeyGrass = 0;
int numCrossingMonkeyGrass2Sago = 0;
int debug = 1;
int running = 1;
mutex drivewayMutex;
sem_t sagoToMonkeyGrassSem;
sem_t monkeyGrassToSagoSem;
sem_t catAlertSem;

class Cat {
    int _id;
    thread* _catThread;
public:
    Cat(int id);
    int getId();
    void run();
    void wait();
private:
    void sleepNow();
    static void catThread(Cat *aCat);
};

Cat::Cat(int id) : _id(id) {}

int Cat::getId() {
    return _id;
}

void Cat::run() {
    _catThread = new thread(&Cat::catThread, this);
}

void Cat::wait() {
    _catThread->join();
}

void Cat::sleepNow() {
    int sleepSeconds = 1 + (int)(random() / (double)RAND_MAX * MAX_CAT_SLEEP);
    if (debug) {
        cout << "[" << _id << "] cat sleeping for " << sleepSeconds << " seconds" << endl;
    }
    sleep(sleepSeconds);
}

void Cat::catThread(Cat *aCat) {
    while (running) {
        aCat->sleepNow();
        // Check for too many lizards crossing
        if (numCrossingSago2MonkeyGrass + numCrossingMonkeyGrass2Sago > MAX_LIZARD_CROSSING) {
            cout << "\tThe cats are happy - they have toys.\n";
            exit(-1);
        }
    }
}

class Lizard {
    int _id;
    thread* _aLizard;
public:
    Lizard(int id);
    int getId();
    void run();
    void wait();
private:
    void sago2MonkeyGrassIsSafe();
    void crossSago2MonkeyGrass();
    void madeIt2MonkeyGrass();
    void eat();
    void monkeyGrass2SagoIsSafe();
    void crossMonkeyGrass2Sago();
    void madeIt2Sago();
    void sleepNow();
    static void lizardThread(Lizard *aLizard);
};

Lizard::Lizard(int id) : _id(id) {}

int Lizard::getId() {
    return _id;
}

void Lizard::run() {
    _aLizard = new thread(&Lizard::lizardThread, this);
}

void Lizard::wait() {
    if (_aLizard != nullptr) {
        _aLizard->join();
    }
}

void Lizard::sleepNow() {
    int sleepSeconds = 1 + (int)(random() / (double)RAND_MAX * MAX_LIZARD_SLEEP);
    if (debug) {
        cout << "[" << _id << "] sleeping for " << sleepSeconds << " seconds" << endl;
    }
    sleep(sleepSeconds);
}

void Lizard::sago2MonkeyGrassIsSafe() {
    sem_wait(&sagoToMonkeyGrassSem);
}

void Lizard::crossSago2MonkeyGrass() {
    numCrossingSago2MonkeyGrass++;
    if (numCrossingMonkeyGrass2Sago && UNIDIRECTIONAL) {
        cout << "\tCrash! We have a pile-up on the concrete." << endl;
        cout << "\t" << numCrossingSago2MonkeyGrass << " crossing sago -> monkey grass" << endl;
        cout << "\t" << numCrossingMonkeyGrass2Sago << " crossing monkey grass -> sago" << endl;
        exit(-1);
    }
    sleep(CROSS_SECONDS);
    numCrossingSago2MonkeyGrass--;
}

void Lizard::madeIt2MonkeyGrass() {
    if (debug) {
        cout << "[" << _id << "] made the sago -> monkey grass crossing" << endl;
    }
    sem_post(&monkeyGrassToSagoSem);
}

void Lizard::eat() {
    int eatSeconds = 1 + (int)(random() / (double)RAND_MAX * MAX_LIZARD_EAT);
    if (debug) {
        cout << "[" << _id << "] eating for " << eatSeconds << " seconds" << endl;
    }
    sleep(eatSeconds);
    if (debug) {
        cout << "[" << _id << "] finished eating" << endl;
    }
}

void Lizard::monkeyGrass2SagoIsSafe() {
    sem_wait(&monkeyGrassToSagoSem);
}

void Lizard::crossMonkeyGrass2Sago() {
    numCrossingMonkeyGrass2Sago++;
    if (numCrossingSago2MonkeyGrass && UNIDIRECTIONAL) {
        cout << "\tOh No!, the lizards have cats all over them." << endl;
        cout << "\t" << numCrossingSago2MonkeyGrass << " crossing sago -> monkey grass" << endl;
        cout << "\t" << numCrossingMonkeyGrass2Sago << " crossing monkey grass -> sago" << endl;
        exit(-1);
    }
    sleep(CROSS_SECONDS);
    numCrossingMonkeyGrass2Sago--;
}

void Lizard::madeIt2Sago() {
    if (debug) {
        cout << "[" << _id << "] made the monkey grass -> sago crossing" << endl;
    }
    sem_post(&sagoToMonkeyGrassSem);
}

void Lizard::lizardThread(Lizard *aLizard) {
    while (running) {
        aLizard->sleepNow();
        aLizard->sago2MonkeyGrassIsSafe();
        aLizard->crossSago2MonkeyGrass();
        aLizard->madeIt2MonkeyGrass();
        aLizard->eat();
        aLizard->monkeyGrass2SagoIsSafe();
        aLizard->crossMonkeyGrass2Sago();
        aLizard->madeIt2Sago();
    }
}

int main() {
    vector<Lizard*> lizards;
    vector<Cat*> cats;

    // Initialize semaphores
    sem_init(&sagoToMonkeyGrassSem, 0, MAX_LIZARD_CROSSING);
    sem_init(&monkeyGrassToSagoSem, 0, 0);
    sem_init(&catAlertSem, 0, 1);

    // Create cats and lizards
    for (int i = 0; i < NUM_CATS; i++) {
        cats.push_back(new Cat(i));
        cats[i]->run();
    }

    for (int i = 0; i < NUM_LIZARDS; i++) {
        lizards.push_back(new Lizard(i));
        lizards[i]->run();
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_LIZARDS; i++) {
        lizards[i]->wait();
    }

    for (int i = 0; i < NUM_CATS; i++) {
        cats[i]->wait();
    }

    // Clean up semaphores
    sem_destroy(&sagoToMonkeyGrassSem);
    sem_destroy(&monkeyGrassToSagoSem);
    sem_destroy(&catAlertSem);

    return 0;
}
