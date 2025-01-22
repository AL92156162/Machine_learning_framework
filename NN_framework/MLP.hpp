#pragma once
#include "Matrix.hpp"
#include "Losses.hpp"
#include "Utility.hpp"
#include "Activation_functions.hpp"
#include <functional>

float sigmoid(float x)
{
	return 1.0f / (1 + exp(-x));
}

float dSigmoid(float x)
{
	return (x * (1 - x));
}

class MLP {
	std::vector<unsigned int> m_topology;
	float m_learning_rate;
	float m_momentum;

	std::function<float(const float&)> m_activation_fun;
	std::function<float(const float&)> m_derive_activation_fun;

	std::vector<Matrix<float>> m_weight_matrices;
	std::vector<Matrix<float>> m_value_matrices;
	std::vector<Matrix<float>> m_biais_matrices;

	std::vector<Matrix<float>> m_weight_velocity_matrices;
	std::vector<Matrix<float>> m_biais_velocity_matrices;

	Losses losses();

public:

	MLP(std::vector<unsigned int> topology, float learning_rate, float momentum, std::function<float(const float&)> activation_fun, std::function<float(const float&)> derive_activation_fun) {
		m_topology = topology;
		m_learning_rate = learning_rate;
		m_momentum = momentum;
		m_activation_fun = activation_fun;
		m_derive_activation_fun = derive_activation_fun;

		for (int i = 0; i < topology.size()-1; ++i) {
			Matrix<float> weight_mat = randomMatrix(topology[i], topology[i + 1]);
			m_weight_matrices.push_back(weight_mat);

			Matrix<float> weight_velocity_mat(topology[i], topology[i + 1]);
			m_weight_velocity_matrices.push_back(weight_velocity_mat);

			Matrix<float> biais_mat = randomMatrix(1, topology[i + 1]);
			m_biais_matrices.push_back(biais_mat);

			Matrix<float> biais_velocity_mat(1, topology[i + 1]);
			m_biais_velocity_matrices.push_back(biais_velocity_mat);

		}

		m_value_matrices.resize(topology.size());
	}

	
	std::vector<float> feedForward(std::vector<float> input) {

		if (input.size() != m_topology[0]) {
			throw std::logic_error("Input shape does not fit the MLP topology");
		}

		Matrix<float> value_mat(1, input.size(), input);

		for (int i = 0; i < m_topology.size()-1; ++i) {
			m_value_matrices[i] = value_mat;
			value_mat = m_weight_matrices[i].multiply(value_mat);
			value_mat = value_mat.add(m_biais_matrices[i]);
			value_mat = value_mat.applyFunction(m_activation_fun);
		}

		m_value_matrices[m_value_matrices.size()-1] = value_mat;
		return value_mat.vec();
	}

	void backProgagate(std::vector<float> input, std::vector<float> output) {
		
		if (input.size() != m_topology[0] || output.size() != m_topology.back()) {
			throw std::logic_error("Input or output shape does not fit the MLP topology");
		}

		std::vector<float> pred = feedForward(input);
		Matrix errors(1, output.size(), output);
		errors = errors - m_value_matrices.back();


		for (int i = m_weight_matrices.size() - 1; i >= 0; --i) {
			Matrix previous_errors = m_weight_matrices[i].transpose().multiply(errors);

			// Compute delta for biais and weights
			Matrix out_mat = m_value_matrices[i+1].applyFunction(m_derive_activation_fun);
			Matrix gradient = errors.multiply_elementwise(out_mat);
			gradient = gradient.multiply(m_learning_rate);

			Matrix val_t = m_value_matrices[i].transpose();
			Matrix weight_gradient = gradient.multiply(val_t);

			Matrix biais_velocity = m_biais_velocity_matrices[i].multiply(m_momentum) + gradient;
			Matrix weight_velocity = m_weight_velocity_matrices[i].multiply(m_momentum) + weight_gradient;

			// Adjust values
			m_biais_matrices[i] = m_biais_matrices[i] + biais_velocity;
			m_weight_matrices[i] = m_weight_matrices[i] + weight_velocity;

			errors = previous_errors;
			m_biais_velocity_matrices[i] = biais_velocity;
			m_weight_velocity_matrices[i] = weight_velocity;
		}
	}

	float error(std::vector<float> input, std::vector<float> output) {
		std::vector<float> pred = feedForward(input);
		float error = 0.0f;
		for (int i = 0; i < pred.size(); ++i) {
			error += pow(output[i] - pred[i],2);
		}
		return error;
	}

	Matrix<float> predict(Matrix<float> input) {
		input = input.transpose();
		std::vector<float> outvec = {};
		for (int i = 0; i < input.height(); ++i) {
			std::vector<float> invec = input.vec();
			invec = slicing(invec, i * input.width(), (i+1)*input.width());

			std::vector<float> out = feedForward(invec);
			outvec.insert(outvec.end(), out.begin(), out.end());
		}
		Matrix output(input.height(), outvec.size()/input.height(), outvec);
		return output;

	}

	std::vector<float> fit(Matrix<float> input, Matrix<float> output, int epochs, bool verbose = false) {
		int n_sample = input.width();
		std::vector<float> errors(epochs);

		for (int i = 0; i < epochs; i++)
		{
			std::vector<Matrix<float>> training_vec = { input , output };
			shuffle(training_vec);
			input = training_vec[0];
			output = training_vec[1];

			std::vector<float> epoch_errors(n_sample);
			for (int j = 0; j < input.width(); ++j) {

				std::vector<float> invec = input.slice(j, j + 1, 0).vec();
				std::vector<float> outvec = output.slice(j, j + 1, 0).vec();

				backProgagate(invec, outvec);
				epoch_errors.push_back(error(invec, outvec));
				std::cout << j << std::endl;
			}
			errors[i] = average(epoch_errors);
			if (verbose) {
				std::cout << "=======================================================" << std::endl;
				std::cout << "EPOCH : " << i << " ended" << std::endl;
				std::cout << "Error = " << errors[i] << std::endl;
				std::cout << "=======================================================" << std::endl;
			}
		}
		return errors;
	}

};