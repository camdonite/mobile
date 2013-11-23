/*===================================================================================
Zach Hudson
Folder: Hudson923
Project Sampler

PROGRAMMER:				Zach Hudson
PATH:					
COURSE:					CSC 525/625
LAST MODIFIED DATE:		Nov. 12, 2013
DESCRIPTION:			Demo: 3D Mobile sampler using SOIL
NOTE:					N/A
FILES:					proj.cpp, (hwProject.sln, ...)
IDE/COMPILER:			MicroSoft Visual Studio 2012
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on hwProject.sln	to OPEN the project
2.		Press Ctrl+F7					to COMPILE
3.		Press Ctrl+Shift+B				to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5					to EXECUTE
===================================================================================*/

#include "helpers.h"
#include "picture.h"

#define editor_title "3D Mobile"

picture* mypic[3];

static bool spinning = true;
static const int FPS = 60;
static GLfloat currentAngleOfRotation[4] = { 0, 30, 90, 180 };

int picCount = 3;
GLuint textures[3];

string load_me[3] = { "C:\\temp\\project\\pics\\samp\\bard.jpg",
					  "C:\\temp\\project\\pics\\samp\\batoro.png",
					   "C:\\temp\\project\\pics\\samp\\sette.jpg"};

using namespace std;
int height = 600,
	width  = 600,
	depth = 600;

double distanceMultiplier = 5;

int globalX, globalY;
double angle = 0;

int followPicIndex = -1;

bool hideHelp = true,
	 hideCoord = true,
	 tracking = false;

double camera[9] =  {0, 500, depth*distanceMultiplier*1.5, 0, 0, 0, 0, 1, 0};

float pictures [4][6] = { 100, 100, width + 20, 1, width-100, 1,
						  100, 100, width + 700, 2, 200, 1,
						  100, 100, width + 300, 1, -100, -1,
						  50,  50, width, 1, -width/2, 1 };

void renderPictures(float x, float y, float z, float angle, float verse, float transX, float transY, float transZ, GLuint pic_index) {
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
void resetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera[0] = camera[1] = camera[3] = camera[4] = camera[5] = 0;
	camera[2] = depth*distanceMultiplier*1.5;
	gluPerspective(70.0, width/height, 1, depth*distanceMultiplier*1000);
	gluLookAt(camera[0], camera[1], camera[2], 
		      camera[3], camera[4], camera[5], 
			  camera[6], camera[7], camera[8]);
}
void cameraTrack() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, width/height, 1, depth*distanceMultiplier*1000);

	double z = pictures[followPicIndex][2]+1000;

	camera[0] = z * sin(currentAngleOfRotation[followPicIndex]*pictures[followPicIndex][5]*M_PI/180);
	camera[1] = pictures[followPicIndex][4];
	camera[2] = z * cos(currentAngleOfRotation[followPicIndex]*pictures[followPicIndex][5]*M_PI/180);
	camera[3] = 0;
	camera[4] = pictures[followPicIndex][4];
	camera[5] = 0;
	
	gluLookAt(camera[0], camera[1], camera[2], 
		      camera[3], camera[4], camera[5], 
			  camera[6], camera[7], camera[8]);
}
void highlightPicture(float x, float y, float z, float angle, float verse, float transX, float transY, float transZ) {
	glPushMatrix();
	glRotatef(angle*verse, 0.0, 1.0, 0.0);
	glTranslatef(transX, transY, transZ);
	glColor3f(1, 1, 0);
	glLineWidth(5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
		glVertex3f(x+1, y+1, z);	
	    glVertex3f(-x-1, y+1, z);
		glVertex3f(-x-1, -y-1, z);
		glVertex3f(x+1, -y-1, z);
	glEnd();
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void redraw(){
	if (!hideCoord)
		coordinates(500);
	for (int i = 0; i < 3; i++) {
		//old method
		//renderPictures(pictures[i][0], pictures[i][1], pictures[i][2], 
		//			   currentAngleOfRotation[i], pictures[i][5],
		//			   0, pictures[i][4], 0, 
		//			   textures[i%picCount]);

		//new method
		mypic[i]->display(pictures[i][0], pictures[i][1], pictures[i][2], 
				currentAngleOfRotation[i], pictures[i][5],
				0, pictures[i][4], 0);

		if (i == followPicIndex) {
			highlightPicture(pictures[i][0], pictures[i][1], pictures[i][2], 
							 currentAngleOfRotation[i], pictures[i][5],
							 0, pictures[i][4], 0);
		}
	}

}
void myDisplayCallback(){
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (tracking) cameraTrack();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	redraw();

	coordinates(500);
	//mypic[0]->display(90.0, 0.0, 0.0, 0.0);

	glFlush();
	glutSwapBuffers();
}
void translate(double x, double y) {
	myDisplayCallback();
}
void rotate(double rads) {
	myDisplayCallback();
}
void keyboardCallback(unsigned char key, int cursorX, int cursorY) {
	switch (key) {
		case '1': followPicIndex = 0;
				  break;
		case '2': followPicIndex = 1;
				  break;
		case '3': followPicIndex = 2;
				  break;
		case '4': followPicIndex = 3;
				  break;
		case 'r': followPicIndex = -1;
				  if (tracking) {
					  tracking = !tracking;
					  resetCamera();
				  }
				  break;
		case 'c': hideCoord = !hideCoord;
				  break;
		case 'q': exit(EXIT_SUCCESS);
				  break;
		default: break;
	}
}
void mouseCallback(int buttonName, int state, int cursorX, int cursorY) {
	if (buttonName == GLUT_LEFT_BUTTON && state == GLUT_UP && followPicIndex != -1) {
		tracking = !tracking;
		if (!tracking) resetCamera();
	}
}
void myInit(){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, width/height, 1, depth*distanceMultiplier*1000);
	gluLookAt(camera[0], camera[1], camera[2], 
		      camera[3], camera[4], camera[5], 
			  camera[6], camera[7], camera[8]);
}
void timer(int v) {
  if (spinning) {
	for (int i = 0; i < 4; i++) {
		currentAngleOfRotation[i] += pictures[i][3];
		if (currentAngleOfRotation[i] > 360.0) {
		  currentAngleOfRotation[i] -= 360.0;
		}
	}
    glutPostRedisplay();
  }
  glutTimerFunc(1000/FPS, timer, v);
}
void passiveMove(int cursorX, int cursorY) {
	// No use yet
}
void mouseMovement(int cursorX, int cursorY) {
	// No use yet
}
void loadImages() {
	cout << "Loading images...\n";
	glGenTextures(picCount, textures);
	for (int i = 0; i < picCount; i++) {
		textures[i] = loadTexture(load_me[i].c_str());

		if (textures[i] > 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			pictures[i][0] = GL_TEXTURE_WIDTH/width*100;
			pictures[i][1] = GL_TEXTURE_HEIGHT/height*100;
			cout << "Image: " << load_me[i].c_str() << " successfully loaded.\n";
		} else {
			cout << "Failure: " << load_me[i].c_str() << " : " << textures[i] << "\n";
			glDisable(GL_TEXTURE_2D);
		}

		//if (textures[i] > 0) {
		//	glEnable(GL_TEXTURE_2D);
		//	glBindTexture(GL_TEXTURE_2D, textures[i]);
		//	pictures[i][0] = GL_TEXTURE_WIDTH/width*100;
		//	pictures[i][1] = GL_TEXTURE_HEIGHT/height*100;
		//} else glDisable(GL_TEXTURE_2D);

		
	}
	cout << "Images loaded.\n";
}
void main(int argc, char ** argv){
	glutInit(& argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(600, 0);			// specify a window position
	glutCreateWindow(editor_title);	// create a titled window
	myInit();									// setting up
	
	//loadImages();
	mypic[0] = new picture("C:\\temp\\project\\pics\\samp\\bard.jpg");
	mypic[1] = new picture("C:\\temp\\project\\pics\\samp\\batoro.png");
	mypic[2] = new picture("C:\\temp\\project\\pics\\samp\\sette.jpg");

	glutKeyboardFunc(keyboardCallback);
	glutDisplayFunc(myDisplayCallback);		// register a callback
	glutTimerFunc(100, timer, 0);
	
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMovement);
	glutPassiveMotionFunc(passiveMove);
	glutMainLoop();							// get into an infinite loop
}
