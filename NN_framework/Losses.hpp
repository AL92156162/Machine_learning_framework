#pragma once

#include <vector>
#include <cmath>
#include <string>
#include "Utility.hpp"

class Losses {

	uint16_t m_selector = 0;

public:

	Losses() {
		// do nothing
	}

	Losses(std::string const selected_loss) {
		select_loss(selected_loss);
	}

	void select_loss(std::string const selected_loss) {
		if (selected_loss == "MSE") {
			m_selector = 1;
		}
		else if (selected_loss == "CCE") {
			m_selector = 2;
		}
		else {
			throw std::logic_error("Unknown selected loss");
		}
	}

	uint16_t selected_loss() {
		return m_selector;
	}

	template <typename T>
	Matrix<T> operator()(Matrix<T> y_true, Matrix<T> y_pred, bool reduction = true) {
		Matrix<T> res;
		if (m_selector == 1) {
			res = meanSquareError(y_true, y_pred);
		}
		else if (m_selector == 2) {
			res = categoricalCrossEntropy(y_true, y_pred);
		}

		if (reduction){
			T new_loss = average(res.vec());
			std::vector<T> new_vec = { new_loss };
			Matrix<T> new_res(1, 1, new_vec);
			res = new_res;
		}

		return res;
	}


	template <typename T>
	Matrix<T> meanSquareError(Matrix<T> y_true, Matrix<T> y_pred) {
		std::vector<T> loss_vec(y_true.width());
		for (int i = 0; i < y_true.width(); ++i) {
			T loss = 0;
			for (int j = 0; j < y_true.height(); ++j) {
				loss += pow(y_true.getValue(i,j) - y_pred.getValue(i,j), 2);
			}
			loss_vec[i] = loss / y_true.height();
		}
		Matrix<T> res(y_true.width(), 1, loss_vec);
		return res;
	}

	template <typename T>
	Matrix<T> meanSquareErrorDerivative(Matrix<T> y_true, Matrix<T> y_pred) {
		Matrix<T> res = y_true - y_pred;
		return res;
	}

	template <typename T>
	Matrix<T> categoricalCrossEntropy(Matrix<T> y_true, Matrix<T> y_pred) {

		std::vector<T> loss_vec(y_true.width());
		for (int i = 0; i < y_true.width(); ++i) {
			T loss = 0;
			for (int j = 0; j < y_true.height(); ++j) {
				loss -= y_true.getValue(i, j) * log(y_pred.getValue(i, j));

			}
			loss_vec[i] = loss;
		}
		Matrix<T> res(y_true.width(), 1, loss_vec);
		return res;
	}

	template <typename T>
	Matrix<T> categoricalCrossEntropyDerivative(Matrix<T> y_true, Matrix<T> y_pred) {

		// CCE derivative if softmax or sigmoid activation function
		Matrix<T> res = y_pred - y_true;
		return res;
	}
};