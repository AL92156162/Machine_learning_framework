#pragma once

int XOR_learning() {

	// creating neural network 2 input neurons, 3 hidden neurons and 1 output neuron 
	std::vector<unsigned int> topology = { 2,3,1 };
	MLP model(topology, 0.1, 0.0, sigmoid, dSigmoid);
	//MLP model(topology, 0.1, 0.0, relu, dRelu);

	//sample dataset
	std::vector<std::vector<float>> targetInputs = {
		{0.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f}
	};
	std::vector<std::vector<float>> targetOutputs = {
		{0.0f},
		{0.0f},
		{1.0f},
		{1.0f}
	};


	//Build training set
	int n_sample = 100;
	std::vector<float> xvec(2 * n_sample);
	std::vector<float> yvec(n_sample);
	for (uint32_t i = 0; i < n_sample; i++)
	{
		int index = rand() % 4;

		xvec[2 * i] = targetInputs[index][0];
		xvec[2 * i + 1] = targetInputs[index][1];
		yvec[i] = targetOutputs[index][0];
	}

	Matrix X(2, n_sample, xvec);
	X = X.transpose();
	Matrix y(n_sample, 1, yvec);

	std::vector<Matrix<float>> training_vec = { X, y };
	shuffle(training_vec);

	X = training_vec[0];
	X.display();

	y = training_vec[1];
	y.display();

	std::vector<Matrix<float>> train_test_matrices = split_train_test(training_vec, 0.8);
	
	Matrix X_train = train_test_matrices[0];
	X_train.display();
	Matrix y_train = train_test_matrices[2];
	y_train.display(); 
	Matrix X_test = train_test_matrices[1];
	X_test.display();
	Matrix y_test = train_test_matrices[3];
	y_test.display();



	//training the neural network with randomized data
	//std::cout << "\ntraining start\n";
	//uint32_t epoch = 10000;
	//std::vector<float> errors;
	//for (uint32_t i = 0; i < epoch; i++)
	//{
	//	uint32_t index = rand() % 4;
	//	model.backProgagate(targetInputs[index], targetOutputs[index]);
	//	errors.push_back(model.error(targetInputs[index], targetOutputs[index]));
	//}
	//std::cout << "training complete \n";
	//writeVector("Error.txt", errors);

	//testing the neural network

	std::vector<float> errors = model.fit(X, y, 100, true);
	writeVector("out/XOR_loss.txt", errors);

	Matrix y_pred = model.predict(X);
	Matrix delta_mat = y - y_pred;
	y.display();
	std::cout << std::endl;
	y_pred.display();
	std::cout << std::endl;
	delta_mat.display();

	/*std::vector<Matrix<float>> weights = model.getWeights();
	std::vector<Matrix<float>> biais = model.getBiais();
	std::vector<Matrix<float>> values = model.getValues();
	std::vector<Matrix<float>> weights_delta = model.getWeightsDelta();
	std::vector<Matrix<float>> biais_delta = model.getBiaisDelta();

	std::cout << "Weights : " << std::endl;
	for (int i = 0; i < weights.size(); ++i) {
		weights[i].display();
	}
	std::cout << "Biais : " << std::endl;
	for (int i = 0; i < biais.size(); ++i) {
		biais[i].display();
	}
	std::cout << "Values : " << std::endl;
	for (int i = 0; i < values.size(); ++i) {
		values[i].display();
	}
	std::cout << "delta Weights : " << std::endl;
	for (int i = 0; i < weights_delta.size(); ++i) {
		weights_delta[i].display();
	}
	std::cout << "delta Biais : " << std::endl;
	for (int i = 0; i < biais_delta.size(); ++i) {
		biais_delta[i].display();
	}*/

	return 0;
}