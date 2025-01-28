#pragma once
#include <cmath>
#include <math.h> 

//SIGMOID
float sigmoid(float x)
{
	return 1.0f / (1 + exp(-x));
}
float dSigmoid(float x)
{
	return (x * (1 - x));
}

//RELU
float relu(float x)
{
	float res = 0;
	if (x > 0) {
		res = x;
	}
	return res;
}
float dRelu(float x)
{
	return x > 0;
}

//TANH
//float tanh(float x)
//{
//	return std::tanh(x);
//}
float dTanh(float x)
{
	return 1.0 - (std::tanh(x) * std::tanh(x));
}