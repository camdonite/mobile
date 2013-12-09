#ifndef PICTURE_H
#define PICTURE_H

#define DEFAULT_DESCRIPTION ""
#define DEFAULT_WIDTH 0
#define DEFAULT_HEIGHT 0
#include "helpers.h"
class picture{
	/* This class provides storage and operations for each image.
	 * The constructor creates a picture object, and loads the image from the file
	 * display(x, y, z, angle, [selected]) renders the image on the screen at the coords
	 * and rotated at the specifyed angle(about it's own center)
	 */
public:

	GLuint loadTexture(string);
	float texWidth;
	float texHeight;
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
	picture(string filename, string description = DEFAULT_DESCRIPTION, GLfloat inWidth = DEFAULT_WIDTH, GLfloat inHeight = DEFAULT_HEIGHT);

	//picture(string filename, string description = DEFAULT_DESCRIPTION){
	//	

	//	picture(filename, 0, 0, description);
	//	cout<<"overloaded w:"<<width<<" h:"<<height<<"\n";
	//}

	//picture(string filename, GLfloat inWidth, GLfloat inHeight, string description = DEFAULT_DESCRIPTION){
	//	picture(filename, description);
	//}

	//picture(string filename, string description = DEFAULT_DESCRIPTION);

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