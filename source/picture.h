#include "helpers.h"
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 600
#define DEFAULT_NAME "untitled"
#define DEFAULT_DESCRIPTION "no description"

class picture{
private:
	string name;
	string description;
	//char name[100];
	//char description[1000];

	float texWidth;
	float texHeight;
	GLuint texture[1];
	bool loaded;
public:
	GLfloat width;
	GLfloat height;
	//constructors
	//picture(string filename);
	//picture(string filename, GLfloat width, GLfloat height);
	//picture(string filename, string name, string description); //filename, name, description
	picture(string filename, GLfloat width, GLfloat height, string name = DEFAULT_NAME, string description = DEFAULT_DESCRIPTION);
	void display(GLfloat, GLfloat, GLfloat, GLfloat);
	//void display(float, float, float, float, float, float, float, float);
};