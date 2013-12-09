//see pictures.h for comments
#include "picture.h"

picture::picture(string inFilename, GLfloat inWidth, GLfloat inHeight, string inDescription){
	if (strcmp(inDescription.c_str(), "") == 0){
		hasDescription = false;
	} else {
		hasDescription = true;
	}
	errno = 0;
#ifdef DEBUG	
	cout << "->Constructing texture object for " << inFilename << ": Loading...";
#endif
	description = string(inDescription);
	filename = string(inFilename);
	
	width = inWidth;
	height = inHeight;

	loaded = false;
	x = 0;
	y = 0;
	z = 0;
	highlighted = false;
	
	texture = loadTexture(filename.c_str());
	if (texture <= 0) {
#ifdef DEBUG
		cout << "Error " << errno << " on image " << texture << ": " << strerror(errno) <<"\n";
#endif
		glDisable(GL_TEXTURE_2D);
	} else {
#ifdef DEBUG
		cout << "Done! Storing[" << texture << "]...";
#endif
		unsigned char* img;
		int w, h, channels;

		img = SOIL_load_image(filename.c_str(), &w, &h, &channels, SOIL_LOAD_AUTO);
		SOIL_free_image_data(img);

		texWidth = w;
		texHeight = h;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (width == 0) width = texWidth;
		if (height == 0) height = texHeight;
#ifdef DEBUG
		cout << "Success!\n";
		cout << "Picture Information: \n";
		cout << " - width: " << width << ", height: " << height << "\n";
		cout << " - description: " << description << "\n\n";
		cout << "Success!\n";
#endif
		loaded = true;
	}
}

void picture::display(){
	if (loaded) { //doesn't display anything if a pic failed to load
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glLoadIdentity();
		glTranslatef(x, y - height, z);
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
			glVertex3f(width, -height, 0);
		glEnd();

		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//highlight picture if need be
		if (highlighted) {
			glPushMatrix();
			glTranslatef(x, y - height, z);
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

GLuint picture::loadTexture(string filename) {
	GLuint tex_ID = SOIL_load_OGL_texture(filename.c_str(), 
										  SOIL_LOAD_AUTO,
										  SOIL_CREATE_NEW_ID, 
										  SOIL_FLAG_POWER_OF_TWO
										  | SOIL_FLAG_DDS_LOAD_DIRECT);
	return tex_ID;
}
