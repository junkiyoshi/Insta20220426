#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 120 < 45) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 45, 0.05, 0.001);
	}


	this->face.clear();
	this->frame.clear();
	this->setBoxRecursive(glm::vec3(0, 0, 0), 300);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.333333);

	this->face.draw();
	this->frame.drawWireframe();

	ofSetColor(255);
	for (auto& vertex : this->face.getVertices()) {

		ofDrawSphere(vertex, 3);
	}

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxRecursive(glm::vec3 location, float size) {

	vector<glm::vec3> location_list = {
		location + glm::vec3(-size * 0.5, -size * 0.5, size * 0.5),
		location + glm::vec3(-size * 0.5, size * 0.5, size * 0.5),
		location + glm::vec3(size * 0.5, size * 0.5, size * 0.5),
		location + glm::vec3(size * 0.5, -size * 0.5, size * 0.5),
		location + glm::vec3(-size * 0.5, -size * 0.5, -size * 0.5),
		location + glm::vec3(-size * 0.5, size * 0.5, -size * 0.5),
		location + glm::vec3(size * 0.5, size * 0.5, -size * 0.5),
		location + glm::vec3(size * 0.5, -size * 0.5, -size * 0.5)
	};


	if (size < 50) {

		for (auto& l : location_list) {

			auto noise_value = ofNoise(glm::vec4(l * 0.05, this->noise_param));

			if (noise_value < 0.50) {

				this->setBoxToMesh(this->face, this->frame, l, size);
			}
		}
	}
	else {

		for (auto& l : location_list) {

			auto noise_value = ofNoise(glm::vec4(l * 0.05, this->noise_param));


			if (size == 300 || noise_value < 0.45) {

				this->setBoxRecursive(l, size * 0.5);
			}
			else if (noise_value < 0.50) {

				/*
				auto size_param = ofMap(noise_value, 0.45, 0.7, 0, 1);
				this->setBoxToMesh(this->face, this->frame, l, size * 1);
				*/
			}
			else {

				this->setBoxToMesh(this->face, this->frame, l, size);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth) {

	auto noise_value = ofNoise(glm::vec4(location *  0.005, ofGetFrameNum() * 0.008));
	if (noise_value < 0.45) { noise_value = 0; }
	else if (noise_value < 0.75) { noise_value = ofMap(noise_value, 0.45, 0.75, 0, 1); }
	else { noise_value = 1; }

	int face_index = face_target.getNumVertices();
	int frame_index = frame_target.getNumVertices();

	vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	vertices.push_back(glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	vertices.push_back(glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	vertices.push_back(glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	for (auto& vertex : vertices) {

		auto noise_vertex = glm::vec3(
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -width * 0.5, width * 0.5),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -height * 0.5, height * 0.5),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -depth * 0.5, depth * 0.5));

		auto distance = vertex - noise_vertex * noise_value;
		auto d = (distance / 1) * noise_value;

		face_target.addVertex(location + vertex * 0.99 - d);
		frame_target.addVertex(location + vertex - d);
	}

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 1);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);
	face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 2);

	face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 4);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 3);

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 0);

	frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 5); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	frame_target.addIndex(frame_index + 7); frame_target.addIndex(frame_index + 4);

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 4);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 7);

	for (int i = 0; i < 8; i++) {

		face_target.addColor(ofColor(0));
		frame_target.addColor(ofColor(255));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}