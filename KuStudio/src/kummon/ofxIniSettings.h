#ifndef OFX_INISETTINGS_H
#define OFX_INISETTINGS_H

/************************************************************

OpenFrameworks Library

File: 			ofxIniSettings.h
Description: 	Read and Write .ini files

Last Modified: 2009.01.22
Creator: Rick Companje
Version: 0.1

************************************************************/

//#include <conio.h>
#include <fstream>
#include <iostream>
#include <map>

#include "ofMain.h"
//#include "ofxVectorMath.h"

class ofxIniSettings {
public:

    ofxIniSettings() {}
    ofxIniSettings(string filename) { load(filename); }

    //rick 15-1-2009! i changed clearFirst default to true
    bool load(string filename, bool clearFirst=true); // you can call multiple times with different files, incremental
    bool has(string key) { return keys[key]!=""; };
    void clear();

    map<string,string> keys;
    string outputFilename;

    //getters
    int get(string key, int defaultValue);
    bool get(string key, bool defaultValue);
    float get(string key, float defaultValue);
    string get(string key, string defaultValue);
    //ofxVec2f get(string key, ofxVec2f defaultValue);
    //ofxVec3f get(string key, ofxVec3f defaultValue);
    //ofxVec4f get(string key, ofxVec4f defaultValue);
    ofRectangle get(string key, ofRectangle defaultValue);

    //setters
    void setString(string id, string value);

    template <typename T> void set(string id, string key, T value) { set(id=="" ? key : (id+"."+key), value); } //returntype void
    template <typename T> void set(string key, T value) { setString(key, ofToString(value)); } //returntype void

};

bool ofFileExists(string filename);

#endif

