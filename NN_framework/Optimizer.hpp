#pragma once


class SGD {
	double m_learning_rate;
	double m_momentum;

public:
	SGD(double learning_rate, double momentum) {
		m_learning_rate = learning_rate;
		m_momentum = momentum;
	}

	
};