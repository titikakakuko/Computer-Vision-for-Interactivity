#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	imgLena.load("lena.jpg");
	matLena = toCv(imgLena);

	imgCircuit.load("circuit.tif");
	matCircuit = toCv(imgCircuit);

	// Determine whether the image is loaded successfully
	//if (!imgLena.data)
	//{
	//	cout << "failed!" << endl;
	//	return;
	//}
}

//--------------------------------------------------------------
void ofApp::update() {
}


//--------------------------------------------------------------
void ofApp::draw() {

	int params1[] = { 3,5,7,9,11 };
	int params2[] = { 4,5,6,7,8,9,10 };

	// To see the results of each question, please comment out the code for the other questions to avoid confusion.

	// (1) Lena boxFilter

	imgLena.draw(0,0);

	for (int i = 0; i < size(params1); i++)
	{
		boxFilter(matLena, matNewLena, -1, Size(params1[i], params1[i]));
		drawMat(matNewLena, imgLena.getWidth()*((i + 1) % 3), imgLena.getHeight()*((i + 1) / 3));
	}
	
	// (1) Circuit boxFilter

	imgCircuit.draw(0, 0);

	for (int i = 0; i < size(params1); i++)
	{
		boxFilter(matCircuit, matNewCircuit, -1, Size(params1[i], params1[i]));
		drawMat(matNewCircuit, imgCircuit.getWidth()*((i + 1) % 3), imgCircuit.getHeight()*((i + 1) / 3));
	}


	// (2) Lena GaussianBlur

	imgLena.draw(0,0);

	for (int i = 0; i < size(params1); i++)
	{
		for (int j = 0; j < size(params2); j++)
		{
			GaussianBlur(matLena, matNewLena, Size(params1[i], params1[i]), params2[j], params2[j]);
			drawMat(matNewLena, imgLena.getWidth()*((j + 1) % 4), imgLena.getHeight()*((j + 1) / 4 + i * 2));
		}
	}

	
	// (2) Circuit GaussianBlur

	imgCircuit.draw(0, 0);

	for (int i = 0; i < size(params1); i++)
	{
		for (int j = 0; j < size(params2); j++)
		{
			GaussianBlur(matCircuit, matNewCircuit, Size(params1[i], params1[i]), params2[j], params2[j]);
			drawMat(matNewCircuit, imgCircuit.getWidth()*((j + 1) % 4), imgCircuit.getHeight()*((j + 1) / 4 + i * 2));
		}
	}
	
	// (3) Lena medianBlur

	imgLena.draw(0, 0);

	for (int i = 0; i < size(params1); i++)
	{
		medianBlur(matLena, matNewLena, params1[i]);
		drawMat(matNewLena, imgLena.getWidth()*((i + 1) % 3), imgLena.getHeight()*((i + 1) / 3));
	}

	
	// (3) Circuit medianBlur

	imgCircuit.draw(0, 0);

	for (int i = 0; i < size(params1); i++)
	{
		medianBlur(matCircuit, matNewCircuit, params1[i]);
		drawMat(matNewCircuit, imgCircuit.getWidth()*((i + 1) % 3), imgCircuit.getHeight()*((i + 1) / 3));
	}
	
}