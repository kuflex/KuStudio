#pragma once

#include "ofMain.h"



class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void exitWithoutSaving();
    
    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void processMenu( string cmd );
    void processButtons();
    void processOsc();
    void processOscCommand( string cmd, vector<string> &values );
    
    void playAll();
    void playSegment();
    void stop();
    
    void projectWasUpdated();
private:
    bool _saveOnExit;
    void resized();
    
};
