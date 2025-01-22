#pragma once
#include <cmath>



class Sigmoid {

	float operator()(float x)
	{
		return 1.0f / (1 + exp(-x));
	}

	float derive(float x)
	{
		return (x * (1 - x));
	}
};


class Relu {

	float operator()(float x)
	{
		float res = 0;
		if (x > 0) {
			res = x;
		}
		return res;
	}

	float derive(float x)
	{
		return x>0;
	}
};