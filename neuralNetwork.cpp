#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "configReader.h"
#include "layers.h"

using namespace std;

//Global Config
NeuralNetworkConfig config("config.txt");


int main()
{
    // Create a named semaphore
    sem_t* layerSem = sem_open("/layer_sem", O_CREAT | O_EXCL, 0644, 0);

    //if the semaphore already exists, unlink and recreate
    if (layerSem == SEM_FAILED) {
        sem_unlink("/layer_sem");
        layerSem = sem_open("/layer_sem", O_CREAT | O_EXCL, 0644, 0);
    }



    if (layerSem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    for (int i = 0; i < config.NHiddenLayers(); ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            cout << "Child process " << i << " created" << endl;

            // Create a layer object
            Layer* l= new Layer();


            


            // Set up the layer
            l->setUpLayer(config.hiddenLayerWeights[i]);

            // Simulate neuron threads execution with sleep
            cout << "Child process " << i  << ": Neuron threads execution started" << endl;
            cout.flush();
            // sleep(3); // Simulate work done by neuron threads

            l->runNeuronThreads();

            sleep(3); // Simulate work done by neuron threads

            cout.flush();
            cout << "Child process " << i << ": Neuron threads execution completed" << endl;
            cout.flush();


            // Signal completion
            sem_post(layerSem); // Signal parent that layer is finished

            exit(0);
        } else if (pid > 0) {
            // Parent process
            // Wait for the child process to finish
            cout << "Parent process: Waiting for child process " << i << " to finish" << endl;
            sem_wait(layerSem); // Wait for child layer to finish before starting the next layer
            cout << "Parent process: Child process " << i << " finished" << endl;
        }
    }

    // Close and unlink the named semaphore
    sem_close(layerSem);
    sem_unlink("/layer_sem");

    //destroy the semaphore
    // sem_destroy(&layerSem);

    return 0;
}
