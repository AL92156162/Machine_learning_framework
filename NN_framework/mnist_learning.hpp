#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "matrix.hpp"
//#include <opencv2/opencv.hpp>
//#include <opencv2/ml.hpp>
#include <cstdint>

// Function to read IDX3-UBYTE files
std::vector<Matrix<unsigned char>> readIDX3UByteFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the IDX3-UBYTE file." << std::endl;
        return {};
    }

    // Read the IDX3-UBYTE file header
    char magicNumber[4];
    char numImagesBytes[4];
    char numRowsBytes[4];
    char numColsBytes[4];

    file.read(magicNumber, 4);
    file.read(numImagesBytes, 4);
    file.read(numRowsBytes, 4);
    file.read(numColsBytes, 4);

    //std::cout << static_cast<int>(numImagesBytes[0]) << "  " << static_cast<int>(numImagesBytes[1]) << "  " <<
    //    (int)static_cast<unsigned char>(numImagesBytes[2]) << "  " << static_cast<int>(numImagesBytes[3]) << "  " << std::endl;

    // Convert the header information from big-endian to native endianness
    int numImages = (static_cast<unsigned char>(numImagesBytes[0]) << 24) | (static_cast<unsigned char>(numImagesBytes[1]) << 16) | (static_cast<unsigned char>(numImagesBytes[2]) << 8) | static_cast<unsigned char>(numImagesBytes[3]);
    int numRows = (static_cast<unsigned char>(numRowsBytes[0]) << 24) | (static_cast<unsigned char>(numRowsBytes[1]) << 16) | (static_cast<unsigned char>(numRowsBytes[2]) << 8) | static_cast<unsigned char>(numRowsBytes[3]);
    int numCols = (static_cast<unsigned char>(numColsBytes[0]) << 24) | (static_cast<unsigned char>(numColsBytes[1]) << 16) | (static_cast<unsigned char>(numColsBytes[2]) << 8) | static_cast<unsigned char>(numColsBytes[3]);

    std::cout << numImages << ";" << numRows << ";" << numCols << std::endl;

    // Initialize a vector to store the images
    std::vector<Matrix<unsigned char>> images;

    for (uint32_t i = 0; i < numImages; ++i) {

        // Read each image as a vector of bytes
        std::vector<unsigned char> imvec(numRows * numCols);
        file.read((char*)(imvec.data()), numRows * numCols);

        Matrix<unsigned char> image(numRows, numCols, imvec);
        images.push_back(image);
    }

    file.close();

    return images;
}

// Function to read IDX3-UBYTE files
std::vector<unsigned char> readLabelFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the IDX3-UBYTE file." << std::endl;
        return {};
    }

    // Read the IDX3-UBYTE file header
    char magicNumber[4];
    char numImagesBytes[4];

    file.read(magicNumber, 4);
    file.read(numImagesBytes, 4);

    // Convert the header information from big-endian to native endianness
    int numImages = (static_cast<unsigned char>(numImagesBytes[0]) << 24) | (static_cast<unsigned char>(numImagesBytes[1]) << 16) | (static_cast<unsigned char>(numImagesBytes[2]) << 8) | static_cast<unsigned char>(numImagesBytes[3]);

    // Initialize a vector to store the images
    std::vector<unsigned char> labels;

    for (int i = 0; i < numImages; i++) {
        // Read each image as a vector of bytes
        unsigned char label(1);
        file.read((char*)&label, 1);
        labels.push_back(label);
    }

    file.close();

    return labels;
}

int MNIST_learning() {

    std::string filename = "D:/Stockage/Travail/BDD/MNIST/train-images.idx3-ubyte";
    std::string label_filename = "D:/Stockage/Travail/BDD/MNIST/train-labels.idx1-ubyte";

    std::cout << "Extracting dataset\n";
    std::vector<Matrix<unsigned char>> images = readIDX3UByteFile(filename);
    images[0].display();

    std::vector<unsigned char> labels = readLabelFile(label_filename);
    std::cout << labels[0] << static_cast<int>(labels[0])<< std::endl;

    std::cout << "Preparing training data\n";
    uint32_t n_image = 500;
    uint32_t width = images[0].width();
    uint32_t height = images[0].height();

    std::vector<unsigned char> imvec;
    std::vector<uint32_t> labels_true(n_image);
    for (uint32_t i = 0; i < n_image; ++i) {
        imvec.insert(imvec.end(), images[i].vec().begin(), images[i].vec().end());
        labels_true[i] = labels[i];
    }

    // Definition of X (width * height, n_image) then transpose
    std::vector<float> float_imvec(imvec.begin(), imvec.end());
    Matrix<float> X(width * height, n_image, float_imvec);
    X = X.transpose();
    Matrix<float> y = oneHotEncode(labels_true);

    X = normalizeMatrix(X);

    writeMatrix("out/X_mnist.txt", X);

    std::cout << std::endl;
    for (int i = 0; i < n_image; ++i) {
        std::cout << static_cast<int>(labels[i]) << ";";
    }
    std::cout << std::endl;
    y.display();
    
    //std::vector<Matrix<float>> training_vec = { X, y };
    //shuffle(training_vec);
    //std::vector<Matrix<float>> train_test_matrices = split_train_test(training_vec, 0.8);
    //Matrix X_train = train_test_matrices[0];
    ////X_train.display();
    //Matrix y_train = train_test_matrices[2];
    ////y_train.display();
    //Matrix X_test = train_test_matrices[1];
    ////X_test.display();
    //Matrix y_test = train_test_matrices[3];
    ////y_test.display();

    //creating neural network 
    std::cout << "Building model\n";
    std::vector<unsigned int> topology = { width*height, 30, 10 };
    MLP model(topology, 1.0, 0.0, sigmoid, dSigmoid);

    std::cout << "Training start\n";
    std::vector<float> loss = model.fit(X, y, 100, true);
    writeVector("out/MNIST_loss.txt", loss);
;
    Matrix<float> y_pred = model.predict(X);
    y_pred.display();
    

    Losses m_losses("CCE");
    Matrix<float> loss2 = m_losses(y, y_pred, true);
    std::cout << "Loss : " << loss2.getValue(0, 0) << std::endl;

    Matrix<float> y_pred_classif = getClassificationPred(y_pred);
    
    //y.display();
    //y_pred_classif.display();
    
    std::vector<uint32_t> labels_pred = oneHotDecode(y_pred_classif);
    
    DisplayVec(labels_true);
    DisplayVec(labels_pred);
    
    std::cout << std::endl;
    
    float acc = accuracy(labels_true, labels_pred);
    std::cout << "Accuracy : " << acc << std::endl;
    
    std::cout << std::endl;
    
    Matrix<float> cm = confusionMatrix(y, y_pred_classif);
    cm.display();


    //Matrix delta_mat = y - y_pred;
    //y.display();
    //std::cout << std::endl;
    //y_pred.display();
    //std::cout << std::endl;
    //delta_mat.display();

    return 0;
}