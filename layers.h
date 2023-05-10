#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
//time
#include <ctime>

using namespace std;

class Neurons {
public:
    vector<double> weights;
};

class Layer {
public:
    vector<Neurons> neurons;
    static vector<sem_t*> sems;  // Define the static member variable

    void setUpLayer(const vector<vector<double>>& LayerWeights) {
        for (const auto& weights : LayerWeights) {
            Neurons n;
            n.weights = weights;
            neurons.push_back(n);
        }
    }

    void printLayer() {
        cout << "Printing Layer" << endl;
        for (const auto& neuron : neurons) {
            for (const auto& weight : neuron.weights) {
                cout << weight << " ";
            }
            cout << endl;
        }
    }

    void runNeuronThreads() {
        pthread_t threads[neurons.size()];

        // Create a semaphore for each neuron
        for (int i = 0; i < neurons.size(); i++) {
            sem_t* sem = new sem_t;
            sem_init(sem, 0, 0);
            sems.push_back(sem);
        }

        // Create a thread for each neuron
        for (int i = 0; i < neurons.size(); i++) {
            int* index = new int(i);
            pthread_create(&threads[i], nullptr, &Layer::NeuronThreadWrapper, reinterpret_cast<void*>(index));
        }

        // Wait for all threads to complete
        for (int i = 0; i < neurons.size(); i++) {
            sem_wait(sems[i]);
        }

        // Clean up
        for (int i = 0; i < neurons.size(); i++) {
            sem_destroy(sems[i]);
            delete sems[i];
        }
    }

    // Static wrapper function for thread
    static void* NeuronThreadWrapper(void* arg) {
        Layer* layer = reinterpret_cast<Layer*>(arg);
        

        // Get the index of the neuron
        int index = *reinterpret_cast<int*>(arg);
        // delete reinterpret_cast<int*>(arg);

        // Call the member function
        layer->NeuronThread(index);
        return nullptr;
    }

    // Member function for thread
    void NeuronThread(int index) {
        // Do some work...
        cout << "Neuron " << index << " is working..." << endl;
        sleep(1);
        cout.flush();

        // Signal completion by incrementing the semaphore
        sem_post(sems[index]);
    }
};

// Define the static member variable outside of the class definition
vector<sem_t*> Layer::sems;
