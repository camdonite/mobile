//see helpers.h for comments
#include "helpers.h"

bool coinFlip(){
	//returns true or false
	return (rand() % 2) == 0;
}
double toRadian(double in){
	return in * M_PI / 180;
}
double toDegree(double in){
	return in * 180 / M_PI;
}
void renderBitmapString(double x, double y, double z, void *font, string str, double drop, int max) {
	int chars = 1;
	int newlines = 1;
	glRasterPos3d(x,y - (newlines * drop),z);
	for (string::iterator c = (&str)->begin(); c != (&str)->end(); ++c) {
		if (chars == max) break;
		chars ++;
		char chr = *c;
		if (chr == '\n') { 
			newlines ++;
			glRasterPos3d(x,y - (newlines * drop),z);	
		}else{
			glutBitmapCharacter(font, chr);
		}
	}
}
void coordinates(GLfloat size) {
	glLineWidth(1);
	glBegin(GL_LINES);
		// X coordinate line
		glColor3f(0, 0, 1);
		glVertex3f(size, 0, 0);
		glVertex3f(-1 * size, 0, 0);

		// Y coordinate line
		glColor3f(1, 0, 0);
		glVertex3f(0, size, 0);
		glVertex3f(0, -1 * size, 0);

		// Z coordinate line
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, size);
		glVertex3f(0, 0, -1 * size);

	glEnd();
	glColor3f(0, 0, 1);
	glRasterPos3f(size + 20, 0, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'X');
	glColor3f(1, 0, 0);
	glRasterPos3f(0, size + 20, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'Y');
	glColor3f(0, 1, 0);
	glRasterPos3f(0, 0, size + 20);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'Z');
}