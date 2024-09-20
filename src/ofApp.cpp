#include "ofApp.h"
#include <vector>
#include <cmath>
#include <algorithm>


void ofApp::setup() {
    // 1. Load an image
    img.load("LOAD IMAGE HERE");
    img.resize(512, 512);

    // 2. Place k starting sites
    int k = 150; // Number of sites
    for (int i = 0; i < k; i++) {
        Site site;
        site.position = ofVec2f(ofRandom(img.getWidth()), ofRandom(img.getHeight()));
        site.color = img.getColor(site.position.x, site.position.y);
        sites.push_back(site);
    }

    regions.resize(img.getWidth(), std::vector<int>(img.getHeight()));
}

void ofApp::update() {
    // 3. Compute regions for all sites
    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            ofColor pixelColor = img.getColor(x, y);
            float minDist = std::numeric_limits<float>::max();
            int closestSite = 0;

            for (int i = 0; i < sites.size(); i++) {
                float dist = ofVec2f(x, y).distance(sites[i].position) * 2; // Screen distance weight
                dist += ofVec3f(pixelColor.r, pixelColor.g, pixelColor.b).distance(
                    ofVec3f(sites[i].color.r, sites[i].color.g, sites[i].color.b)); // Color distance

                if (dist < minDist) {
                    minDist = dist;
                    closestSite = i;
                }
            }

            regions[x][y] = closestSite;
        }
    }

    // 4. Compute average position and color for each region
    std::vector<ofVec2f> newPositions(sites.size(), ofVec2f(0, 0));
    std::vector<ofVec3f> newColors(sites.size(), ofVec3f(0, 0, 0));
    std::vector<int> counts(sites.size(), 0);

    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            int siteIndex = regions[x][y];
            ofColor pixelColor = img.getColor(x, y);

            newPositions[siteIndex] += ofVec2f(x, y);
            newColors[siteIndex] += ofVec3f(pixelColor.r, pixelColor.g, pixelColor.b);
            counts[siteIndex]++;
        }
    }

    for (int i = 0; i < sites.size(); i++) {
        if (counts[i] > 0) {
            sites[i].position = newPositions[i] / counts[i];
            ofVec3f avgColor = newColors[i] / counts[i];
            sites[i].color = ofColor(avgColor.x, avgColor.y, avgColor.z);
        }
    }
}

void ofApp::draw() {
    // 6. Display the output
    ofPixels pixels = img.getPixels();
    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            pixels.setColor(x, y, sites[regions[x][y]].color);
        }
    }
    img.setFromPixels(pixels);
    img.draw(0, 0);

    // Draw boundaries
    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            if (x > 0 && regions[x][y] != regions[x - 1][y] ||
                y > 0 && regions[x][y] != regions[x][y - 1] ||
                x < img.getWidth() - 1 && regions[x][y] != regions[x + 1][y] ||
                y < img.getHeight() - 1 && regions[x][y] != regions[x][y + 1]) {
                ofDrawRectangle(x, y, 1, 1);
            }
        }
    }

    // 7. Draw small circles at each site location
    if (showSites) {
        for (const auto& site : sites) {
            ofDrawCircle(site.position, 1);
        }
    }
}