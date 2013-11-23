//see pictures.h for comments
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
#ifdef DEBUG	
	cout << "->Constructing texture object for " << filename << ": Loading...";
#endif
	name = string(inName);
	description = string(inDescription);
	
	loaded = false;
	width = inWidth;
	height = inHeight;
	
	texture = loadTexture(filename.c_str());
	if (texture < 0) {
#ifdef DEBUG
		cout << "Error on image " << texture << ": " << errno << "\n";
#endif
		glDisable(GL_TEXTURE_2D);
	} else {
#ifdef DEBUG
		cout << "Done! Storing[" << texture << "]...";
#endif
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
#ifdef DEBUG
		cout << "Success!\n";
#endif
		loaded = true;
	}

}

void picture::display(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool highlighted){
	if (loaded) { //doesn't display anything if a pic failed to load
#ifdef DEBUG
		cout << "->displaying pic " << name << "[" << texture << "]! DES:" << description << "\n";
#endif
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 0.0, 1.0, 0.0);

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

		//highlight picture if need be
		if (highlighted) {
			glPushMatrix();
			glTranslatef(x, y, z);
			glRotatef(angle, 0.0, 1.0, 0.0);
			glColor3f(1, 1, 0);
			glLineWidth(5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
				glVertex3f(width+1, height+1, 0);	
				glVertex3f(-width-1, height+1, 0);
				glVertex3f(-width-1, -height-1, 0);
				glVertex3f(width+1, -height-1, 0);
			glEnd();
			glPopMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}
