#ifndef __PREDENGINE_HPP__
#define __PREDENGINE_HPP__

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

namespace predengine
{

const double eta = 0.03;

double sigmoidf(double x)
{
    return (1 / (1 + exp((double)-x)));
}

class Neuron
{
public:
    unsigned int nr_of_weights;
    double *weights;
    double neuron_value;
    Neuron()
    {
        nr_of_weights = 0;
        weights = nullptr;
        neuron_value = 0;
    }
    Neuron(unsigned int nr_of_weights)
    {
        this->nr_of_weights = nr_of_weights;
        this->weights = (double *)malloc(sizeof(double) * (size_t)nr_of_weights);
        for (size_t i = 0; i < (size_t)nr_of_weights; i++)
        {
            this->weights[i] = rand() / (double)(RAND_MAX);
        }
        neuron_value = 0;
    }
    ~Neuron()
    {
        if (weights != nullptr)
            free(weights);
    }
};

class Layer
{
public:
    unsigned int nr_of_neurons;
    Neuron **neurons;
    Layer() {}
    ~Layer(){};
};

class InputLayer : public Layer
{
public:
    InputLayer(unsigned int nr_of_neurons, unsigned int nr_of_weights)
    {
        this->nr_of_neurons = nr_of_neurons;
        neurons = new Neuron *[nr_of_neurons];
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            neurons[i] = new Neuron(nr_of_weights);
        }
    }
    ~InputLayer()
    {
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            delete neurons[i];
        }
        delete neurons;
    }
};

class HiddenLayer : public Layer
{
public:
    double bias;
    double *bias_weigths;
    //double* target;
    HiddenLayer(unsigned int nr_of_neurons, unsigned int nr_of_weigths)
    {
        this->nr_of_neurons = nr_of_neurons;
        neurons = new Neuron *[nr_of_neurons];
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            neurons[i] = new Neuron(nr_of_weigths);
            neurons[i]->nr_of_weights = nr_of_weigths;
        }
        bias = 1.0;
        bias_weigths = (double *)malloc(sizeof(double) * nr_of_neurons);
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            bias_weigths[i] = rand() / (double)(RAND_MAX);
        }
        //target = (double*)calloc(nr_of_weigths, sizeof(double));
    }
    void forward_propagation(Neuron **previous, unsigned int nr_of_prev_neurons)
    {
        double tmp = 0.0;
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            tmp = 0.0;
            for (unsigned int j = 0; j < nr_of_prev_neurons; j++)
            {
                tmp += previous[j]->weights[i] * previous[j]->neuron_value;
            }
            tmp += bias_weigths[i] * bias;
            neurons[i]->neuron_value = sigmoidf(tmp);
        }
    }
    void back_propagation(Neuron **previous, unsigned int nr_of_prev_ner, double *dETotal_dout, double *error_from_here)
    {
        double tmp = 0;
        for (size_t i = 0; i < nr_of_neurons; i++)
        {
            for (size_t j = 0; j < nr_of_prev_ner; j++)
            {
                tmp = dETotal_dout[i] * neurons[i]->neuron_value * (1 - neurons[i]->neuron_value) * previous[j]->neuron_value;
                previous[j]->weights[i] = previous[j]->weights[i] - eta * tmp;
                error_from_here[j] += dETotal_dout[i] * neurons[i]->neuron_value * (1 - neurons[i]->neuron_value) * previous[j]->weights[i];
            }
            tmp = dETotal_dout[i] * neurons[i]->neuron_value * (1 - neurons[i]->neuron_value) * bias;
            bias_weigths[i] = bias_weigths[i] - eta * tmp;
        }
    }
    ~HiddenLayer()
    {
        for (size_t i = 0; i < nr_of_neurons; i++)
        {
            delete neurons[i];
        }
        delete neurons;
        free(bias_weigths);
        //free(target);
    }
};

class OutputLayer : public Layer
{
public:
    double bias;
    double *bias_weigths;
    double error_t;
    double *target;
    OutputLayer(unsigned int nr_of_neurons, unsigned int nr_of_weigths)
    {
        this->nr_of_neurons = nr_of_neurons;
        neurons = new Neuron *[nr_of_neurons];
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            neurons[i] = new Neuron();
        }
        bias = 1.0;
        bias_weigths = (double *)malloc(sizeof(double) * nr_of_weigths);
        for (unsigned int i = 0; i < nr_of_weigths; i++)
        {
            bias_weigths[i] = rand() / (double)(RAND_MAX);
        }
        error_t = 0.0;
        target = (double *)calloc(nr_of_neurons, sizeof(double));
    }
    void forward_propagation(Neuron **previous, unsigned int nr_of_prev_neuron)
    {
        double tmp = 0.0;
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            tmp = 0.0;
            for (unsigned int j = 0; j < nr_of_prev_neuron; j++)
            {
                tmp += previous[j]->weights[i] * previous[j]->neuron_value;
            }
            tmp += bias_weigths[i] * bias;
            neurons[i]->neuron_value = sigmoidf(tmp);
        }
    }
    int back_propagation(Neuron **previous, unsigned int nr_of_prev_ner, unsigned int target_position, double *dETotal_dout)
    {
        double error_f = 0.0;
        target[target_position] = 1.0;
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            error_f += pow(target[i] - neurons[i]->neuron_value, 2) / 2.0;
        }
        //if(error_f < 0.33) return 1;
        printf("Functia errorii pentru Ol: %.2f\n", error_f);
        double tmp;
        for (size_t i = 0; i < nr_of_neurons; i++)
        {
            for (size_t j = 0; j < nr_of_prev_ner; j++)
            {
                tmp = ((target[i] - neurons[i]->neuron_value) * -1.0) * neurons[i]->neuron_value * (1.0 - neurons[i]->neuron_value) * previous[j]->neuron_value;
                previous[j]->weights[i] = previous[j]->weights[i] - eta * tmp;
                dETotal_dout[j] += ((target[i] - neurons[i]->neuron_value) * -1.0) * neurons[i]->neuron_value * (1.0 - neurons[i]->neuron_value) * previous[j]->weights[i];
            }
            tmp = ((target[i] - neurons[i]->neuron_value) * -1.0) * neurons[i]->neuron_value * (1.0 - neurons[i]->neuron_value) * bias;
            bias_weigths[i] = bias_weigths[i] * eta * tmp;
        }
        target[target_position] = 0.0;
        return 0;
    }
    unsigned int get_result_position()
    {
        double max = 0.0;
        unsigned int result = 0;
        for (unsigned int i = 0; i < nr_of_neurons; i++)
        {
            if (neurons[i]->neuron_value > max)
            {
                max = neurons[i]->neuron_value;
                result = i;
            }
        }
        return result;
    }
    ~OutputLayer()
    {
        for (size_t i = 0; i < nr_of_neurons; i++)
        {
            delete neurons[i];
        }
        delete *neurons;
        free(target);
        free(bias_weigths);
    }
};

}; // namespace predengine

#endif // __PREDENGINE_HPP__