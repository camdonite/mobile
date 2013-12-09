#ifndef PICTURE_H
#define PICTURE_H

#define DEFAULT_DESCRIPTION ""
#include "helpers.h"
class picture{
	/* This class provides storage and operations for each image.
	 * The constructor creates a picture object, and loads the image from the file
	 * display(x, y, z, angle, [selected]) renders the image on the screen at the coords
	 * and rotated at the specifyed angle(about it's own center)
	 */
public:

	GLuint loadTexture(string);
	GLfloat texWidth;
	GLfloat texHeight;
	GLuint texture;
	bool loaded;
	string filename;
	string description;
	bool hasDescription;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat angle;
	GLfloat width;
	GLfloat height;
	bool highlighted;
	//constructors.
	picture(string filename, GLfloat inWidth, GLfloat inHeight, string description = DEFAULT_DESCRIPTION);

	picture(string filename, string description = DEFAULT_DESCRIPTION){
		picture(filename, 0, 0, description);
	}
	void display(GLfloat lx, GLfloat ly, GLfloat lz, GLfloat langle, bool lhighlighted = false){
		x = lx;
		y = ly;
		z = lz;
		angle = langle;
		highlighted = lhighlighted;
		display();
	}
	void display();
};
#endif