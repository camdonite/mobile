//see helpers.h for comments
#include "helpers.h"

double toRadian(double in){
	return in * M_PI / 180;
}

double toDegree(double in){
	return in * 180 / M_PI;
}

GLuint loadTexture(string filename) {
	GLuint tex_ID = SOIL_load_OGL_texture(filename.c_str(), 
										  SOIL_LOAD_AUTO,
										  SOIL_CREATE_NEW_ID, 
										  SOIL_FLAG_POWER_OF_TWO
										  | SOIL_FLAG_DDS_LOAD_DIRECT);
	return tex_ID;
}

void renderBitmapString(double x, double y, void *font, string str) {
	int newlines = 1;
	glRasterPos2d(x,y);
	for (string::iterator c = (&str)->begin(); c != (&str)->end(); ++c) {
		if (*c == '\n' || *c == '|') { 
			glRasterPos2d(x,y + (newlines * 15));
			newlines ++;
		}else{
			glutBitmapCharacter(font, *c);
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