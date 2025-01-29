//#include "MLP.hpp"

//Layer::Layer(const uint32_t size,
//	const char type,
//	std::function<float(const float&)> activation_fun,
//	std::function<float(const float&)> derive_activation_fun) {
//
//	m_size = size;
//	m_type = type;
//	m_activation_fun = activation_fun;
//	m_derive_activation_fun = derive_activation_fun;
//
//}
//
//void Layer::initialize(std::pair<int,int> shape) {
//	Matrix<float> m_weights = randomMatrix(shape.first, shape.second);
//	Matrix<float> m_bias = randomMatrix(1, shape.second);
//	Matrix<float> m_values = zeroMatrix(1, shape.second);
//}
//
//Matrix<float> Layer::feedForward(Matrix<float> input) {
//
//	m_values = m_weights.multiply(input);
//	m_values = m_values.add(m_bias);
//	m_values = m_values.applyFunction(m_activation_fun);
//	return m_values;
//}
