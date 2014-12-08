/*
  ==============================================================================

    Shapes.cpp
    Created: 29 Nov 2014 3:44:51pm
    Author:  Jeffrey Robbins

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>

#define PI 3.14159265


class Shape {
    float x, y;
    double* sHeight, sWidth, pitch;
    float degrees;
    bool selfRot{false}, rotate{false};
    
    
};

struct Bubble {
    float x;
    float y;
    float dist;
    double* sHeight;
    double* sWidth;
    double* pitch;
    int counter{0};
    float degrees;
    Bubble(int x, int y, float dist, float degrees, double& scHeight, double& scWidth, double& pitch) : sHeight(&scHeight),
                                                                                         sWidth(&scWidth)
    {
        this->pitch = &pitch;
        this->dist = dist;
        this->degrees = degrees;
        this->x = (dist * cos(degrees)) + *sWidth/2 - dist;
        this->y = (dist * sin(degrees)) + *sHeight/2 - dist;
    }
    
    //Draws bubble
    //g = graphics
    void draw(Graphics& g) {
        counter++;
            this->dist += 10;
            this->x = (dist * cos(degrees)) + *sWidth/2 - 50;
            this->y = (dist * sin(degrees)) + *sHeight/2 - 10;
        
        g.drawEllipse(this->x, this->y, *pitch/50, *pitch/50, 5);
    }
};

//Circle that spins around the center
class SpinningCircle {
    //bool clockwise;
    
    float dist;
    float degrees;
    float size;
    double* sHeight;
    double* sWidth;
    double* pitch;
    double maxPitch{0};
    std::deque<Bubble> bubbles;
    
public:
    float x;
    float y;
    SpinningCircle(float degrees, float dist, double& scHeight, double& scWidth, double& pitch) : sHeight(&scHeight),
    sWidth(&scWidth)
    {
        assert(scWidth > 0);
        assert(scHeight > 0);
        this->bubbles = *new std::deque<Bubble>();
        this->pitch = &pitch;
        this->dist = dist;
        this->degrees = degrees;
        this->x = (dist * cos(degrees)) + *sWidth/2 - dist;
        this->y = (dist * sin(degrees)) + *sHeight/2 - dist;
        this->maxPitch = .1;
    };
    
    //Draws circle
    //g = graphics
    //clockwise = is the circle spinnign clockwise?
    void draw(Graphics& g, bool clockwise) {
        assert(*pitch >= 0);
        if (*pitch > maxPitch * .5){
            if (dist > 120) {
                dist -= rand() % 14;
            }
            else {
                dist += rand() % 14;
            }
        }
        else if (*pitch < maxPitch * .4) {
            if (dist < 30) {
                dist += rand() % 14;
            }
            else {
                dist -= rand() % 14;
            }
        }
        
        size = (*pitch/maxPitch) * 40;
        
        degrees += clockwise ? 10 * (*pitch / maxPitch) : -10 * (*pitch/maxPitch);
        this->x = (dist * cos(degrees * PI / 180.0)) + *sWidth/2 - 50;
        this->y = (dist * sin(degrees * PI / 180.0)) + *sHeight/2 - 10;
        
        assert(this->x < 9999);
        assert(this->y < 9999);
        
        
        g.fillEllipse(this->x - size/2, this->y - size/2, size, size);
        g.drawLine(this->x, this->y, *sWidth/2 -50, *sHeight/2 - 10);
        maxChecker();
        
        for (int i = 0; i < bubbles.size(); i++) {
            bubbles[i].draw(g);
        }
    }
    
    //checks if pitch is > maxPitch and sends out bubbles
    void maxChecker() {
        if (*pitch > maxPitch) {
            maxPitch = *pitch;
        }
        if (*pitch + 50 >= maxPitch) {
            assert(&bubbles != nullptr);
            //std::cout << "bubble"<< std::endl;
            bubbles.push_back(Bubble(x, y, dist, degrees, *sHeight, *sWidth, *pitch));
        }
        if (bubbles.size() > 0 &&
            (bubbles.front().x > *sWidth || bubbles.front().x < 0
             ||bubbles.front().y > *sHeight || bubbles.front().y < 0)) {
                
                assert(bubbles.size() > 0);
                bubbles.pop_front();
            }
        
        
    }
    
};

//Class to handles all of the shapes for the visualizer
class Shapes {
    bool clockwise = true;
    double* pScreenHeight;
    double* pScreenWidth;
    double lastPitch;
    double* pPitch;
    std::vector<SpinningCircle *> circs;
    //std::vector<SpinningSquare *> squares;
    
public:
    Shapes() {
    }
    Shapes(double& height, double& width, double& pitch) {
        this->pPitch = &pitch;
        this->pScreenHeight = &height;
        this->pScreenWidth = &width;
        this->circs = *new std::vector<SpinningCircle *>();
        //this->squares = *new std::vector<SpinningSquare *>();
    }
    ~Shapes() {
        for (int i = 0; i < circs.size(); i++) {
            delete circs[i];
        }
        circs.clear();
    }
    
    //creates a circle that will spin around the center
    void makeSpinningCircle(float degrees, float dist) {
        circs.push_back(new SpinningCircle(degrees, dist, *pScreenHeight, *pScreenWidth, *pPitch));
    }
    
    
    //method for drawing 
    void drawShapes(Graphics& g) {
        clockwise = rand() % 1000 < 20 ? !clockwise : clockwise;
        float lastX = 0;
        float lastY = 0;
        for (int i = 0; i < circs.size(); i++) {
            if (lastX != 0) {
                g.drawLine(lastX, lastY, circs[i]->x, circs[i]->y,3);
            }
            lastX = circs[i]->x;
            lastY = circs[i]->y;
            circs[i]->draw(g, clockwise);
        }
        g.drawLine(lastX, lastY, circs[0]->x, circs[0]->y,3);
       
    }
};
