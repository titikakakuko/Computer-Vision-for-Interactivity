#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	imgMan.load("running.jpg");
	imgBg.load("bg.jpg");

	matMan = toCv(imgMan);
	matBg = toCv(imgBg);

	for (int x = 0; x < matMan.cols; x++) {
		for (int y = 0; y < matMan.rows; y++) {
			int c = matMan.cols;
			if ((int(matMan.data[(x + y * c) * 3 + 1])) > 200) {
				matMan.data[(x + y * c) * 3] = matBg.data[(x + y * c) * 3];
				matMan.data[(x + y * c) * 3 + 1] = matBg.data[(x + y * c) * 3 + 1];
				matMan.data[(x + y * c) * 3 + 2] = matBg.data[(x + y * c) * 3 + 2];
			}

		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	drawMat(matMan, 0, 0);
}

