#include "ofApp.h"

// ・GPSデータ
// ・通信(ボタン・ディスプレイ）

//--------------------------------------------------------------
void ofApp::setup() {

	// window
	ofSetWindowShape(1000, 1000);
	ofSetFrameRate(60);
	ofSetWindowTitle("Toin highschool Cansat2024");
	cout << "----setup" << boolalpha << endl << endl;

	// font
	cout << "load font:" << font.load("C:/Users/magur/OneDrive/ドキュメント/.ScienceClub/2024-toin-cansat/プログラム/CansatDesktop/src/yu.ttf", FONT_SIZE) << endl << endl;

	// serial
	bool serial_succeed = false;
	while (!serial_succeed) {
		serial.listDevices();
		cout << "Device scan:" << endl;
		vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();
		for (auto& device : deviceList) cout << device.getDeviceName() << endl;
		cout << ">";

		cin >> device_name;
		serial_succeed = serial.setup(device_name, 9600);
		cout << "serial setup:" << serial_succeed << endl << endl;
	}
	cout << endl << "----loop" << endl << endl;
}


//--------------------------------------------------------------
void ofApp::update() {
	vector<char> serial_c(0);
	bool difference = false;
	bool available = serial.available();
	while (serial.available()) {
		difference = true;
		serial_c.push_back(static_cast<char>(serial.readByte()));
	}
	if (available) {
		// cout << "communication err" << endl;
		// cout << "length:" << serial_c.size() << endl;

		string buf{ serial_c.begin(),serial_c.end() };
		serial_received = buf;
		char c = serial_c[0];
		
		for (int i = 4; i >= 0; i--) {
			previous_button[i] = button[i];
			button[i] = (c >> i) & 0x01;
			//if(c>>i)
			//cout << button[i] << " ";
			//cout << serial_received << endl;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);

	// stats
	std::string stats_str = "";
	string button_str = "";
	for (int i = 0; i < 5; i++){
		if (button[i] == true) button_str += "O";
		else button_str += "_";
	}
	//cout << button_str << endl;
	string time = to_string(ofGetHours()) + ":" + to_string(ofGetMinutes()) + ":" + to_string(ofGetSeconds());
	stats_str =
		"Port:" + device_name + "\n" +
		"Time:" + time + "\n" +
		"Serial:" + serial_received + "\n" +
		"Button:" + button_str + "\n"
		;
	font.drawString(stats_str, 0, FONT_SIZE);

	ofSetLineWidth(4);
	ofColor(16, 32, 230);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
