#include "helpers.h"

class picture{
private:
	string name;
	string description;
	GLfloat width;
	GLfloat height;
	float texWidth;
	float texHeight;
	GLuint texture[1];
	bool loaded;
public:
	picture(string);
	void display(GLfloat, GLfloat, GLfloat, GLfloat);
	void display(float, float, float, float, float, float, float, float);
};