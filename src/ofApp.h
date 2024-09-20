// ofApp.h
#pragma once

#include "ofMain.h"
#include <vector>

struct Site {
    ofVec2f position;
    ofColor color;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    std::vector<Site> sites;
    ofImage img;
    std::vector<std::vector<int>> regions;
    bool showSites;
};