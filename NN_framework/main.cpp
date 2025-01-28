#include "matrix.hpp"
#include "MLP.hpp"
#include "Test_matrix.hpp"
#include "CSV_parser.hpp"
#include "Utility.hpp"
#include "xor_learning.hpp"
#include "mnist_learning.hpp"
#include "Losses.hpp"
#include <vector>
#include <string>

int main() {

	//Test_matrix();

	//XOR_learning();

	MNIST_learning();

	//std::vector<float> vec_true = { 0.5, -1 ,7, 1, 1, -6 };
	//Matrix<float> y_true(3, 2, vec_true);
	//y_true.display();

	//std::vector<float> vec_pred = {0, -1, 8, 2, 2, -5};
	//Matrix<float> y_pred(3, 2, vec_pred);
	//y_pred.display();

	//Losses loss("MSE");
	//Matrix<float> res = loss(y_true, y_pred, true);
	//res.display();



	//std::vector<float> vec_true = { 1, 1, 0, 0, 0, 0, 1, 1 };
	//Matrix<float> y_true(4, 2, vec_true);
	//y_true.display();

	//std::vector<float> vec_pred = {0.9, 0.8, 0.3, 0.01, 0.1, 0.2, 0.7, 0.99};
	//Matrix<float> y_pred(4, 2, vec_pred);
	//y_pred.display();

	//Losses loss("CategoricalCrossEntropy");
	//Matrix<float> res = loss(y_true, y_pred, true);
	//res.display();


	return 0;
}

