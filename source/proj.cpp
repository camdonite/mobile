/*===================================================================================
Zach Hudson
Folder: Hudson923
Term Project

PROGRAMMER:				Zach Hudson and Chase Coates
PATH:					
COURSE:					CSC 525/625
LAST MODIFIED DATE:		Dec. 8, 2013
DESCRIPTION:			3D Mobile
NOTE:					N/A
FILES:					proj.cpp, (hwProject.sln, ...)
IDE/COMPILER:			MicroSoft Visual Studio 2012
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on hwProject.sln	to OPEN the project
2.		Press Ctrl+F7					to COMPILE
3.		Press Ctrl+Shift+B				to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5					to EXECUTE
If you get compile errors, goto
Project->ProjectProject Properties->Configuration Properties->General->Project Defaults->Character Set
And make sure that it is on Not Set
This is an issue with a 3rd party library we used(tiny_dir.h)
===================================================================================*/

#include <sstream>
#include "helpers.h"
#include "picture.h"
#include "camera.h"
#include "tinydir.h"

#define editor_title "3D Picture Mobile"
#define DEFAULT_MANIFEST "./manifest.txt"
#define DEFAULT_FOLDER "./"
#define DEFAULT_TREE_FOLDER "./mobiletree"

#define NODE_FOLDER_LEFT "/1"
#define NODE_FOLDER_RIGHT "/2"

//animation speeds
#define FLY_SPEED 120 //how many frames the fly to will take
#define SPEED_RANGE 0.2 //pictures will rotate a max of SPEED_RANGE degrees per frame
#define WOBBLE 2 // This is how much of a wobble when looking at a picture in degrees
#define FRAMES_PER_CHAR 2 //how many frames between each character when displaying in the description
#define DEFAULT_FRAMERATE 60

//style properties
//#define VIEW_LOOKAT_MULTIPLIER -.8
#define VIEW_DISTANCE_MULTIPLIER 2
#define VIEW_ANGLE -.2
#define STICK_COLOR 1, .5, 0
#define STICK_WIDTH 3
#define TEXT_COLOR 0, 1, 0
#define TEXT_BACKGROUND 0, 0, 0, 0.6
#define HIGHLIGHT_COLOR 1, 1, 0
#define FONT_HEIGHT 50 //space between lines
#define TEXT_POS_X -.9 //text starting position from -1 to 1. 0 is the middle of the screen
#define TEXT_POS_Y -.2 
#define FONT GLUT_BITMAP_TIMES_ROMAN_24
#define DROP_DISTANCE -230  // For the mobile drop distance for each hanging/raised(if > 0) piece
#define NODE_SPACE 100

#define WINDOW_X 600
#define WINDOW_Y 0
int height = 600,
	width  = 1000;

//camera stuff
const int depth = 600;
double distanceMultiplier = 5;
camera cam(70.0, 1000, 600, 1, depth*distanceMultiplier*1000); //initalize the camera object
//cameraPos initialPos = {0, -500, depth*distanceMultiplier*1.5, 0, -1500, 0};
cameraPos initialPos = {0, 0, depth*distanceMultiplier*1.5, 0, -1000, 0};

bool //showHelp = false,
	 showCoords = false,
	 tracking = false,
	 definedTree = false,
	 spinning = true;

using namespace std;

static GLbitfield bitmask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
static int FPS = DEFAULT_FRAMERATE;

//for text animation
static int currentChars = 0;
static int framesSinceLastChar = 0;

picture* lookat;
picture* selected;
int pointingToAbyss = 0;

int picCount = 0,
	picLookat = -1;
picture* pics[100];

#ifdef BENCHMARK
struct timeStat{
	WORD num;
	WORD sum;
	timeStat(){
		num = 0;
		sum = 0;
	}
};
timeStat timestat;
#endif

GLfloat random(){
	//Returns a random value based on the defines
	GLfloat out = ((GLfloat) rand() / RAND_MAX) * (SPEED_RANGE * 2) - SPEED_RANGE;
#ifdef DEBUG
	cout<<"->Random:"<<out<<"\n";
#endif
	return out;
}

struct mouseClick{
	//Provides data for mouse selecting a picture
	bool clicked;
	int x;
	int y;
	GLfloat lastZ;
	picture* lastPic;
	mouseClick(){
		clicked = false;
		x = 0;
		y = 0;
		lastZ = 0;
		lastPic = NULL;
	}
};
mouseClick click;

struct treeNode{
	//Tree node. Can have left and/or right children, or a picture if it is a leaf
	picture* pic;
	treeNode* right;
	treeNode* left;
	GLfloat xpos;
	GLfloat ypos;
	GLfloat zpos;
	GLfloat angle;
	GLfloat radius;
	GLfloat speed;
	treeNode(GLfloat y, GLfloat r){
		pic = NULL;
		right = NULL;
		left = NULL;
		xpos = 0.0;
		ypos = y;
		zpos = 0.0;
		angle = 0.0;
		speed = random();
		radius = r;
	}
	treeNode() {
		treeNode(0.0, 0.0);
	}
};
treeNode* root;

void reshape(int w, int h){
	cam.changePerspective(70.0, w, h, 1, depth*distanceMultiplier*1000);
}
void track(){
	double largestDimention = (lookat->height > lookat->width) ? lookat->height : lookat->width;
	double distance = largestDimention * 1.5;
	cam.set(lookat->x - (distance * sin(toRadian(lookat->angle + 180 + WOBBLE))),
			lookat->y - lookat->height,
			lookat->z - (distance * cos(toRadian(lookat->angle + 180 - WOBBLE))),
			lookat->x, lookat->y - lookat->height, lookat->z, false);
}
void displayDescription() {
	//Set the matricies to absolute coords
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Set blending functions
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(TEXT_BACKGROUND);

	//render the semi-transparent background
	glBegin(GL_POLYGON);
		glVertex3f(-1, TEXT_POS_Y, 0);	
		glVertex3f(1, TEXT_POS_Y, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(-1, -1, 0);
	glEnd();

	//render the text
	glColor3f(TEXT_COLOR);
	framesSinceLastChar ++;
	if (framesSinceLastChar >= FRAMES_PER_CHAR) {
		framesSinceLastChar = 0;
		currentChars ++;
	}
	renderBitmapString(TEXT_POS_X, TEXT_POS_Y, 0, FONT, lookat->description, FONT_HEIGHT / cam.height, currentChars);

	//reset blending
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	//reset the matricies
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void drawTree(treeNode* tree) {	
	//This is all recursive up in here!
	if (tree->pic) {
		//leaf(with picture)
		//Render the picture
		if (tree->pic != NULL) tree->pic->display(tree->xpos, tree->ypos, tree->zpos, tree->angle, tree->pic == selected);
	
		//Find the new z buffer value at the mouse coord, if it has changed, then this picture is the one the mouse is over
		GLfloat newClickDepth;
		glReadPixels(click.x, click.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &newClickDepth);
		//cout<<newClickDepth<<":"<<click.lastZ<<"|lookat"<<click.lastPic<<"\n";
		if (newClickDepth < click.lastZ) {
			click.lastPic = tree->pic;
			click.lastZ = newClickDepth;
		}
		//cout<<newClickDepth<<":"<<click.lastZ<<"|lookat"<<click.lastPic<<"\n";
  
		click.lastZ = newClickDepth;
	} else {
		GLfloat x1 = tree->xpos + (cos(toRadian(tree->angle)) * tree->radius);
		GLfloat z1 = tree->zpos + (sin(toRadian(tree->angle)) * tree->radius);
		GLfloat x2 = tree->xpos + (cos(toRadian(tree->angle + 180)) * tree->radius);
		GLfloat z2 = tree->zpos + (sin(toRadian(tree->angle + 180)) * tree->radius);
		glColor3f(STICK_COLOR);
		
		//draw the sticks
		glLineWidth(STICK_WIDTH);
		glBegin(GL_LINES);		
			if (tree->left != NULL) {
				glVertex3f(x1, tree->left->ypos, z1);
				glVertex3f(x1, tree->ypos, z1);
			}

			glVertex3f(x1, tree->ypos, z1);
			glVertex3f(x2, tree->ypos, z2);
	
			if (tree->right != NULL) {
				glVertex3f(x2, tree->ypos, z2);
				glVertex3f(x2, tree->right->ypos, z2);
			}
		glEnd();

		//process left subtree
		if (tree->left) {
			tree->left->xpos = x1;
			tree->left->zpos = z1;
			drawTree(tree->left);
		}
		//process right subtree
		if (tree->right) {
			tree->right->xpos = x2;
			tree->right->zpos = z2;
			drawTree(tree->right);
		}
	}
	tree->angle += tree->speed; //add speed to the angle each fram
	//check for angle overflow
	if (tree->angle > 360) {
		tree->angle -= 360;
	}
	if (tree->angle < 0) {
		tree->angle += 360;
	}
}
void redraw(){
	glClear(bitmask);
	//click.lastPic = nullptr;

	if (tracking) track();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (showCoords) coordinates(1000);

	click.lastZ = 1; // 1 is the abyss
	
	drawTree(root);
	
	if (click.lastZ == 1) {
		selected = nullptr;
	} else {
		selected = click.lastPic;
	}

	cam.touch();
	if (click.clicked) {
		tracking = true;
		click.clicked = false;
		currentChars = 1;
		framesSinceLastChar = 0;
	}

	if (tracking && cam.framesLeft <= 0){
		//glDisable(GL_DEPTH_TEST);
		//lookat->display();
		//glEnable(GL_DEPTH_TEST);		
		if (lookat->hasDescription) {
			displayDescription();
		}
	}
	glFlush();
	glutSwapBuffers();
}
void myDisplayCallback(){
	//don't put stuff here, have everything redraw off the timer tick
	// prevents window refreshes from making the thing spin out of control
}
void constructRandomTree(){
	//construct the tree	
	const int nodeWidth = NODE_SPACE;
	root = new treeNode(0, nodeWidth);  // Main root

	//add the first pic
	if (picCount > 0) {
		root->left = new treeNode(DROP_DISTANCE, nodeWidth);
		root->left->pic = pics[0];
		root->radius += pics[0]->width;
	}
	//add the second pic
	if (picCount > 1) {
		root->right = new treeNode(DROP_DISTANCE, nodeWidth);
		root->right->pic = pics[1];
		root->radius += pics[1]->width;
	}
	//add the subsequent pics
	for (int i = 2; i < picCount; i++){
#ifdef DEBUG
		cout<<"Adding pics["<<i<<"] "<<pics[i]->filename<<"\n";
#endif
		int treeHeight = 0;
		bool leafNotFound = true;
		
		treeNode* leafNode = root;
		
		while (leafNotFound) {
			treeHeight ++;
			
			leafNode->radius += pics[i]->width / 2;
			if (leafNode->pic == NULL) {				
				leafNode = (coinFlip()) ? leafNode->left : leafNode->right;
			} else {
				//Found a leaf. Replace leaf with node, and move the found pic to the left.
				leafNode->left = new treeNode(DROP_DISTANCE * treeHeight, nodeWidth);
				leafNode->radius += leafNode->pic->width / 2;
				leafNode->left->pic = leafNode->pic;
				leafNode->pic = NULL;
				
				//Add current pics[i] to right side
				leafNode->right = new treeNode(DROP_DISTANCE * treeHeight, nodeWidth);
				leafNode->right->pic = pics[i];
				leafNotFound = false;
			}
		}
	}
}
void keyboardCallback(unsigned char key, int cursorX, int cursorY) {
	key = tolower(key);
	switch (key) {
	case '.':
		cam.animate(FLY_SPEED);
		if (picLookat < picCount - 1) {
			picLookat ++;
			lookat = pics[picLookat];		
			click.clicked = true; //cause the animation to start
		}
		break;
	case ',':
		cam.animate(FLY_SPEED);
		if (picLookat > 0) {
			picLookat --;
			lookat = pics[picLookat];
			click.clicked = true; //cause the animation to start
		}
		break;
	case 'r':
		if (!definedTree) constructRandomTree();
		break;
	case 't':
#ifdef BENCHMARK
		cout << "Avg render time of last " << timestat.num << " frames is " << (timestat.sum / timestat.num) << " milliseconds.\n";
		timestat.sum = 0;
		timestat.num = 0;
#endif
		break;

	case '+':
		FPS += 10;
	case '-':
		FPS -= 5;
		if (FPS <= 0) FPS = 5;
		cout<<"Framerate(FPS):"<<FPS<<"\n";
		break;
	case 'b'://this one is for fun
		bitmask ^= GL_COLOR_BUFFER_BIT;
		if (bitmask & GL_COLOR_BUFFER_BIT) {
			cout<<"Clearing buffer(normal)\n";
		} else {
			cout<<"No longer clearing buffer\n";
		}
		break;
	case 'd'://also for fun
		if (glIsEnabled(GL_DEPTH_TEST)) {
			glDisable(GL_DEPTH_TEST);
			cout<<"Disabled Depth Test\n";
		} else {
			glEnable(GL_DEPTH_TEST);
			cout<<"Enabled Depth Test\n";
		}
		break;
	case ' ':
		redraw();
		break;
	case 's':
		if (spinning) {
			cout << "Stopping Animation(use space to advance frame)\n";
			spinning = false;
		} else {
			cout << "Starting Animation\n";
			spinning = true;
		}
		break;
	case 'c': 
		if (showCoords) {
			cout<<"Hiding Coordinate Axis\n";
			showCoords = false;
		} else {
			cout<<"Showing Coordinat Axis\n";
			showCoords = true;
		}
		break;
	case 'q': 
	case 27://escape
		exit(EXIT_SUCCESS);
		cout<<"Bye!\n";
		break;
	default: 
		//movement keys
		const int moveamt = 100;
		switch (key) {		
			case '1'://back
				initialPos.z += moveamt;
				break;
			case '4'://forth
				initialPos.z -= moveamt;
				break;
			case '2'://up
				initialPos.y -= moveamt;
				break;
			case '5'://down
				initialPos.y += moveamt;
				break;
			case '3'://lookat up
				initialPos.lookatY -= moveamt;
				break;
			case '6'://lookat down
				initialPos.lookatY += moveamt;
				break;

		}
		cam.set(initialPos);
		cout<<"Camera Z:"<<initialPos.z<<" Y:"<<initialPos.y<<" LookatY:"<<initialPos.lookatY<<"\n";
		break;
	}
}
void mouseCallback(int buttonName, int state, int cursorX, int cursorY) {
#ifdef DEBUG
	cout<<"->MouseClick X:"<<cursorX<<" Y:"<<cursorY<<"\n";
#endif
	cam.animate(FLY_SPEED);
	if (buttonName == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (tracking) {
			cam.set(initialPos, true);
			tracking = false;
		} else if (click.lastPic != nullptr) {
			click.clicked = true;
			click.x = cursorX;
			click.y = width - cursorY;
			lookat = click.lastPic;
		}
	}
}
void timer(int v) {
#ifdef BENCHMARK
	SYSTEMTIME before, after;
	GetSystemTime(&before);
#endif
	if (spinning) redraw();
#ifdef BENCHMARK
	GetSystemTime(&after);
	WORD frameTime = after.wMilliseconds - before.wMilliseconds;
	if (before.wSecond < after.wSecond) frameTime += 1000; //in case frame renders over a second boundry
	timestat.sum += frameTime;
	timestat.num ++;
#endif
	glutTimerFunc(1000/FPS, timer, v);	
}
void passiveMove(int cursorX, int cursorY) {
	if (!tracking) {
		click.x = cursorX;
		click.y = (int) cam.height - cursorY;
	}
}
char* parseTextFile(const char *path, GLfloat* inwidth, GLfloat* inheight) {
	string text;
	string temp;
  
	ifstream file;
	file.open (path);
	if (file.is_open()) {		
		if (file.peek() == '#'){
			file.get();
			file>>*inwidth;
			file>>*inheight;
		}
		while (!file.eof()) {
			
			getline(file, temp);
			text.append(temp);
			text.append("\n");
		}
		file.close();
		char * ret = new char[strlen(text.c_str())+1]();
		strcpy(ret, text.c_str());
		return ret;
#ifdef DEBUG
	} else {
		cout << " ( ! ) Missing file: " << path << " ( ! )\n\n";
#endif
	}
	return "";
}
int searchDirectory(const char *path, treeNode *leaf, float depth) {
	tinydir_dir dir;
	tinydir_open(&dir, path);
	if (!dir.has_next) return 0;

	bool hasLeftNode = false;
	bool hasLeftPic = false;
	bool nodeNotFull = true;
	depth += DROP_DISTANCE;

	while (dir.has_next && nodeNotFull) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (file.is_dir) {
			if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
				if (hasLeftNode)  {//go Right
					leaf->right = new treeNode(depth, NODE_SPACE);
					leaf->radius += searchDirectory(file.path, leaf->right, depth) / 2; // Check right
					nodeNotFull = false;
				} else { //go Left
					leaf->left = new treeNode(depth, NODE_SPACE);
					leaf->radius += searchDirectory(file.path, leaf->left, depth) / 2; // Check left
					hasLeftNode = true;
				}
			}
		} else {
			char fileName[256], extension[20];
			char textPath[4096];
			char* description;
			sscanf(file.name, "%[^.].%s", fileName, extension);
			if (strcmp(extension, "txt") != 0) {
				sprintf(textPath, "%s/%s%s", path, fileName, ".txt");
#ifdef DEBUG
				printf("->Preparing files:\n  %s\n  %s\n\n", file.path, textPath);
#endif
				GLfloat picWidth = 0;
				GLfloat picHeight = 0;
				description = (char*)parseTextFile(textPath, &picWidth, &picHeight);

				if (hasLeftNode) {				
					if (leaf->right == NULL) leaf->right = new treeNode(depth, NODE_SPACE);
					pics[picCount] = new picture(file.path, description, picWidth, picHeight);
					leaf->right->pic = pics[picCount];
					leaf->radius += pics[picCount]->width / 2;
					if (pics[picCount]->loaded) picCount ++;
					nodeNotFull = false;
				} else {
					if (leaf->left == NULL) leaf->left = new treeNode(depth, NODE_SPACE);
					pics[picCount] = new picture(file.path, description, picWidth, picHeight);
					leaf->left->pic = pics[picCount];
					leaf->radius += pics[picCount]->width / 2;
					if (pics[picCount]->loaded) picCount ++;
					hasLeftNode = true;
				}
			}
#ifdef DEBUG			
			printf("\n");
#endif
		}	
		tinydir_next(&dir);
	}
	tinydir_close(&dir);
	return leaf->radius;
}
bool constructMobileTree(const char* path) {
	root = new treeNode(0, NODE_SPACE);  // Main root

	root->left = new treeNode(DROP_DISTANCE, NODE_SPACE); // Left child
	string leftFolder = string(path);
	leftFolder.append(NODE_FOLDER_LEFT);
	int leftReturn = searchDirectory(leftFolder.c_str(), root->left, DROP_DISTANCE); //Check left
	root->radius += leftReturn;

	string rightFolder = string(path);
	rightFolder.append(NODE_FOLDER_RIGHT);
	root->right = new treeNode(DROP_DISTANCE, NODE_SPACE); // Right child
	int rightReturn = searchDirectory(rightFolder.c_str(), root->right, DROP_DISTANCE); // Check right
	root->radius += rightReturn;
	return !((leftReturn == 0) && (rightReturn == 0)); //if no sub folders are found, return false
}
bool loadFolder(const char* path){
	tinydir_dir dir;
	tinydir_open(&dir, path);
	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (!file.is_dir) {
			char fileName[256], extension[20];
			char textPath[4096];
			char* description;
			sscanf(file.name, "%[^.].%s", fileName, extension);
			if (strcmp(extension, "txt") != 0) {
				sprintf(textPath, "%s/%s%s", path, fileName, ".txt");
#ifdef DEBUG
				printf("->Preparing files:\n  %s\n  %s\n\n", file.path, textPath);
#endif
				GLfloat picWidth = 0;
				GLfloat picHeight = 0;
				description = (char*)parseTextFile(textPath, &picWidth, &picHeight);

				pics[picCount] = new picture(file.path, description, picWidth, picHeight);
				if (pics[picCount]->loaded) picCount ++;
			}
		}
		tinydir_next(&dir);
	}
	if (picCount > 0) return true; else return false;
	tinydir_close(&dir);
}
bool loadManifest(const char* manifestFilename){
	/* Loads the configuration data from the manifest file. 
	 * See example manifest.txt for more deets
	 * manifestFilename : filename of the config file
	 */
	
	ifstream file;
	file.open(manifestFilename);
	if (file.fail()) {
#ifdef DEBUG
		cout<<manifestFilename<<" can't be loaded.\n";
#endif
		return false;
	}
	string filename = "";
	GLfloat width;
	GLfloat height;
	string name;
	string description = "";
	string token;
	char ctoken[512];
	file>>token;
	while (!file.eof()) {		
		if (token == "#") {
			file>>width;
			file>>height;
			file.getline(ctoken, 512); //get filename
			file.getline(ctoken, 512); //Has to be 2. Dunno why.
			filename="";
			description="";
			filename.append(ctoken);
			while (!file.eof()) {
				//parse the description
				file>>token;
				if (token[0] == '#') break;
				description.append(token);
				file.getline(ctoken, 512);
				description.append(ctoken);
				description.append("\n");
			}
#ifdef DEBUG
			cout<<"pic["<<picCount<<"] filename:"<<filename<<"\n description:"<<description<<"\n";
#endif
			pics[picCount] = new picture(filename, description, width, height);
			if (pics[picCount]->loaded) picCount++;		
		}
	}
	file.close();
	return true;
}
void main(int argc, char ** argv){
	//initialize glut and openGL
	glutInit(& argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);			// specify a window position
	glutCreateWindow(editor_title);	// create a titled window
	//setup
	glEnable(GL_DEPTH_TEST); 
	cam.set(initialPos);
	cam.touch();

	srand(time(NULL));

	//figure out what to do
	if (argc == 1) {
		if (loadManifest(DEFAULT_MANIFEST)){
			constructRandomTree();
		} else if (constructMobileTree(DEFAULT_TREE_FOLDER)) {
		} else {
			loadFolder(DEFAULT_FOLDER);
			constructRandomTree();
		}
	} else {
		if (loadManifest(argv[1])){
			constructRandomTree();
		} else if (constructMobileTree(argv[1])) {
		} else {
			loadFolder(argv[1]);
			constructRandomTree();
		}
	}
	if (picCount <= 0) {
		cout<<"No pictures found";
	}
	initialPos.z = root->radius * VIEW_DISTANCE_MULTIPLIER;
	
	//initialPos.lookatY = root->radius * VIEW_LOOKAT_MULTIPLIER;
	initialPos.y = initialPos.lookatY + (root->radius * VIEW_ANGLE);
	
	//initialPos.lookatY = VIEW_LOOKAT_Y;
	//initialPos.y = VIEW_Y;
	
	cam.set(initialPos);

	glutKeyboardFunc(keyboardCallback);
	glutDisplayFunc(myDisplayCallback);		// register a callback
	glutTimerFunc(100, timer, 0);
	
	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(passiveMove);
	glutReshapeFunc(reshape);
	
	glutMainLoop();							// get into an infinite loop
}