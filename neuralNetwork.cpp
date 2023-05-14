#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "configReader.h"
#include "layers.h"

using namespace std;

// Global Config
NeuralNetworkConfig config("config.txt");


int createNewFifo( const char *fifoName )
{
    struct stat stats;
    if ( stat( fifoName, &stats ) < 0 )
    {
        if ( errno != ENOENT )          // ENOENT is ok, since we intend to delete the file anyways
        {
            perror( "stat failed" );    // any other error is a problem
            return( -1 );
        }
    }
    else                                // stat succeeded, so the file exists
    {
        if ( unlink( fifoName ) < 0 )   // attempt to delete the file
        {
            perror( "unlink failed" );  // the most likely error is EBUSY, indicating that some other process is using the file
            return( -1 );
        }
    }

    if ( mkfifo( fifoName, 0666 ) < 0 ) // attempt to create a brand new FIFO
    {
        perror( "mkfifo failed" );
        return( -1 );
    }

    return( 0 );
}



int main() {

    const char* fifoName = "myfifo";

    // Create a named semaphore
    sem_t* layerSem = sem_open("/layer_sem", O_CREAT | O_EXCL, 0644, 0);
    if (layerSem == SEM_FAILED) {
        sem_unlink("/layer_sem");
        layerSem = sem_open("/layer_sem", O_CREAT | O_EXCL, 0644, 0);
    }

    if (layerSem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }


    // Create a named pipe
    if (createNewFifo(fifoName) == -1) {
        perror("createNewFifo");
        return 1;
    }

    // Open the FIFO
    int fifoFD = open(fifoName, O_RDWR | O_NONBLOCK);
 

    for (int i = 0; i < config.NHiddenLayers(); ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            cout << "Child process " << i << " created" << endl;

            // Create a layer object
            Layer* l = new Layer();

            // Set up the layer
            l->setUpLayer(config.hiddenLayerWeights[i],fifoFD);


            if (i == 0) {
                // for the first layer, read from the config file
                l->inputs= config.inputData;



            } else {
                // Read from the FIFO (non-blocking)
              l->readFromPipe();
            }





            // Run the layer
            l->printInputs();




           
            l->runNeuronThreads();


            l->printOutputs();



            // save the outputs of the layer
            l->writeToPipe();





            // Simulate work     done by neuron threads
            sleep(3);

            // Signal completion
            sem_post(layerSem); // Signal parent that layer is finished

            exit(0);
        } else if (pid > 0) {
            // Parent process
            cout << "Parent process: Waiting for child process " << i << " to finish" << endl;

            // Wait for child layer to finish before starting the next layer
            sem_wait(layerSem);

            cout << "Parent process: Child process " << i << " finished" << endl;
        }
    }

    // Close and unlink the named semaphore
    sem_close(layerSem);
    sem_unlink("/layer_sem");



    // Close and unlink the FIFO
    unlink(fifoName);

    return 0;
}
