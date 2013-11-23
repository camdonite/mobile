/* Provides helper functions and global defines
 *
 */

#ifndef HELPERS_H
#define HELPERS_H

#define DEBUG

#define _USE_MATH_DEFINES
using namespace std;
#include <iostream>
#include <GL/glut.h>				// include GLUT library
#include <cmath>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include "soil\SOIL.h"

double toDegree(double);
//converts radains to degrees

double toRadian(double);
//converts degrees to radians

GLuint loadTexture(string);

void renderBitmapString(double, double, void*, string);
//displays the string on the screen (xcoord, ycoord, font, diplaystring)

void coordinates(GLfloat);
//draws the coordinate axis to the distance specifyed
#endif