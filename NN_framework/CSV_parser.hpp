#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


void writeMatrix() {
	// Do nothing
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


