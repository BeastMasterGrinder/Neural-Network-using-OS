
class NeuralNetwork {
public:
  NeuralNetwork(const std::vector<std::vector<double>>& inputLayerWeights,
              const std::vector<std::vector<double>>& hiddenLayerWeights,
              const std::vector<std::vector<double>>& outputLayerWeights)
    : inputLayerWeights_(inputLayerWeights),
      hiddenLayerWeights_(hiddenLayerWeights),
      outputLayerWeights_(outputLayerWeights) {}


std::vector<double> feedForward(const std::vector<double>& inputData) {
    std::vector<std::vector<double>> prevLayerOutputs;
    std::vector<std::vector<double>> layerOutputs;

    // Feed-forward through the input layer
    prevLayerOutputs.push_back(inputData);
    layerOutputs = feedForwardLayer(prevLayerOutputs, inputLayerWeights_);

    // Feed-forward through the hidden layers
    for (const auto& layerWeights : hiddenLayerWeights_) {
        prevLayerOutputs = layerOutputs;
        layerOutputs = feedForwardLayer(prevLayerOutputs, layerWeights);
    }

    // Feed-forward through the output layer
    prevLayerOutputs = layerOutputs;
    layerOutputs = feedForwardLayer(prevLayerOutputs, outputLayerWeights_);

    return layerOutputs[0];  // Assuming there is only one output neuron in the output layer
}

std::vector<std::vector<double>> feedForwardLayer(const std::vector<std::vector<double>>& inputs,
                                                  const std::vector<double>& weights) {
    std::vector<std::vector<double>> outputs;
    std::vector<pthread_t> threads(weights.size());
    std::vector<ThreadData> threadData(weights.size());

    for (size_t i = 0; i < weights.size(); ++i) {
        threadData[i].inputs = inputs[0];  // Assuming all neurons in a layer have the same inputs
        threadData[i].weights = weights;

        int result = pthread_create(&threads[i], NULL, neuronThread, &threadData[i]);
        if (result != 0) {
            std::cerr << "Error creating thread. Exiting...\n";
            exit(1);
        }
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], NULL);
        outputs.push_back({threadData[i].output});
    }

    return outputs;
}


private:
    struct ThreadData {
        std::vector<double> inputs;
        std::vector<double> weights;
        double output;
    };

    static void* neuronThread(void* arg) {
        ThreadData* data = static_cast<ThreadData*>(arg);
        const std::vector<double>& inputs = data->inputs;
        const std::vector<double>& weights = data->weights;
        double& output = data->output;

        double sum = 0.0;
        for (size_t i = 0; i < inputs.size(); ++i) {
            sum += inputs[i] * weights[i];
        }
        output = sum;

        pthread_exit(NULL);
    }

    
    std::vector<std::vector<double>> inputLayerWeights_;
    std::vector<std::vector<double>> hiddenLayerWeights_;
    std::vector<std::vector<double>> outputLayerWeights_;
};

