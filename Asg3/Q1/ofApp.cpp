#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	if (!vid.load("faceDetection.mov")) {
		cout << "Error loading video";
	}
	else {
		vid.play();
	}

	//need to use absolute path here, for the latest version of OpenFrameworks and its OpenCV addon
	if (!face_cascade.load("C:/Users/w/Desktop/cs/2020-2021/cs4187cv/faceDetection_Image/bin/data/haarcascade_frontalface_default.xml")) {
		cout << "Error loading haarcascade_frontalface_default.xml" << endl;
	}

	if (!eye_cascade.load("C:/Users/w/Desktop/cs/2020-2021/cs4187cv/faceDetection_Image/bin/data/haarcascade_eye.xml")) {
		cout << "Error loading haarcascade_eye.xml" << endl;
	}


}

//--------------------------------------------------------------
void ofApp::update() {
	vid.update();
	vid.getCurrentFrame();

	if (vid.isFrameNew()) {

		ofPixels pixs = vid.getPixels();
		vidImg.setFromPixels(pixs);
		vidMat = toCv(vidImg);

	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	drawMat(vidMat, 0, 0);

	// Because of my OpenCv version, I use CASCADE_SCALE_IMAGE instead of CV_HAAR_SCALE_IMAGE
	// You can adjust for your own version of OpenCv. 
	// Here are the reference links: https://docs.opencv.org/3.1.0/d9/d31/group__objdetect__c.html#ga812f46d031349fa2ee78a5e7240f5016
	//                               https://answers.opencv.org/question/20066/simple-face-recognition/
	face_cascade.detectMultiScale(vidMat, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));
	for (int i = 0; i < faces.size(); i++) {

		//In each detected face, detect eyes
		eye_cascade.detectMultiScale(vidMat, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		for (int j = 0; j < 2; j++) {
			ofNoFill();
			//There must be eyes inside face area
			if ((faces.at(i).x < eyes.at(j).x) &&
				((faces.at(i).x + faces.at(i).width) > eyes.at(j).x) &&
				(faces.at(i).y < eyes.at(j).y) &&
				((faces.at(i).y + faces.at(i).height) > eyes.at(j).y)) {
				ofDrawRectangle(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
			}
		}
	}
}