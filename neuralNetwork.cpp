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




struct ThreadData {
    vector<double> inputs;
    vector<double> weights;
    double output;
};



class NeuralNetwork {

    public:
        vector<vector<double>> inputLayerWeights_;
        vector<vector<double>> hiddenLayerWeights_;
        vector<double> outputLayerWeights_;

        

        NeuralNetwork(const vector<vector<double>>& inputLayerWeights,
                  const vector<vector<double>>& hiddenLayerWeights,
                  const vector<vector<double>>& outputLayerWeights)
        : inputLayerWeights_(inputLayerWeights),
          hiddenLayerWeights_(hiddenLayerWeights),
          outputLayerWeights_(outputLayerWeights[0]) {}

        double feedForward(const vector<double>& inputs)
        {
            vector<vector<double>> prevLayerOutputs;
            vector<vector<double>> layerOutputs;

            // Feed-forward through the input layer
            prevLayerOutputs.push_back(inputs);
            layerOutputs = feedForwardLayer(prevLayerOutputs, inputLayerWeights_[0]);

            // Feed-forward through the hidden layers
            for (const auto& layerWeights : hiddenLayerWeights_) {
                prevLayerOutputs = layerOutputs;
                layerOutputs = feedForwardLayer(prevLayerOutputs, layerWeights);
            }

            // Feed-forward through the output layer
            prevLayerOutputs = layerOutputs;
            layerOutputs = feedForwardLayer(prevLayerOutputs, outputLayerWeights_);

            return layerOutputs[0][0];  // Assuming there is only one output neuron in the output layer
        }

        vector<vector<double>> feedForwardLayer(const vector<vector<double>>& inputs, const vector<double>& weights)
        {
            vector<vector<double>> outputs;
            vector<pthread_t> threads(weights.size());
            vector<ThreadData> threadData(weights.size());

            for (size_t i = 0; i < weights.size(); ++i) {
                threadData[i].inputs = inputs[0];  // Assuming all neurons in a layer have the same inputs
                threadData[i].weights = weights;

                int result = pthread_create(&threads[i], NULL, neuronThread, &threadData[i]);
                if (result != 0) {
                    cerr << "Error creating thread. Exiting...\n";
                    exit(1);
                }
            }

            for (size_t i = 0; i < threads.size(); ++i) {
                pthread_join(threads[i], NULL);
                outputs.push_back({threadData[i].output});
            }

            return outputs;
        }

        static void * neuronThread(void* threadData)
        {
            ThreadData* data = (ThreadData*)threadData;
            double sum = 0.0;

            for (size_t i = 0; i < data->inputs.size(); ++i)
                sum += data->inputs[i] * data->weights[i];

            data->output = 1.0 / (1.0 + exp(-sum));

            return NULL;
        }



};


int main()
{
    NeuralNetworkConfig config("config.txt");
    NeuralNetwork nn(config.inputLayerWeights, config.hiddenLayerWeights, config.outputLayerWeights);

    config.print();

    cout << "Input data: ";
    for (const auto& input : config.inputData)
        cout << input << " ";
    cout << endl;

    cout << "Output: " << nn.feedForward(config.inputData) << endl;

    return 0;
}