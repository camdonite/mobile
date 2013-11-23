#include "helpers.h"
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 600
#define DEFAULT_NAME "untitled"
#define DEFAULT_DESCRIPTION "no description"

class picture{
	/* This class provides storage and operations for each image.
	 * The constructor creates a picture object, and loads the image from the file
	 * display(x, y, z, angle, [selected]) renders the image on the screen at the coords
	 * and rotated at the specifyed angle(about it's own center)
	 */
private:
	string name;
	string description;
	//char name[100];
	//char description[1000];

	float texWidth;
	float texHeight;
	GLuint texture;
	bool loaded;
public:
	GLfloat width;
	GLfloat height;
	//constructors
	//picture(string filename);
	//picture(string filename, GLfloat width, GLfloat height);
	//picture(string filename, string name, string description); //filename, name, description
	picture(string filename, GLfloat width, GLfloat height, string name = DEFAULT_NAME, string description = DEFAULT_DESCRIPTION);
	void display(GLfloat, GLfloat, GLfloat, GLfloat, bool highlighted = false);
	//void display(float, float, float, float, float, float, float, float);
};