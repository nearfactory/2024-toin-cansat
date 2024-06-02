#pragma once

#include "ofTrueTypeFont.h"
#include "ofSerial.h"

#include "ofMain.h"
#include "hoge.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

		//
		std::string characters[11][5] = {
		  { "Ç†", "Ç¢", "Ç§", "Ç¶", "Ç®" },
		  { "Ç©", "Ç´", "Ç≠", "ÇØ", "Ç±" },
		  { "Ç≥", "Çµ", "Ç∑", "Çπ", "Çª" },
		  { "ÇΩ", "Çø", "Ç¬", "Çƒ", "Ç∆" },
		  { "Ç»", "Ç…", "Ç ", "ÇÀ", "ÇÃ" },
		  { "ÇÕ", "Ç–", "Ç”", "Ç÷", "ÇŸ" },
		  { "Ç‹", "Ç›", "Çﬁ", "Çﬂ", "Ç‡" },
		  { "Ç‚", "ÇÓ", "Ç‰", "ÇÔ", "ÇÊ" },
		  { "ÇÁ", "ÇË", "ÇÈ", "ÇÍ", "ÇÎ" },
		  { "ÇÌ", "Å@", "Ç", "Å@", "ÇÒ" },
		  { "ÅJ", "ÅK", "space", "backspace", "send" }
		};
		
		ofSerial serial;
		std::string serial_received = "";
	std::string status_str{};
		string device_name{};

		struct XYcoordinate {
			uint8_t x = 0, y = 0;
		};
		XYcoordinate coordinate;

		ofTrueTypeFont font;
		const int FONT_SIZE = 16;

		bool button[5] = {};
		bool previous_button[5] = {};
};
