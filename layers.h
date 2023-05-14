#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>


//time
#include <ctime>

using namespace std;

// Defining debug constants booleans
#define DEBUG 1
#define DEBUG_PIPE 0







class Neurons {
public:
    vector<double> weights;
};

class Layer {
public:
    vector<Neurons> neurons;
    // static vector<sem_t*> sems;  // Define the static member variable
    sem_t threadSem;

    //the inputs recieved from the previous layer
    vector<double> inputs;

    //the outputs of the layer
    vector<double> outputs;

    int fd;
   



    // Constructor

    Layer() {
       





    }

  
  void printInputs(){
      cout<<"Inputs: "<<endl;
      for(int i=0; i<inputs.size(); i++){
          cout<<inputs[i]<<" ";
      }
      cout<<endl;

            cout.flush();


  }

  void printOutputs(){
      cout<<"Outputs: "<<endl;
      for(int i=0; i<outputs.size(); i++){
          cout<<outputs[i]<<" ";
      }
      cout<<endl;
      cout.flush();
  }


    void writeToPipe() {

        //add dummy pid to the vector
        outputs.push_back(getpid());

        //first write the size of the vector
        int size=outputs.size();
        if (write(fd, &size, sizeof(size)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }




        // Write to the pipe
        if (write(fd, outputs.data(), outputs.size() * sizeof(double)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (DEBUG && DEBUG_PIPE)
        {
            cout<<"Wrote to pipe: "<<endl;
                    //print the output
                    for (int i = 0; i < outputs.size(); i++) {
                        cout << outputs[i] << " ";
            }

        cout.flush();




        }

        
    }

    void readFromPipe() {

        // Read from the pipe
        
        int size;
        if (read(fd, &size, sizeof(size)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }


        if (DEBUG && DEBUG_PIPE)
        {
            cout<<"Read from pipe: "<<endl;
            cout<<"Size: "<<size<<endl;
        }

  

        //resize the vector
        inputs.resize(size);

        if (read(fd, inputs.data(), inputs.size() * sizeof(double)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }


        if (DEBUG && DEBUG_PIPE)
        {
               //print the input
        for (int i = 0; i < inputs.size(); i++) {
            cout << inputs[i] << " ";
        }

        cout<<endl;
       

       cout.flush();
        }

     

        
    }


    // Set up the layer
    void setUpLayer(const vector<vector<double>>& LayerWeights, int fd) {
        
        //set up pipe
        this->fd=fd;

        for (const auto& weights : LayerWeights) {
            Neurons n;
            n.weights = weights;
            neurons.push_back(n);
        }
    }

    void printLayer() {
        cout << "Printing Layer " <<&neurons<< endl;
        for (const auto& neuron : neurons) {
            for (const auto& weight : neuron.weights) {
                cout << weight << " ";
            }
            cout << endl;
        }
    }

    ~Layer() {
        
        //delete neurons
        for (int i = 0; i < neurons.size(); i++) {
            delete &neurons[i];
        }

        //clear the vector
        neurons.clear();

        //unlink the pipe






    }



    void setInput() {
        
        //read from pipe
        //copy the input to the input vector
        
        



    }

    void runNeuronThreads() {
      
      
      
        pthread_t threads[neurons.size()];

        cout<<"Number of threads: "<<neurons.size()<<"for process id: "<<getpid()<<endl;
\

        printLayer();



        // // Create a semaphore for each neuron
        // for (int i = 0; i < neurons.size(); i++) {
        //     sem_t* sem = new sem_t;
        //     sem_init(sem, 0, 0);
        //     sems.push_back(sem);
        // }

        //coutnign semaphore for number of threads
        sem_init(&threadSem, 0,8);

        // Create a thread for each neuron
        for (int i = 0; i < neurons.size(); i++) {
            int* index = new int(i);
            pthread_create(&threads[i], nullptr, &Layer::NeuronThreadWrapper, reinterpret_cast<void*>(index));
        }




        //print semaphore value
        int* value= new int;
        sem_getvalue(&threadSem, value);
        // cout << "SEMSEMSEMSEM value: " << *value << endl;



        // Wait for all threads to complete

        //check if semaphore is 0
        while(*value != 0){
            sem_getvalue(&threadSem, value);
            // cout << "SEMSEMSEMSEM value: " << *value << endl;
            sleep(1);
        }       

 


        

        sem_destroy(&threadSem);

        //clean up threads and semaphores



    }

    // Static wrapper function for thread
    static void* NeuronThreadWrapper(void* arg) {
        Layer* layer = reinterpret_cast<Layer*>(arg);
        

        // Get the index of the neuron
        int index = *reinterpret_cast<int*>(arg);
        // delete reinterpret_cast<int*>(arg);

        // Call the member function
        layer->NeuronThread(index);



        //print semaphore value



  
        //end of thread
        pthread_exit(NULL);


        return nullptr;
    }

    // Member function for thread
    void NeuronThread(int index) {
        // // Do some work...
        // cout << "Neuron " << index << " is working..." << endl;
        //         cout.flush();

        // sleep(2);


        // //print semaphore value


        //based of index get the weights 
        vector<double> weights=neurons[index].weights;

        //multiply the weights with the inputs
        double sum=0;
        for(int i=0; i<weights.size(); i++){
            sum+=weights[i]*inputs[i];
        }


        //apply activation function
        double output;
        // output=1/(1+exp(-sum));
        output=sum;

        //add the output to the output vector at index 
        outputs[index]=output;

      

        //print the output
        cout<<"Output of neuron "<<index<<" is: "<<output<<endl;






        // Signal completion by incrementing the semaphore
        sem_trywait(&threadSem);
 


        
    }
};

// Define the static member variable outside of the class definition
// vector<sem_t*> Layer::sems;
