#ifndef ANN_H
#define ANN_H

#include <vector>
#include <string>

class ANN {
private:
    std::vector<std::vector<float> > weights;
    std::vector<std::vector<float> > biases;
    std::vector<std::vector<float> > m_weights;
    std::vector<std::vector<float> > v_weights;
    std::vector<std::vector<float> > m_biases;
    std::vector<std::vector<float> > v_biases;

    static const int SIDE = 3; // Changed from constexpr for simplicity
    static const int inputSize = SIDE * SIDE;
    static const int outputSize = SIDE * SIDE;
    const std::vector<int> architecture;
    const std::string weightsFile;
    float learningRate;
    float beta1, beta2;
    float epsilon;
    int timestep;

public:
    ANN();
    std::vector<float> predict(const std::vector<float>& input);
    void trainBatch(const std::vector<std::vector<float> >& inputs, const std::vector<int>& targets);
    void saveWeights() const;

private:
    std::vector<float> forwardLayer(const std::vector<float>& input, const std::vector<float>& layerWeights,
                                    const std::vector<float>& layerBiases, bool isLastLayer);
    std::pair<std::vector<std::vector<float> >, std::vector<std::vector<float> > > backpropagation(const std::vector<float>& input, int target);
    void applyAdamUpdate(int layerIdx, const std::vector<float>& gradWeights, const std::vector<float>& gradBiases);
    float relu(float x) const;
    float reluDerivative(float x) const;
    std::vector<float> softmax(const std::vector<float>& z) const;
    void loadWeights();
    void addVectors(std::vector<float>& a, const std::vector<float>& b);
    void scaleVector(std::vector<float>& v, float scalar);
};

#endif // ANN_H