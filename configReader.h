#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct NeuralNetworkConfig {
    std::vector<std::vector<double>> inputLayerWeights;
    std::vector<std::vector<std::vector<double>>> hiddenLayerWeights;
    std::vector<std::vector<double>> outputLayerWeights;
    std::vector<double> inputData;
    
    
    // Constructor
    NeuralNetworkConfig(const std::string& filename) {
        read(filename);
    }
    
    void read(const std::string& filename);
    void print();
    int NHiddenLayers();
    int NNeuronsInLayer()
    {
        return inputLayerWeights[0].size();
    }
};

void NeuralNetworkConfig::read(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    enum class Section { IL, HL, OL, ID, None };
    Section currentSection = Section::None;
    int hiddenLayerCount = 0;

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        if (line == "IL")
            currentSection = Section::IL;
        else if (line == "OL")
            currentSection = Section::OL;
        else if (line == "ID")
            currentSection = Section::ID;
        else if (line.substr(0, 2) == "HL") {
            currentSection = Section::HL;
            hiddenLayerCount++;
            hiddenLayerWeights.emplace_back();  // Add a new hidden layer
            continue;
        }

        std::istringstream iss(line);
        std::vector<double> values;
        double value;

        if (currentSection == Section::ID) {
            while (iss >> value) {
                values.push_back(value);
                if (iss.peek() == ',')
                    iss.ignore();
            }

            inputData = values;
        } else {
            while (iss >> value) {
                values.push_back(value);
                if (iss.peek() == ',')
                    iss.ignore();
            }

            switch (currentSection) {
                case Section::IL:
                    inputLayerWeights.push_back(values);
                    break;
                case Section::HL:
                    hiddenLayerWeights[hiddenLayerCount - 1].push_back(values);
                    break;
                case Section::OL:
                    outputLayerWeights.push_back(values);
                    break;
                default:
                    break;
            }
        }
    }


    //remove the first empy vector in inputLayerWeights
    inputLayerWeights.erase(inputLayerWeights.begin());

}

void NeuralNetworkConfig::print() {
    std::cout << "Input Layer Weights:\n";
    for (const auto& layer : inputLayerWeights) {
        for (const auto& weight : layer) {
            std::cout << weight << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nHidden Layer Weights:\n";
    for (const auto& layer : hiddenLayerWeights) {
        for (const auto& neuron : layer) {
            for (const auto& weight : neuron) {
                std::cout << weight << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "\nOutput Layer Weights:\n";
    for (const auto& layer : outputLayerWeights) {
        for (const auto& weight : layer) {
            std::cout << weight << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nInput Data:\n";
    for (const auto& data : inputData) {
        std::cout << data << " ";
    }
    std::cout << "\n";
}

int NeuralNetworkConfig::NHiddenLayers() {
    return hiddenLayerWeights.size();
}


   
