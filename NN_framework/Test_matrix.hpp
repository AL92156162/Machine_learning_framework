#pragma once

#include "matrix.hpp"
#include <vector>
#include <regex>


int Test_matrix() {

	std::string str;
	std::string file_name = "test_matmul.txt";
	std::ifstream myfile(file_name);
	std::regex reg(R"((\d+).(\d+))");
	std::smatch matches;
	std::vector<Matrix<float>> mat_vec;
	if (myfile.is_open())
	{
		int width = 0;
		int height = 0;
		std::vector<float> vec;

		while (getline(myfile, str, '\n')) {
			if (str.empty()) {
				Matrix<float> mat(width, height, vec);
				mat_vec.push_back(mat);
				height = 0;
				width = 0;
				vec.erase(vec.begin(), vec.end());
			}
			else {
				while (std::regex_search(str, matches, reg)) {
					vec.push_back(std::stof(matches[0]));
					str = matches.suffix();
				}
				if (width == 0) {
					width = vec.size();
				}
				height += 1;
			}

		}
	}
	int n = mat_vec.size() / 3;
	for (int i = 0; i < n; ++i) {
		Matrix<float> m1 = mat_vec[3 * i];
		Matrix<float> m2 = mat_vec[3 * i + 1];
		Matrix<float> m3 = mat_vec[3 * i + 2];
		Matrix<float> m4 = m1.multiply(m2);
		float delta = 0;
		for (int j = 0; j < m3.width() * m3.height(); ++j) {
			delta += m3.vec()[j] - m4.vec()[j];
		}
		if (delta != 0) {
			std::cout << delta << std::endl;
			//m1.display();
			//m2.display();
			//m3.display();
			//m4.display();
		}
	}
	return 0;
}