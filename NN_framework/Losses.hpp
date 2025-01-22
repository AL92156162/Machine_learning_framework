#pragma once

#include <vector>
#include <cmath>

class Losses {

public:

	Losses() {
		// Do nothing
	}

	template <typename T>
	T meanSquareError(std::vector<T> y_true, std::vector<T> y_pred) {
		T loss = 0;
		for (int i = 0; i < y_true.size(); ++i) {
			loss += pow(y_true[i], y_pred[i]);
		}
		return loss / y_true.size();
	}

	template <typename T>
	T meanAbsoluteError(std::vector<T> y_true, std::vector<T> y_pred) {
		T loss = 0;
		for (int i = 0; i < y_true.size(); ++i) {
			loss += std::abs(y_true[i], y_pred[i]);
		}
		return loss / y_true.size();
	}

	template <typename T>
	T binaryCrossEntropy(std::vector<bool> y_true, std::vector<T> y_pred) {
		
		double loss = 0;

		for (int i = 0; i < y_true.size(); ++i) {
			loss += (y_true[i] * log(y_pred[i])) + ((1-y_true[i]) * log(1-y_pred[i]));
		}

		return - loss / y_true.size();
	}

	template <typename T>
	T categoricalCrossEntropy(std::vector<bool> y_true, std::vector<T> y_pred) {

		double loss = 0;

		for (int i = 0; i < y_true.size(); ++i) {
			loss -= y_true[i] * log(y_pred[i]);
		}

		return loss;
	}


};