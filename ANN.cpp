#include "ANN.h"
#include <random>
#include <fstream>
#include <cmath>

ANN::ANN() : architecture{inputSize, 16, 8, outputSize}, weightsFile("ann_weights.dat"), 
             learningRate(0.001f), beta1(0.9f), beta2(0.999f), epsilon(1e-8f), timestep(1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-0.1f, 0.1f);

    for (size_t i = 0; i < architecture.size() - 1; ++i) {
        int layerInputSize = architecture[i];
        int layerOutputSize = architecture[i + 1];

        weights.push_back(std::vector<float>(layerInputSize * layerOutputSize));
        for (float& weight : weights.back()) weight = dist(gen);
        biases.push_back(std::vector<float>(layerOutputSize, 0.0f));
        m_weights.push_back(std::vector<float>(layerInputSize * layerOutputSize, 0.0f));
        v_weights.push_back(std::vector<float>(layerInputSize * layerOutputSize, 0.0f));
        m_biases.push_back(std::vector<float>(layerOutputSize, 0.0f));
        v_biases.push_back(std::vector<float>(layerOutputSize, 0.0f));
    }
    loadWeights();
}

std::vector<float> ANN::predict(const std::vector<float>& input) {
    std::vector<float> activations = input;
    for (size_t layerIdx = 0; layerIdx < architecture.size() - 1; ++layerIdx) {
        activations = forwardLayer(activations, weights[layerIdx], biases[layerIdx], layerIdx == architecture.size() - 2);
    }
    return activations;
}

void ANN::trainBatch(const std::vector<std::vector<float> >& inputs, const std::vector<int>& targets) {
    std::vector<std::vector<float> > batchGradWeights(architecture.size() - 1);
    std::vector<std::vector<float> > batchGradBiases(architecture.size() - 1);

    for (size_t i = 0; i < inputs.size(); ++i) {
        std::pair<std::vector<std::vector<float> >, std::vector<std::vector<float> > > grads = backpropagation(inputs[i], targets[i]);
        std::vector<std::vector<float> >& gradWeights = grads.first;
        std::vector<std::vector<float> >& gradBiases = grads.second;
        for (size_t layerIdx = 0; layerIdx < architecture.size() - 1; ++layerIdx) {
            if (batchGradWeights[layerIdx].empty()) {
                batchGradWeights[layerIdx] = gradWeights[layerIdx];
                batchGradBiases[layerIdx] = gradBiases[layerIdx];
            } else {
                addVectors(batchGradWeights[layerIdx], gradWeights[layerIdx]);
                addVectors(batchGradBiases[layerIdx], gradBiases[layerIdx]);
            }
        }
    }

    for (size_t layerIdx = 0; layerIdx < architecture.size() - 1; ++layerIdx) {
        scaleVector(batchGradWeights[layerIdx], 1.0f / inputs.size());
        scaleVector(batchGradBiases[layerIdx], 1.0f / inputs.size());
        applyAdamUpdate(layerIdx, batchGradWeights[layerIdx], batchGradBiases[layerIdx]);
    }
    timestep++;
}

void ANN::saveWeights() const {
    std::ofstream outFile(weightsFile, std::ios::binary);
    if (outFile.is_open()) {
        for (const auto& layerWeights : weights) {
            for (const float& weight : layerWeights) {
                outFile.write(reinterpret_cast<const char*>(&weight), sizeof(weight));
            }
        }
        for (const auto& layerBiases : biases) {
            for (const float& bias : layerBiases) {
                outFile.write(reinterpret_cast<const char*>(&bias), sizeof(bias));
            }
        }
        outFile.close();
    }
}

std::vector<float> ANN::forwardLayer(const std::vector<float>& input, const std::vector<float>& layerWeights,
                                     const std::vector<float>& layerBiases, bool isLastLayer) {
    int inputSize = input.size();
    int outputSize = layerBiases.size();
    std::vector<float> z(outputSize, 0.0f);

    for (int i = 0; i < outputSize; ++i) {
        for (int j = 0; j < inputSize; ++j) {
            z[i] += input[j] * layerWeights[i * inputSize + j];
        }
        z[i] += layerBiases[i];
    }
    return isLastLayer ? softmax(z) : std::vector<float>(z.begin(), z.end());
}

std::pair<std::vector<std::vector<float> >, std::vector<std::vector<float> > > ANN::backpropagation(const std::vector<float>& input, int target) {
    std::vector<std::vector<float> > gradWeights(architecture.size() - 1);
    std::vector<std::vector<float> > gradBiases(architecture.size() - 1);
    std::vector<std::vector<float> > activations(architecture.size());
    std::vector<std::vector<float> > zs(architecture.size() - 1);

    activations[0] = input;
    for (size_t layerIdx = 0; layerIdx < architecture.size() - 1; ++layerIdx) {
        zs[layerIdx] = forwardLayer(activations[layerIdx], weights[layerIdx], biases[layerIdx], false);
        activations[layerIdx + 1] = (layerIdx == architecture.size() - 2) ? softmax(zs[layerIdx]) : zs[layerIdx];
    }

    std::vector<float> delta = activations.back();
    delta[target] -= 1.0f;

    for (int layerIdx = architecture.size() - 2; layerIdx >= 0; --layerIdx) {
        int inputSize = architecture[layerIdx];
        int outputSize = architecture[layerIdx + 1];
        gradWeights[layerIdx].resize(inputSize * outputSize);
        gradBiases[layerIdx].resize(outputSize);

        for (int i = 0; i < outputSize; ++i) {
            gradBiases[layerIdx][i] = delta[i];
            for (int j = 0; j < inputSize; ++j) {
                gradWeights[layerIdx][i * inputSize + j] = delta[i] * activations[layerIdx][j];
            }
        }

        if (layerIdx > 0) {
            std::vector<float> prevDelta(inputSize, 0.0f);
            for (int i = 0; i < inputSize; ++i) {
                for (int j = 0; j < outputSize; ++j) {
                    prevDelta[i] += delta[j] * weights[layerIdx][j * inputSize + i];
                }
                prevDelta[i] *= reluDerivative(zs[layerIdx - 1][i]);
            }
            delta = prevDelta;
        }
    }
    return {gradWeights, gradBiases};
}

void ANN::applyAdamUpdate(int layerIdx, const std::vector<float>& gradWeights, const std::vector<float>& gradBiases) {
    for (size_t i = 0; i < weights[layerIdx].size(); ++i) {
        m_weights[layerIdx][i] = beta1 * m_weights[layerIdx][i] + (1 - beta1) * gradWeights[i];
        v_weights[layerIdx][i] = beta2 * v_weights[layerIdx][i] + (1 - beta2) * gradWeights[i] * gradWeights[i];
        float mHat = m_weights[layerIdx][i] / (1 - std::pow(beta1, timestep));
        float vHat = v_weights[layerIdx][i] / (1 - std::pow(beta2, timestep));
        weights[layerIdx][i] -= learningRate * mHat / (std::sqrt(vHat) + epsilon);
    }

    for (size_t i = 0; i < biases[layerIdx].size(); ++i) {
        m_biases[layerIdx][i] = beta1 * m_biases[layerIdx][i] + (1 - beta1) * gradBiases[i];
        v_biases[layerIdx][i] = beta2 * v_biases[layerIdx][i] + (1 - beta2) * gradBiases[i] * gradBiases[i];
        float mHat = m_biases[layerIdx][i] / (1 - std::pow(beta1, timestep));
        float vHat = v_biases[layerIdx][i] / (1 - std::pow(beta2, timestep));
        biases[layerIdx][i] -= learningRate * mHat / (std::sqrt(vHat) + epsilon);
    }
}

float ANN::relu(float x) const { return std::max(0.0f, x); }

float ANN::reluDerivative(float x) const { return x > 0 ? 1.0f : 0.0f; }

std::vector<float> ANN::softmax(const std::vector<float>& z) const {
    std::vector<float> expZ(z.size());
    float sumExpZ = 0.0f;
    for (size_t i = 0; i < z.size(); ++i) {
        expZ[i] = std::exp(z[i]);
        sumExpZ += expZ[i];
    }
    for (float& val : expZ) val /= sumExpZ;
    return expZ;
}

void ANN::loadWeights() {
    std::ifstream inFile(weightsFile, std::ios::binary);
    if (inFile.is_open()) {
        for (auto& layerWeights : weights) {
            for (float& weight : layerWeights) {
                inFile.read(reinterpret_cast<char*>(&weight), sizeof(weight));
            }
        }
        for (auto& layerBiases : biases) {
            for (float& bias : layerBiases) {
                inFile.read(reinterpret_cast<char*>(&bias), sizeof(bias));
            }
        }
        inFile.close();
    }
}

void ANN::addVectors(std::vector<float>& a, const std::vector<float>& b) {
    for (size_t i = 0; i < a.size(); ++i) a[i] += b[i];
}

void ANN::scaleVector(std::vector<float>& v, float scalar) {
    for (float& val : v) val *= scalar;
}