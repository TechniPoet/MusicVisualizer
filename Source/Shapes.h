/*
  ==============================================================================

    Shapes.h
    Created: 29 Nov 2014 3:44:51pm
    Author:  Jeffrey Robbins

  ==============================================================================
*/
/*
#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>

//Generic shape class for every shape
class Shape {
protected:
    //degrees: degrees around center that the shape should be placed
    //size: size of shape
    //dist: distance from center of screen
    float degrees, size, dist;
    //sHeight: pointer to screen height
    //sWidth: pointer to screen width
    //pitch: pointer to current pitch
    //maxPitch: highest pitch detected so far -
    //          used for relative pitch checking
    double* sHeight, *sWidth, *pitch, maxPitch{.01};
    //offsetX / offsetY: offsets of origin for geometrical calculations
    float offsetX, offsetY;
    //x,y: Cartesian coordinates for shape
    float x, y;
    
public:
    /*
     Constructor for generic shape
     degrees: degrees around center that the shape should be placed
     dist: distance from center of screen
     scHeight: screen height reference
     scWidth: screen width reference
     pitch: current pitch reference
     */
    Shape(float degrees, float dist, double& scHeight, double& scWidth, double& pitch);
    
    //Destructor
    ~Shape() {}
    
    //Getter function for x Coordinate
    float getX();
    //Getter funciton for y Coordinate
    float getY();
    
    //Checks if current pitch exceeds any previous
    //readings. if so then we make current pitch our new max
    void maxChecker();
};

//defines Bubble for use in SpinningShape
class Bubble;


class SpinningShape : public Shape {
protected:
    float tlCornerX, tlCornerY,
    trCornerX, trCornerY,
    blCornerX, blCornerY,
    brCornerX, brCornerY,
    offsetX, offsetY;
    float** pColors;
    float rotDeg;
    int phase;
    Colour *color;
    std::deque<Bubble> bubbles;
public:
    SpinningShape (float degrees, float dist, double& scHeight, double& scWidth, double& pitch,float* colors[3]);
    SpinningShape (float degrees, float dist, double& scHeight, double& scWidth, double& pitch);
    
    void drawCircle (Graphics& g);
    void drawSquare(Graphics& g);
    
    void manageBubbles(Graphics& g, bool clockwise);
    void draw(Graphics& g, bool clockwise, int phase);
};


//Bubble extends Shape
/*
 This class is defined for bubbles that are generated
 by SpinningShape
 The main difference is the lack of
 */
class Bubble : public SpinningShape {
public:
    Bubble (float degrees, float dist, double& scHeight, double& scWidth, double& pitch, int phase);
    
    void drawCircle (Graphics& g);
    void drawSquare(Graphics& g);
    
    void draw(Graphics& g, bool clockwise);
};



//Class to handles all of the shapes for the visualizer
class Shapes {
    
    float *pRed = new float(50);
    float *pBlue = new float(50);
    float *pGreen = new float(50);
    float* colors[3] = {pRed, pBlue, pGreen};
    bool clockwise = true;
    double* pScreenHeight;
    double* pScreenWidth;
    double lastPitch;
    double* pPitch;
    int phase{0}, counter{0};
    std::vector<SpinningShape *> spinShapes;
    
public:
    Shapes() {}
    Shapes(double& height, double& width, double& pitch);
    ~Shapes();
    
    void swap ();
    void addSpinner(float degrees, float dist);
    
    //method for drawing
    void drawShapes(Graphics& g);
};


/*
#endif  // SHAPES_H_INCLUDED
*/