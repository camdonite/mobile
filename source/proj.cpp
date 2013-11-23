/*===================================================================================
Zach Hudson
Folder: Hudson923
Term Project

PROGRAMMER:				Zach Hudson and Chase Coates
PATH:					
COURSE:					CSC 525/625
LAST MODIFIED DATE:		Nov. 22, 2013
DESCRIPTION:			3D Mobile
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
#define DEFAULT_MANIFEST "C:\\temp\\project\\manifest.txt"

picture* pics[300];
int picCount = 0;
double myangle = 0;

static bool spinning = true;
static const int FPS = 5;
static GLfloat currentAngleOfRotation[4] = { 0, 30, 90, 180 };

//int picCount = 3;
//GLuint textures[3];
//
//string load_me[3] = { "C:\\temp\\project\\pics\\samp\\bard.jpg",
//					  "C:\\temp\\project\\pics\\samp\\batoro.png",
//					   "C:\\temp\\project\\pics\\samp\\sette.jpg"};

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

double camera[9] =  {0, 0, depth*distanceMultiplier*1.5, 0, 0, 0, 0, 1, 0};

float pictures [4][6] = { 100, 100, width + 20, 1, width-100, 1,
						  100, 100, width + 700, 2, 200, 1,
						  100, 100, width + 300, 1, -100, -1,
						  50,  50, width, 1, -width/2, 1 };

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
		//mypic[i]->display(pictures[i][0], pictures[i][1], pictures[i][2], 
		//		currentAngleOfRotation[i], pictures[i][5],
		//		0, pictures[i][4], 0);

		//if (i == followPicIndex) {
		//	highlightPicture(pictures[i][0], pictures[i][1], pictures[i][2], 
		//					 currentAngleOfRotation[i], pictures[i][5],
		//					 0, pictures[i][4], 0);
		//}
	}

}
void myDisplayCallback(){
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (tracking) cameraTrack();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//redraw();

	myangle += 5;
	if (myangle > 360) myangle = 1;
	coordinates(500);

	pics[0]->display(1000.0, 0.0, 1000.0, myangle);

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
void loadManifest(const char* manifestFilename){
	/* Loads the configuration data from the manifest file. 
	 * See example manifest.txt for more deets
	 * manifestFilename : filename of the config file
	 * NOTES: This whole thing could be better.
	 */

	//known bug: will crash if comment is at the end
	ifstream file;
	file.open(manifestFilename);
	string filename;
	GLfloat width;
	GLfloat height;
	string name;
	string description;
	string token;
	while (!file.eof()) {
		file>>token;
		if (token == "/*") {
			//itereate over comments
			while (true){
				file>>token;
				if (token == "*/") break;
			}
			file>>token;
		}
		filename = token;
		file>>width>>height>>name>>description;
#ifdef DEBUG
		cout<<"->IDX:"<<picCount<<"FN:"<<filename<<" W:"<<width<<" H:"<<height<<" NAME:"<<name<<" DESCRIPTION:"<<description<<"\n"; //debug
#endif
		pics[picCount] = new picture(filename, width, height, name, description);
		picCount ++;
	}
}
void main(int argc, char ** argv){
	glutInit(& argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(600, 0);			// specify a window position
	glutCreateWindow(editor_title);	// create a titled window
	myInit();									// setting up
	
	if (argc < 2) {
		//no command line parameters given
		loadManifest(DEFAULT_MANIFEST);
	} else {
		loadManifest(argv[1]);
	}

	glutKeyboardFunc(keyboardCallback);
	glutDisplayFunc(myDisplayCallback);		// register a callback
	//glutTimerFunc(100, timer, 0);
	
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMovement);
	glutPassiveMotionFunc(passiveMove);
	glutMainLoop();							// get into an infinite loop
}
