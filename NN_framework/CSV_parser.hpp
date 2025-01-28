#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

template <typename T>
void writeMatrix(std::string file_name, Matrix<T> mat) {
	std::ofstream myfile(file_name);
	if (myfile.is_open())
	{
		for (int y = 0; y < mat.height(); ++y) {
			for (int x = 0; x < mat.width(); ++x) {
				std::string str = std::to_string(mat.getValue(x,y)) + ';';
				myfile << str;
			}
			myfile << "\n";
		}
		myfile.close();
	}
}

template <typename T>
void writeVector(std::string file_name, std::vector<T> vec) {
	std::ofstream myfile(file_name);
	if (myfile.is_open())
	{
		for (int i = 0; i < vec.size(); ++i) {
			std::string str = std::to_string(vec[i]) + ';';
			myfile << str;
		}
		myfile.close();
	}
}


