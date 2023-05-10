// Nueral Network Implementation

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "configReader.h"
#include "layers.h"


using namespace std;



//Global Config
NeuralNetworkConfig config("config.txt");

int main()
{
  
    // NeuralNetwork nn(config.inputLayerWeights, config.hiddenLayerWeights, config.outputLayerWeights);

    // config.print();


    //semaphore a layer
    sem_t layerSem;
    sem_init(&layerSem, 0, 0);

    //making a process for each layer

    for(int i = 0; i < config.NHiddenLayers();i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            //child process
            cout << "Child process " << i << " created" << endl;
            // sleep(1);
            cout.flush();

            //create a layer object
            Layer l;

            //set up the layer
            l.setUpLayer(config.hiddenLayerWeights[i]);


            //print the layer

            // l.printLayer();

            l.runNeuronThreads();

            

            sem_post(&layerSem);




            



            







            exit(0);
        }
        else if(pid > 0)
        {
            //parent process
            //wait for the child process to finish
            sem_wait(&layerSem);
            // cout << "Child process " << i << " finished" << endl;
            // cout.flush();



            
        }
   
    }


 

    return 0;
}