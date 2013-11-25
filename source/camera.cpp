#include "camera.h"

camera::camera(GLdouble ifovy, GLdouble iwidth, GLdouble iheight, GLdouble izNear, GLdouble izFar) {
	upx = 0;
	upy = 1;
	upz = 0;
	fovy = ifovy;
	width = iwidth;
	height = iheight;
	zNear = izNear;
	zFar = izFar;
	currentPos.x = currentPos.y = currentPos.z = 0;
	currentPos.lookatX = currentPos.lookatY = currentPos.lookatZ = 0;
	toPos.x = toPos.y = toPos.z = 0;
	toPos.lookatX = toPos.lookatY = toPos.lookatZ = 0;
	fromPos.x = fromPos.y = fromPos.z = 0;
	fromPos.lookatX = fromPos.lookatY = fromPos.lookatZ = 0;
	framesLeft = 0;
	changed = true;
}
void camera::changePerspective(GLdouble ifovy, GLdouble iwidth, GLdouble iheight, GLdouble izNear, GLdouble izFar, bool t) {
	fovy = ifovy;
	width = iwidth;
	height = iheight;
	zNear = izNear;
	zFar = izFar;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, width/height, zNear, zFar);
	if (t) {
		gluLookAt(currentPos.x, currentPos.y, currentPos.z,
			  currentPos.lookatX, currentPos.lookatY, currentPos.lookatZ,
			  upx, upy, upz);
	}
}
void camera::copyStruct(cameraPos* from, cameraPos* to){
	from->x = to->x;
	from->y = to->y;
	from->z = to->z;
	from->lookatX = to->lookatX;
	from->lookatY = to->lookatY;
	from->lookatZ = to->lookatZ;
}
void camera::animate(int frames){
	framesLeft = frames;
	framesAnimate = frames;
	copyStruct(&currentPos, &fromPos);
}
void camera::setPos(GLfloat x, GLfloat y, GLfloat z, bool t){
	toPos.x = x;
	toPos.y = y;
	toPos.z = z;
	if (framesLeft <= 0) {
		currentPos.x = x;
		currentPos.y = y;
		currentPos.z = z;
	}
	changed = true;
	if (t) touch();
}
void camera::set(GLfloat x, GLfloat y, GLfloat z, GLfloat lookatX, GLfloat lookatY, GLfloat lookatZ, bool t){
	setPos(x, y, z);
	toPos.lookatX = lookatX;
	toPos.lookatY = lookatY;
	toPos.lookatZ = lookatZ;
	if (t) touch();
}
void camera::setPos(cameraPos position, bool t){
	copyStruct(&position, &toPos);
	if (framesLeft <= 0) {
		copyStruct(&position, &currentPos);
	}
	changed = true;
	if (t) touch();
}
void camera::touch(){
	if (changed) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluLookAt(currentPos.x, currentPos.y, currentPos.z,
				  currentPos.lookatX, currentPos.lookatY, currentPos.lookatZ,
				  upx, upy, upz);
		changed = false;
	}
}