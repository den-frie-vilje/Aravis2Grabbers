#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // uncomment to log to file:
    //ofLogToFile("myLogFile.txt", true);
    
    ofSetFrameRate(25);
    
    ofSetEscapeQuitsApp(false);
    font.load( OF_TTF_SANS,9,true,true);
    
    camWidth = 1280;  // try to grab at this size.
    camHeight = 720;
    
    vidGrabber1.setSize(camWidth, camHeight);
    vidGrabber2.setSize(camWidth, camHeight);

    vidGrabber1.setup(0);
    vidGrabber2.setup(1);
    
    vidGrabber1.setExposure(-1);
    vidGrabber2.setExposure(-1);

    
    vidGrabber1.setPixelFormat(ARV_PIXEL_FORMAT_BAYER_RG_8);
    vidGrabber2.setPixelFormat(ARV_PIXEL_FORMAT_BAYER_RG_8);

    vidGrabber1.setFrameRate(25);
    vidGrabber2.setFrameRate(25);

    SyCam1.setName("Cam1");
    SyCam2.setName("Cam2");

    sender.setup("127.0.0.1", PORTOUT);
    ofLog() << "Opened OSC Sender";
    
    receiver.setup(PORTIN);

    //we can now get back a list of devices.
    getDevices();
    
    //turn off displays
    displayOI = 1;

    //set default devices

    videoTexture1.allocate(camWidth,camHeight,GL_RGBA);
    videoTexture2.allocate(camWidth,camHeight,GL_RGBA);
    ofSetVerticalSync(true);
}


//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0, 0, 0);
    vidGrabber1.update();
    vidGrabber2.update();
    
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        /*
        if(m.getAddress() == "/cam1/device"){
            //ofLog() << "b" << m.getArgAsInt32(0);
            vidGrabber1.setDeviceID(m.getArgAsInt32(0));
            vidGrabber1.setDesiredFrameRate(15);
            vidGrabber1.setPixelFormat(OF_PIXELS_NATIVE);
            vidGrabber1.setup(camWidth, camHeight);
        }
        else if(m.getAddress() == "/cam2/device"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            vidGrabber2.setDeviceID(m.getArgAsInt32(0));
            vidGrabber2.setDesiredFrameRate(15);
            vidGrabber2.setPixelFormat(OF_PIXELS_NATIVE);
            vidGrabber2.setup(camWidth, camHeight);
        }
        */
        //else
        if(m.getAddress() == "/getcams"){
            getDevices();
        }
        else if(m.getAddress() == "/display"){
            displayOI = m.getArgAsInt32(0);
            ofLog() << "display" << m.getArgAsInt32(0);
        }
        else if(m.getAddress() == "/cam1/zoom/x"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            zoomX1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/zoom/x"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            zoomX2 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam1/zoom/y"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            zoomY1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/zoom/y"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            zoomY2 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam1/anchor/x"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            anchorX1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/anchor/x"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            anchorX2 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam1/anchor/y"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            anchorY1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/anchor/y"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            anchorY2 = m.getArgAsFloat(0);
        }
        //BRCOSA
        else if(m.getAddress() == "/cam1/brightness"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            BR1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/brightness"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            BR2 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam1/contrast"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            CO1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/contrast"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            CO2 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam1/saturation"){
            //ofLog() << "zoomX1" << m.getArgAsFloat(0);
            SA1 = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/cam2/saturation"){
            //ofLog() << "t" << m.getArgAsInt32(0);
            SA2 = m.getArgAsFloat(0);
        }
    }

    if(vidGrabber1.isFrameNew()){
        videoTexture1 = vidGrabber1.getTexture();
        SyCam1.publishTexture(&videoTexture1);
    }
    
    if(vidGrabber2.isFrameNew()){
        videoTexture2 = vidGrabber2.getTexture();
        SyCam2.publishTexture(&videoTexture2);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (displayOI){
        ofSetHexColor(0xffffff);
        videoTexture1.draw(20 , 20, camWidth, camHeight);
        videoTexture2.draw(40 + camWidth, 20, camWidth, camHeight);
    }
    font.drawString("fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-150,640);

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // in fullscreen mode, on a pc at least, the
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...

    // Video settings no longer works in 10.7
    // You'll need to compile with the 10.6 SDK for this
    // For Xcode 4.4 and greater, see this forum post on instructions on installing the SDK
    // http://forum.openframeworks.cc/index.php?topic=10343
    if(key == 's' || key == 'S'){
        //vidGrabber1.videoSettings();
    }
    if(key == 'd' || key == 'D'){
        //vidGrabber1.videoSettings();
    }
    if(key == ' ' || key == ' '){
        ofToggleFullscreen();}
}



//--------------------------------------------------------------
void ofApp::getDevices(){
    
    std::vector<std::string>& devices = vidGrabber1.listDevices();
    
    ofxOscMessage m;
    m.setAddress("/camList");
    m.addBoolArg(TRUE);
    sender.sendMessage(m);
    
    for(int i = 0; i < devices.size(); i++){
        ofxOscMessage m;
        m.setAddress("/camList");
        m.addBlobArg(ofBuffer(devices[i].c_str(), devices[i].size()));
        ofLog() << "camera #" << i << ": " << devices[i];
        sender.sendMessage(m);
    }
    
    ofxOscMessage n;
    n.setAddress("/camListRdy");
    n.addBoolArg(TRUE);
    sender.sendMessage(n);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
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
