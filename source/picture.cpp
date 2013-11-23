#include "picture.h"

picture::picture(string filename){
	cout << "Constructing texture object for " << filename.c_str() << ": Loading...";
	loaded = false;
	width = 600;
	height = 600;
	
	texture[0] = loadTexture(filename.c_str());
	if (texture[0] < 0) {
		cout << "Error on image " << texture[0] << ": " << errno << "\n";
		glDisable(GL_TEXTURE_2D);
	} else {
		cout << "Done! Storing[" << texture[0] << "]...";
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		texWidth = GL_TEXTURE_WIDTH / width * 100; // ?
		texHeight = GL_TEXTURE_HEIGHT / height * 100; // also ?
		cout << "Success!\n";
		loaded = true;
	}

}

void picture::display(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	//if (loaded) {
	cout << "displaying pic #" << texture[0] << "!\n";
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0); //change this
		glTranslatef(0.0, 500.0, 0.0); //change this
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x, y, z);
		
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-x, y, z);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-x, -y, z);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x, -y, z);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	//}
}
void picture::display(float x, float y, float z, float angle, float verse, float transX, float transY, float transZ) {
	GLuint pic_index = texture[0];
	if (pic_index > 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pic_index);
	} else glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotatef(angle*verse, 0.0, 1.0, 0.0);
	glTranslatef(transX, transY, transZ);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
		
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, y, z);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, -y, z);

		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, -y, z);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}