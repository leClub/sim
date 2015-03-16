#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0,0,0);

    //init Serial communication
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup("/dev/ttyACM0", baud);
    json = "";

    change = false;
	img.loadImage("2.png");

	//store the width and height for convenience
	int width = img.getWidth();
	int height = img.getHeight();

	//add one vertex to the mesh for each pixel
	for (int y = 0; y < height; y++){
		for (int x = 0; x<width; x++){
			mainMesh.addVertex(ofPoint(x,y,0));	// mesh index = x + y*width
												// this replicates the pixel array within the camera bitmap...
			mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
		}
	}

	for (int y = 0; y<height-1; y++){
		for (int x=0; x<width-1; x++){
			mainMesh.addIndex(x+y*width);				// 0
			mainMesh.addIndex((x+1)+y*width);			// 1
			mainMesh.addIndex(x+(y+1)*width);			// 10

			mainMesh.addIndex((x+1)+y*width);			// 1
			mainMesh.addIndex((x+1)+(y+1)*width);		// 11
			mainMesh.addIndex(x+(y+1)*width);			// 10
		}
	}

	//this determines how much we push the meshes out
	extrusionAmount = 100.0;

	changeHeightmap();
}

//--------------------------------------------------------------
void ofApp::update(){
    if( serial.available() ){
        //cout << "\tavailable: " << ofToString( serial.available() ) << endl;
        while( serial.available() ){
            lastChar = char( serial.readByte() );
            json += ofToString( lastChar );
        }

        if(lastChar == '\n'){
            bool parsingSuccessful = jsonEl.parse( json );

            if( parsingSuccessful ){
                if( jsonEl[ "btn" ] == 1 && change == false){
                    changeHeightmap();
                    change = true;
                }
                else{
                    change = false;
                }
            }
            json = "";
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(-img.getWidth()/2, -img.getHeight()/2, 0);
	ofTranslate(ofGetWidth()/2, 7 * ofGetHeight() / 10., 0);
	ofRotateX(70);
	mainMesh.drawFaces();
	ofPopMatrix();
}

void ofApp::changeHeightmap(){
    //random texture
    choice = (choice + 1) % NBIMAGES;

    string result;          // string which will contain the result
    ostringstream convert;   // stream used for the conversion
    convert << choice;      // insert the textual representation of 'Number' in the characters in the stream
    result = convert.str(); // set 'Result' to the contents of the stream
    img.loadImage(result + ".png");

    for (int i=0; i<img.getWidth()*img.getHeight(); i++){
        ofFloatColor sampleColor(img.getPixels()[i*3]/255.f,   // r
                                 img.getPixels()[i*3+1]/255.f, // g
                                 img.getPixels()[i*3+2]/255.f);// b

        ofVec3f tmpVec = mainMesh.getVertex(i);
        tmpVec.z = sampleColor.getBrightness() * extrusionAmount;
        mainMesh.setVertex(i, tmpVec);
        mainMesh.setColor(i, sampleColor);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    changeHeightmap();
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
