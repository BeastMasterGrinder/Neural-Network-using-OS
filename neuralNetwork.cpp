// Nueral Network Implementation

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "configReader.h"


using namespace std;


class Layer
{
    



};


//Global Config
NeuralNetworkConfig config("config.txt");

int main()
{
  
    // NeuralNetwork nn(config.inputLayerWeights, config.hiddenLayerWeights, config.outputLayerWeights);

    config.print();


    //making a process for each layer

    for(int i = 0; i < config.NHiddenLayers();i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            //child process
            cout << "Child process " << i << " created" << endl;
            







            exit(0);
        }
        else if(pid > 0)
        {
            //parent process


            
        }
   
    }


 

    return 0;
}