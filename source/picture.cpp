#include "picture.h"

//picture::picture(string filename){
//	picture(filename, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_NAME, DEFAULT_DESCRIPTION);
//}
//
//picture::picture(string filename, string inName, string inDescription){
//	picture(filename, DEFAULT_WIDTH, DEFAULT_HEIGHT, inName, inDescription);
//}
//
//picture::picture(string filename, GLfloat inWidth = DEFAULT_WIDTH, GLfloat inHeight = DEFAULT_HEIGHT){
//	picture(filename, inWidth, inHeight, "untitled", DEFAULT_DESCRIPTION);
//}

picture::picture(string filename, GLfloat inWidth, GLfloat inHeight, string inName, string inDescription){
	cout << "Constructing texture object for " << filename << ": Loading...";

	name = string(inName);
	//name = inName;
	description = string(inDescription);
	
	loaded = false;
	width = inWidth;
	height = inHeight;
	
	texture[0] = loadTexture(filename.c_str());
	if (texture[0] < 0) {
		cout << "Error on image " << texture[0] << ": " << errno << "\n";
		glDisable(GL_TEXTURE_2D);
	} else {
		cout << "Done! Storing[" << texture[0] << "]...";
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		cout << "Success!\n";
		loaded = true;
	}

}

void picture::display(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	if (loaded) {
	cout << "displaying pic " << name << "[" << texture[0] << "]!\n";
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glPushMatrix();
		glTranslatef(x, y, z); //change this
		glRotatef(angle, 0.0, 1.0, 0.0); //change this

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(width, height, 0);
		
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-width, height, 0);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-width, -height, 0);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(height, -height, 0);
		glEnd();

		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
}
