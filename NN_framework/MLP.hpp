#pragma once
#include "Matrix.hpp"
#include "Losses.hpp"
#include "Utility.hpp"
#include "Activation_functions.hpp"
#include <functional>
#include <string>

//class Layer {
//	
//	uint32_t m_size;
//	char m_type;
//	Matrix<float> m_weights;
//	Matrix<float> m_values;
//	Matrix<float> m_bias;
//
//	std::function<float(const float&)> m_activation_fun;
//	std::function<float(const float&)> m_derive_activation_fun;
//
//public:
//
//	Layer(const uint32_t size,
//		const char type,
//		std::function<float(const float&)> activation_fun,
//		std::function<float(const float&)> derive_activation_fun);
//
//	std::pair<int, int> shape() const {
//		std::pair<int, int> shape = { m_weights.width(), m_weights.height() };
//		return shape; 
//	}
//	const uint32_t size() const { return m_size; }
//	const char type() const { return m_type; }
//	const Matrix<float> weights() const { return m_weights; }
//	const Matrix<float> bias() const { return m_bias; }
//
//	void initialize(std::pair<int, int> shape);
//	Matrix<float> feedForward(Matrix<float> input);
//
//};

class MLP {
	std::vector<unsigned int> m_topology;
	float m_learning_rate;
	float m_momentum;

	std::function<float(const float&)> m_activation_fun;
	std::function<float(const float&)> m_derive_activation_fun;

	std::vector<Matrix<float>> m_weight_matrices;
	std::vector<Matrix<float>> m_value_matrices;
	std::vector<Matrix<float>> m_biais_matrices;

	std::vector<Matrix<float>> m_weight_delta_matrices;
	std::vector<Matrix<float>> m_biais_delta_matrices;

	Losses m_losses;

public:

	MLP(std::vector<unsigned int> topology, 
		float learning_rate, 
		float momentum, 
		std::function<float(const float&)> activation_fun, 
		std::function<float(const float&)> derive_activation_fun) {

		m_topology = topology;
		m_learning_rate = learning_rate;
		m_momentum = momentum;
		m_activation_fun = activation_fun;
		m_derive_activation_fun = derive_activation_fun;
		m_losses.select_loss("MSE");

		for (int i = 0; i < topology.size()-1; ++i) {
			Matrix<float> weight_mat = randomMatrix(topology[i], topology[i + 1]);
			m_weight_matrices.push_back(weight_mat);

			Matrix<float> weight_velocity_mat(topology[i], topology[i + 1]);
			m_weight_delta_matrices.push_back(weight_velocity_mat);

			Matrix<float> biais_mat = randomMatrix(1, topology[i + 1]);
			m_biais_matrices.push_back(biais_mat);

			Matrix<float> biais_velocity_mat(1, topology[i + 1]);
			m_biais_delta_matrices.push_back(biais_velocity_mat);
		}

		m_value_matrices.resize(topology.size());
	}

	std::vector<Matrix<float>> getWeights() {
		return m_weight_matrices;
	}

	std::vector<Matrix<float>> getBiais() {
		return m_biais_matrices;
	}

	std::vector<Matrix<float>> getValues() {
		return m_value_matrices;
	}

	std::vector<Matrix<float>> getWeightsDelta() {
		return m_weight_delta_matrices;
	}

	std::vector<Matrix<float>> getBiaisDelta() {
		return m_biais_delta_matrices;
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
		Matrix<float> errors(1, output.size(), output);
		errors = errors - m_value_matrices.back();

		for (int i = m_weight_matrices.size() - 1; i >= 0; --i) {
			Matrix previous_errors = m_weight_matrices[i].transpose().multiply(errors);

			// Compute delta for biais and weights
			Matrix out_mat = m_value_matrices[i+1].applyFunction(m_derive_activation_fun);
			//Matrix out_mat = m_value_matrices[i + 1];
			Matrix gradient = errors.multiply_elementwise(out_mat);
			//gradient = gradient.multiply(m_learning_rate);

			Matrix val_t = m_value_matrices[i].transpose();
			Matrix weight_gradient = gradient.multiply(val_t);

			//Matrix biais_velocity = m_biais_delta_matrices[i].multiply(m_momentum) + gradient;
			//Matrix weight_velocity = m_weight_delta_matrices[i].multiply(m_momentum) + weight_gradient;
			

			errors = previous_errors;
			//m_biais_delta_matrices[i] = biais_velocity;
			//m_weight_delta_matrices[i] = weight_velocity;
			m_biais_delta_matrices[i] = gradient;
			m_weight_delta_matrices[i] = weight_gradient;

			m_biais_matrices[i] = m_biais_matrices[i] + m_biais_delta_matrices[i].multiply(m_learning_rate);
			m_weight_matrices[i] = m_weight_matrices[i] + m_weight_delta_matrices[i].multiply(m_learning_rate);
		}
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
		Matrix<float> output(input.height(), outvec.size()/input.height(), outvec);
		return output;
	}

	std::vector<float> fit(Matrix<float> input, Matrix<float> output, int epochs, bool verbose = false) {
		int n_sample = input.width();
		std::vector<float> loss_values(epochs);

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

				// Adjust values
				//for (int i = m_weight_matrices.size() - 1; i >= 0; --i) {
				//	m_biais_matrices[i] = m_biais_matrices[i] + m_biais_delta_matrices[i].multiply(m_learning_rate);
				//	m_weight_matrices[i] = m_weight_matrices[i] + m_weight_delta_matrices[i].multiply(m_learning_rate);
				//}
			}

			Matrix<float> pred = predict(input);
			Matrix<float> loss = m_losses(output, pred, true);

			loss_values[i] = loss.getValue(0, 0);

			if (verbose) {
				std::cout << "=======================================================" << std::endl;
				std::cout << "EPOCH : " << i << std::endl;
				std::cout << "Loss = " << loss.getValue(0, 0) << std::endl;
				std::cout << "=======================================================" << std::endl;
			}
		}
		return loss_values;
	}

};