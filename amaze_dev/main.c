#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <util/readtex.h>

/*
 * Computer Graphics - Final project
 * aMaze
 *
 * Copyright 2004 Michiel Scholten
 */
#define PROGINFO "Project aMaze [ 2004-01-12 v0.2.03 | amaze@aquariusoft.org ]"

/*** Displaylists ***/
typedef enum
{
	MODEL_BOGUS, MODEL_PLAYER_ARM, MODEL_PLAYER_LEG, MODEL_PLAYER_BODY, MODEL_PLAYER_EYE, MODEL_WORLD
} displaylists;

/*** Menu-entries ***/
typedef enum
{
	MNU_NONE, MNU_QUIT, MNU_SMOOTH, MNU_FLAT, MNU_CHEATON, MNU_CHEATOFF, MNU_CENTER
} menus;

/*** Switches for output to console ***/
#define DEBUG_ON
#define INFO_ON
#define MAZEDEBUG_ON

#define FPS_ON
#define FOG_ON

#ifdef DEBUG_ON
#define dprint printf
#endif
#ifndef DEBUG_ON
#define dprint (void)
#endif

#ifdef INFO_ON
#define iprint printf
#endif
#ifndef INFO_ON
#define iprint (void)
#endif

/*** Maze ***/
#define FILE_MAZE_BASEDIR "../models/maze/"
#define FILE_MAZE "default.maz"

/*** Texture defining stuff ***/
#define NUMBER_OF_TEXTURES 1

#define FILE_PLAYER_BASEDIR "../models/dino/"
char player_files[][40] = {"arm.sgf", "body.sgf", "eye.sgf", "leg.sgf"};
typedef enum
{
	player_arm, player_body, player_eye, player_leg
} modelIds;
#define PLAYER_NR_FILES 4

#define TEXTURES_BASEDIR "../textures/"
#define TEXT_REPTILE "reptile.rgb"
#define TEXT_GROUND "brick.rgb"
#define TEXT_WALL "rock.rgb"
RGBImage *pTexture_reptile;
RGBImage *pTexture_ground;
RGBImage *pTexture_wall;
/* Enumeration of textureIds. texture_bogus is because an id > 0 */
typedef enum
{
	texture_bogus, texture_reptile, texture_ground, texture_wall
} textureIds;

/*** Globally used variables ***/
/* Counting fps */
int frame=0,time,timebase=0;
int font=(int)GLUT_BITMAP_8_BY_13;
char s[30];

/* Variables about the world */
double window_h=0, window_w=0;

int world_h=0, world_w=0;
GLfloat zoomlevel = 5.0;
GLfloat world_leftright = 0.0f, world_forthback = 0.0f, world_rotation = 0.0f;
int mazeArray[100][100];	//ugly hack, because now it's always 100 big [lots of memory] and can't be bigger

/* About the player */
GLfloat player_x = 0.0, player_y=1.0;
typedef enum
{
	view_left, view_right, view_back, view_forth
} viewdirections;
int viewdirection = view_left;

GLfloat theta = 0;
int limbback = 0;

/* Switchers for animation and stuff */
int animate = 1;
int texturesEnabled = 1;

int cheating = 0;
#if 1
float textureMode = GL_MODULATE;
#endif
#if 0
float textureMode = GL_REPLACE;
#endif

/*** Define vertex ***/
typedef GLfloat point3[3];

/*** Struct to define material properties ***/
typedef struct
{
	float ambient[4];	/* params parameter in glMaterialfv call */
	float diffuse[4];
	float specular[4];
	float shininess;	/* Must be in range [0,128] */
} Material_t;


////////////////////////////////////// FPS display >
#ifdef FPS_ON
void setOrthographicProjection()
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, window_w, 0, window_h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1); 
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -window_h, 0); 
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection()
{
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{ 
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}
#endif
////////////////////////////////////// FPS display <

////////////////////////////////////// Cube settings >
/* Vertices */
point3 cube[8] =
{
	{-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0}, {-1.0,1.0,-1.0}, {-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}
};

/* Material */
Material_t cubeMaterial =
{
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialAmbient */
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialDiffuse */ 
	{0.3f, 0.3f, 0.3f, 1.0f},	/* materialSpecular */
	20				/* shininess */
}; 
////////////////////////////////////// Cube settings <

/* Colors used by the cubes */
point3 colors[6] =
{
	{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}
};

//point3 specularLight = {.5f, .5f, .5f};
point3 specularLight = {.8f, .8f, .8f};

////////////////////////////////////// Lights >
#if 0
const float LIGHT0_POS[] =  {2.0f, 4.0f, 2.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_SPECULAR[] = {0.3f, 0.3f, 0.3f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, 10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.2f, 0.2f, 0.2f, 1.0f};
#endif
const float LIGHT0_POS[] =  {0.0f, 8.0f, 0.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_SPECULAR[] = {0.3f, 0.3f, 0.3f, 1.0f};

const float LIGHT1_POS[] =  {14.0f, -1.5f, 1.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.2f, 0.2f, 0.2f, 1.0f};
////////////////////////////////////// Lights <

////////////////////////////////////// Rotating functions >
void rotateFunction(int value)
{
	if (animate)
	{
		/*
		 * Every 30msec, but only if the left mouse button is pushed and animation is enabled by user.
		 * Rotating once every 3sec
		 */
		if (limbback)
		{
			theta -= 3.0f;
			if (theta < -20) limbback = 0;
		} else
		{
			theta += 3.0f;
			if (theta > 20) limbback = 1;
		}
		glutTimerFunc(30, rotateFunction, 1);
	}
}
////////////////////////////////////// Rotating functions <

////////// Helper functions for player-walking >
/***
 * Loops through the maze file - starting at the bottom, the nearest to the
 * player, until a passageway is found, then set the player's coords to that unit
 ***/
int findFirstFreePlace()
{
	int i, j;
	for (i = world_h - 1; i >= 0; i--)
	{
		for (j = world_w - 1; j >= 0; j--)
		{
			//dprint("[%i][%i]=%i ", i, j, mazeArray[i][j]);
			if (mazeArray[i][j] == 0)
			{
				dprint("Found passageway at %i,%i\n", i, j);
				/* We found the first non-wall unit */
				player_x = j + 0.5f - (world_w*0.5);
				player_y = i + 0.5f - (world_h*0.5);
				dprint("player_x: %f player_y: %f\n", player_x, player_y);
				return 1;
			}
		}
		//dprint("\n");
	}
	/* We only found walls. Stupid maze :) */
	return 0;
}
/*** Centers the view on the player. Used when loading new map and for 3rd person following ***/
int centerViewOnPlayer()
{
	//
	return 1;
}

/*** Translates coords of player into units of the world [to check for walls] ***/
int getIndexX(GLfloat pl_coord, GLfloat distance)
{
	//iprint ("getIndexX: pl_coord: %f distance: %f world_w: %i\n", pl_coord, distance, world_w);
	return  (int)(pl_coord + distance + (world_w*0.5));
}

int getIndexY(GLfloat pl_coord, GLfloat distance)
{
	//iprint ("getIndexY: pl_coord: %f distance: %f world_h: %i\n", pl_coord, distance, world_h);
	return  (int)(pl_coord + distance + (world_h*0.5));
}
/*** Returns 1 if in maze and x,y is not a wall, otherwise zero ***/
int isValidLocation(int x, int y)
{
	if (x >= 0 && x < world_w && y >= 0 && y < world_h)
	{
		if (mazeArray[y][x] == 0) return 1;
		/* Or, with ghostmode on, we can walk through walls :) */
		if (cheating) return 1;
	}
	return 0;
}
////////// Helper functions for player-walking <

/*** Function for calculating a normal vector ***/
int calculateNormal(point3 t1, point3 t2, point3 t3, point3 normal)
{
	point3 v, v1, v2;
	float l;

	/*
	 * v1 = t2 - t1
	 * v2 = t3 - t1
	 * 
	 * v = v1 * v2
	 * v = [vx, vy, vz], where
	 *   vx = v1y * v2z - v1z * v2y
	 *   vy = v1z * v2x - v1x * v2z
	 *   vz = v1x * v2y - v1y * v2x
	 *       _____________________________
	 * l = \/ vx * vx + vy * vy + vz * vz
	 */
	v1[0] = t2[0] - t1[0];
	v1[1] = t2[1] - t1[1];
	v1[2] = t2[2] - t1[2];

	v2[0] = t3[0] - t1[0];
	v2[1] = t3[1] - t1[1];
	v2[2] = t3[2] - t1[2];

	v[0] = v1[1] * v2[2] - v1[2] * v2[1];
	v[1] = v1[2] * v2[0] - v1[0] * v2[2];
	v[2] = v1[0] * v2[1] - v1[1] * v2[0];

	l = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	
	normal[0] = v[0] / l;
	normal[1] = v[1] / l;
	normal[2] = v[2] / l;

	//dprint("normalVector: %f %f %f \n", normal[0], normal[1], normal[2]);
	return 1;
}

////////////////////////////////////// Drawing >
/*** Function to draw a cube, used for building the maze, which kinda exists out of cubes ***/
int drawCube(int texture)
{
	point3 normalVector;
	
	/* Texture stuff */
	if (texturesEnabled) glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, texture);

	/* Bottom */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[1], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[4]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[5]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[1]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[0]);
	glEnd();

	/* Front */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[4]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[5]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[6]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[7]);
	glEnd();

	/* Top */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[6]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[7]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[3]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[2]);
	glEnd();

	/* Back */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[3]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[3]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[1]);
	glEnd();

	/* Right */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[4]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[5]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[2]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[6]);
	glEnd();

	/* Left */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[5]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(cube[4]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(cube[0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(cube[3]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(cube[7]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	return 1;
}

/*** Draw the base of the world ***/
int drawBase(int rows, int columns)
{
	/* Texture stuff */
	if (texturesEnabled) glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, texture_ground);

	iprint("Drawing base width size of %i x %i\n", rows, columns);
	/* So, draw a nice square: */
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0 * rows);
		glVertex3f(-columns*0.5f, 0.0f, -rows*0.5f);
		glTexCoord2f(1.0*columns, 1.0 * rows);
		glVertex3f(-columns*0.5f, 0.0f,  rows*0.5f);
		glTexCoord2f(1.0*columns, 0.0);
		glVertex3f( columns*0.5f, 0.0f,  rows*0.5f);
		glTexCoord2f(0.0, 0.0);
		glVertex3f( columns*0.5f, 0.0f, -rows*0.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	return 1;	
}

/*** Load the maze datafile and draw it [ -> will be loaded into the world displaylist ] ***/
int loadNDrawMaze()
{
	int rows, columns, currentRow, block, thisBlock;
	int status;
	char filename[255];
	FILE *in;
#ifdef MAZEDEBUG_ON
	int i,j;
#endif

	filename[0] = '\0';
	strcat(filename, FILE_MAZE_BASEDIR);
	strcat(filename, FILE_MAZE);
	    
	iprint("[maze] Loading maze from \"%s\" ... \n", filename);
	if ((in = fopen(filename, "r")) == NULL)
	{
		iprint("[maze] Unable to open the file [file: %s]\n", filename);
		return 0;
	}

	/* Load stuff */
	//while (!feof(in)) // <- obsolete
	{
		status = fscanf(in, " %i %i \n", &rows, &columns);
		dprint("[maze] Maze has dimensions %i x %i\n", rows, columns);

		glPushMatrix();
			if (drawBase(rows, columns) < 1)
			{
				return 0;
			}
		glPopMatrix();

		//createMazeArray(rows, columns);

		if (status == EOF)
		{
			return EOF;
		}
		if (status < 2)
		{
			iprint("ERROR @ loadNDrawMaze :: Reading of dimensions failed!\n");
			return 0;
		}

		world_h = rows;
		world_w = columns;
		iprint("[maze] world_h:%i world_w:%i\n",world_h, world_w);
		
		/* Load vertices and draw the thing */
		for (currentRow = 0; currentRow < rows; currentRow++)
		{
			for (block = 0; block < columns; block++)
			{
				fscanf(in, " %i ", &thisBlock);
				if (thisBlock == 1)
				{
					dprint("W");
					//mazeArray[block][currentRow] = 1;
					mazeArray[currentRow][block] = 1;
					glPushMatrix();
						/*** Transpose the blocks to the right spots ***/
						glTranslatef(block - (columns*0.5)+0.5, 0.5f, currentRow - (rows*0.5)+0.5);
						/* Now make it a 1x1 unit [cube is 2x2x2] */
						glScaled(0.5,0.5,0.5);
						drawCube(texture_wall);
					glPopMatrix();

				} else
				{
					/* skip -> passage */
					mazeArray[currentRow][block] = 0;
					dprint(" ");
				}
			}
			fscanf(in, " \n");
			dprint("\n");
		}
	}
#ifdef MAZEDEBUG_ON
	dprint("format: [x][y]=<0|1>\n");
	for (i = 0; i < world_h; i++)
	{
		for (j = 0; j<world_w; j++)
		{
			dprint("[%i][%i]=%i ", j, i, mazeArray[i][j]);
		}
		dprint("\n");
	}
#endif
	return 1;
}
#if 0
int createMazeArray(int rows, int cols)
{
	static int mazeArray[rows][cols];
}
#endif

/*** Draw the player, default is the dino -> not customizable ***/
int drawPlayer()
{
	GLfloat scale = 0.07f;
	glPushMatrix();
		/* Translate to behind, because middlepoint is in the tail and we don't want that :) */
		glTranslatef(-0.75, 0.0, 0.0);
		glScaled(scale, scale, scale);
		
		glPushMatrix();
			/* Body > */
			glPushMatrix();
				glCallList(MODEL_PLAYER_BODY);
			glPopMatrix();
			/* Body < */
			/* Eye > */
			glPushMatrix();
				glTranslatef(0.0, 0.0, -0.08f);
				glCallList(MODEL_PLAYER_EYE);
			glPopMatrix();
			/* Eye < */
			/* Left arm > */
			glPushMatrix();
				glTranslatef(0.0, 0.0, -0.75f);
				glTranslatef( 10.0,  10.0, -1.0);
				glRotatef(-theta, 0.0, 0.0, 1.0);
				glTranslatef(-10.0, -10.0,  1.0);
				glCallList(MODEL_PLAYER_ARM);
			glPopMatrix();
			/* Left arm < */
			/* Right arm > */
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, 3.0f);
				glTranslatef( 10.0,  10.0, -2.0);
				glRotatef( theta, 0.0, 0.0, 1.0);
				glTranslatef(-10.0, -10.0,  2.0);
				glCallList(MODEL_PLAYER_ARM);
			glPopMatrix();
			/* Right arm < */
			/* Right leg > */
			glPushMatrix();
				glTranslatef(0.0, 0.0, 2.75f);
				glTranslatef( 10.0,  5.0, -2.0);
				glRotatef(-theta, 0.0, 0.0, 1.0);
				glTranslatef(-10.0, -5.0,  2.0);
				glCallList(MODEL_PLAYER_LEG);
			glPopMatrix();
			/* Right leg < */
			/* Left leg > */
			glPushMatrix();
				glTranslatef(0.0, 0.0, -1.25f);
				glTranslatef( 10.0,  5.0, -1.0);

				glRotatef( theta, 0.0, 0.0, 1.0);
				glTranslatef(-10.0, -5.0,  1.0);
				
				glCallList(MODEL_PLAYER_LEG);
			glPopMatrix();
			/* Left leg < */
		glPopMatrix();
	glPopMatrix();
	return 1;
}

/*** Render the whole lot ***/
void renderScene(void)
{
	int rotate, pl_x, pl_y;
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluPerspective(100, 1, 1, 100);
	gluLookAt(0.0, zoomlevel, zoomlevel,	// <- eye
		0.0, 0.0, 0.0,		// <- center
		0.0, 1.0, 0.0);		// <- up

#ifdef FPS_ON
	/* FPS stuff > */
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		pl_x = getIndexX(player_x, 0.0);
		pl_y = getIndexY(player_y, 0.0);
		     
		//sprintf(s,"FPS: %4.2f", frame * 1000.0 / (time-timebase));
		sprintf(s,"FPS:%4.2f x:%i y:%i", frame * 1000.0 / (time-timebase), pl_x, pl_y);
		//sprintf(s,"Time elapsed: %i | FPS:%4.2f", (timebase/1000), frame * 1000.0 / (time-timebase));
		timebase = time;		
		frame = 0;
	}

	glColor3f(0.0f,1.0f,1.0f);
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	renderBitmapString(30,35,(void *)font,s);
	glPopMatrix();
	resetPerspectiveProjection();
	/* FPS stuff < */
#endif

	glPushMatrix();
		/* Transpose the whole world to the left/right [done with arrow_left/-right] */
		glTranslatef(world_leftright, 0.0f, world_forthback);
		/* Rotation of the world */
		glRotatef(world_rotation, 0, 1, 0);

		/* Draw the base and maze > */
		glPushMatrix();
			glCallList(MODEL_WORLD);
		glPopMatrix();
		/* Draw the base and maze < */

		/* Draw the player > */
		glPushMatrix();
			/* Put the player on the right place in the maze */
			glTranslatef(player_x, 0.0f, player_y);
			/* Now rotate it to face the direction it's going */
			if (viewdirection == view_back) rotate = -90;
			if (viewdirection == view_forth) rotate = 90;
			if (viewdirection == view_left) rotate = 180;
			if (viewdirection == view_right) rotate = 0;//-180;
			glRotatef(rotate, 0, 1, 0);
			drawPlayer();
		glPopMatrix();
		/* Draw the player < */
	glPopMatrix();
	
	/* Finally, show the new frame */
	glutSwapBuffers();
}

void reshape_now(GLsizei w, GLsizei h)
{
	double whRatio = (double)h / (double)w;
	window_h = h;
	window_w = w;

	dprint("reshape_now called\n");

	glViewport(0, 0, w, h);
	dprint("reshape_now - whRatio: %f\n", whRatio);
	fflush(stdout);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.3, 0.3, -whRatio * 0.3, whRatio * 0.3, 0.3, 100);
	glMatrixMode(GL_MODELVIEW);
}
////////////////////////////////////// Drawing <

////////////////////////////////////// Loading stuff >
/*** Generic functions ***/
int loadModel(char *filename, int texture)
{
	int vertex, nrVertices;
	char identifier[20]; // string of maxlen 20
	GLfloat vertex_x, vertex_y, vertex_z, normal_x, normal_y, normal_z;
	int status;
	FILE *in;
	
	dprint("loadModel called\n");

	/* Texture stuff */
	if (texture > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_TEXTURE_GEN_S); glEnable(GL_TEXTURE_GEN_T);
	}
	
	iprint("[loadModel] Loading \"%s\" ... \n", filename);
	if ((in = fopen(filename, "r")) == NULL)
	{
		iprint("[loadModel] Unable to open the file [file: %s]\n", filename);
		return 0;
	}
	/* Load stuff */
	while (!feof(in))
	{
		status = fscanf(in, " %s %i \n", &identifier, &nrVertices);
		dprint(" - read type %s, nr of vertices is %i\n", identifier, nrVertices);

		if (status == EOF)
		{
			return EOF;
		}
		if (status < 2)
		{
			iprint("ERROR @ loadModel :: Reading of identifier and number failed!\n");
			return 0;
		}

		/* 
		 * Look which kind of figure we have to draw and start the figure
		 * Choose from POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, QUADS, TRIANGLE_STRIP, TRIANGLE_FAN, QUAD_STRIP or POLYGON
		 */
		if (strcmp(identifier, "POINTS") == 0)
		{
			glBegin(GL_POINTS);
		} else if (strcmp(identifier, "LINES") == 0)
		{
			glBegin(GL_LINES);
		} else if (strcmp(identifier, "LINE_STRIP") == 0)
		{
			glBegin(GL_LINE_STRIP);
		} else if (strcmp(identifier, "LINE_LOOP") == 0)
		{
			glBegin(GL_LINE_LOOP);
		} else if (strcmp(identifier, "TRIANGLES") == 0)
		{
			glBegin(GL_TRIANGLES);
		} else if (strcmp(identifier, "QUADS") == 0)
		{
			glBegin(GL_QUADS);
		} else if (strcmp(identifier, "TRIANGLE_STRIP") == 0)
		{
			glBegin(GL_TRIANGLE_STRIP);
		} else if (strcmp(identifier, "TRIANGLE_FAN") == 0)
		{
			glBegin(GL_TRIANGLE_FAN);
		} else if (strcmp(identifier, "QUAD_STRIP") == 0)
		{
			glBegin(GL_QUAD_STRIP);
		} else if (strcmp(identifier, "POLYGON") == 0)
		{
			glBegin(GL_POLYGON);
		} else
		{
			/* Return an error, or at least a -1 */
			iprint("ERROR @ loadModel :: unknown identifier!\n");
			return -1;
		}
		/* Load vertices and draw the thing */
		for (vertex = 0; vertex < nrVertices; vertex++)
		{
			fscanf(in, " %f %f %f %f %f %f\n", &vertex_x, &vertex_y, &vertex_z, &normal_x, &normal_y, &normal_z);
			glNormal3f(normal_x, normal_y, normal_z);
			glVertex3f(vertex_x, vertex_y, vertex_z);
			//glTexCoord2f(1.0, 0.0); //random placement of textures
		}
		glEnd();	/* Done with this figure */
	}
	if (texture > -1)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	iprint("[loadModel] done\n");
	return 1;
}

void textureGen( GLfloat i )
{
	GLfloat s[4] = { i, 0, i, 0 }; 
	GLfloat t[4] = { i, i, 0, 0 }; 

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
}

int loadTexture(char *filename, int texId, RGBImage *pTex)
{
iprint("loadTexture %s, texId = %i\n", filename, texId);
	pTex = LoadRGB(filename);
	glGenTextures(NUMBER_OF_TEXTURES, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// wrap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// wrap vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//target, level, components, width, height, border, format, type, *pixels
	glTexImage2D(GL_TEXTURE_2D, 0, pTex->components, pTex->sizeX, pTex->sizeY, 0, pTex->format, GL_UNSIGNED_BYTE, pTex->data);

	iprint("Texture \"%s\" loaded\n", filename);
	return 1;
}

int loadTextures()
{
	char filename[255];

	filename[0] = '\0';
	strcat(filename, TEXTURES_BASEDIR);
	strcat(filename, TEXT_REPTILE);
	loadTexture(filename, texture_reptile, pTexture_reptile);

	filename[0] = '\0';
	strcat(filename, TEXTURES_BASEDIR);
	strcat(filename, TEXT_GROUND);
	loadTexture(filename, texture_ground, pTexture_ground);

	filename[0] = '\0';
	strcat(filename, TEXTURES_BASEDIR);
	strcat(filename, TEXT_WALL);
	loadTexture(filename, texture_wall, pTexture_wall);

	return 1;
}

////////// Actual load functions >
/*** Load the player [dino] ***/
int loadPlayer(void)
{
	char filename[255];

	filename[0]='\0';	//reset the string
	strcat(filename, FILE_PLAYER_BASEDIR);
	strcat(filename, player_files[player_arm]);
	glNewList(MODEL_PLAYER_ARM, GL_COMPILE);
		loadModel(filename, texture_reptile);
	glEndList();

	filename[0]='\0';	//reset the string
	strcat(filename, FILE_PLAYER_BASEDIR);
	strcat(filename, player_files[player_leg]);
	glNewList(MODEL_PLAYER_LEG, GL_COMPILE);
		loadModel(filename, texture_reptile);
	glEndList();

	filename[0]='\0';	//reset the string
	strcat(filename, FILE_PLAYER_BASEDIR);
	strcat(filename, player_files[player_body]);
	glNewList(MODEL_PLAYER_BODY, GL_COMPILE);
		loadModel(filename, texture_reptile);
	glEndList();

	filename[0]='\0';	//reset the string
	strcat(filename, FILE_PLAYER_BASEDIR);
	strcat(filename, player_files[player_eye]);
	glNewList(MODEL_PLAYER_EYE, GL_COMPILE);
		loadModel(filename, -1);
	glEndList();

	return 1;
}

/*** Draw base and load maze, and put them into a displaylist ***/
int loadWorld(void)
{
	glNewList(MODEL_WORLD, GL_COMPILE);
		/* loadNDrawMaze draws the base too */
		if (loadNDrawMaze() < 1)
		{
			glEndList();
			return 0;
		}
	glEndList();
	return 1;
}
////////// Actual load functions <
////////////////////////////////////// Loading stuff <

////////////////////////////////////// Handlers >
void keyboard(unsigned char key, int x, int y)
{
	/* When q, Q or ESC is pressed, exit the program */
	if ( key == 'q' || key == 'Q' || key == 27 )
	{
		exit(0);
	}
	/* Space bar is used for starting and stopping the animation */
	if ( key == ' ' )
	{
		if ( animate )
		{
			animate = 0;
		} else
		{
			animate = 1;
			glutTimerFunc(30, rotateFunction, 0);
		}
	}
	if ( (key == '+' || key == '=') && zoomlevel > 0.5 )
	{
		//iprint("Adjusting zoomlevel: zoom in to %f\n", zoomlevel);
		zoomlevel -= 0.5;
	}
	if ( key == '-' && zoomlevel < 20 )
	{
		//iprint("Adjusting zoomlevel: zoom out to %f\n", zoomlevel);
		zoomlevel += 0.5;
	}
	if ( (key == ',' || key == '<') && world_leftright > (-world_w - 100) )
	{
		world_leftright += 0.5;
		//iprint("world_leftright = %f\n", world_leftright);
	}
	if ( (key == '.' || key == '>') && world_leftright < (world_w + 100) )
	{
		world_leftright -= 0.5;
		//iprint("world_leftright = %f\n", world_leftright);
	}
	if ( key == ';' || key == ':' )
	{
		world_rotation -= 2.0;
		if (world_rotation < 0) world_rotation += 360;
		//iprint("world_rotation = %f\n", world_rotation);
	}
	if ( key == '\'' || key == '"')
	{
		world_rotation += 2.0;
		if (world_rotation > 360) world_rotation -= 360;
		//iprint("world_rotation = %f\n", world_rotation);
	}
}


static void specialKeyFunc( int Key, int x, int y )
{
	int pl_x, pl_y, new;
	pl_x = getIndexX(player_x, 0.0);
	pl_y = getIndexY(player_y, 0.0);
	//iprint("x:%i y:%i\n",pl_x , pl_y);
	switch ( Key )
	{
		case GLUT_KEY_UP:
			/* Move player forwards */
			viewdirection = view_forth;
			new = getIndexY(player_y, -0.2f);
			//iprint("UP   %f %f\n", player_x, player_y);
			if (isValidLocation(pl_x, new))
			{
				player_y -= 0.2f;
			}
			break;
		case GLUT_KEY_DOWN:
			/* Move player backwards */
			viewdirection = view_back;
			new = getIndexY(player_y, 0.2f);
			//iprint("DOWN %f %f\n", player_x, player_y);
			if (isValidLocation(pl_x,new))
			{
				player_y += 0.2f;
			}
			break;
		case GLUT_KEY_LEFT:
			/* Move player to the left */
			viewdirection = view_left;
			new = getIndexX(player_x, -0.2f);
			//iprint("LEFT %f %f\n", player_x, player_y);
			if (isValidLocation(new,pl_y))
			{
				player_x -= 0.2f;
			}
			break;
		case GLUT_KEY_RIGHT:
			/* Move player to the right */
			viewdirection = view_right;
			new = getIndexX(player_x, 0.2f);
			//iprint("RIGH %f %f\n", player_x, player_y);
			if (isValidLocation(new,pl_y))
			{
				player_x += 0.2f;
			}
			break;
		case GLUT_KEY_PAGE_UP:
			/* Move world towards viewer */
			world_forthback += 0.5f;
			break;
		case GLUT_KEY_PAGE_DOWN:
			/* Move world away from viewer */
			world_forthback -= 0.5f;
			break;
	}
}

void mouse(int btn, int btn_state, int x, int y)
{
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		dprint("player_x:%f player_y:%f\n", player_x, player_y);
	}
#if 0
//nothing usefull left here
	/* Print the states of the mouse buttons */
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		mousedown = 1;
//		glutTimerFunc(30, rotatePyramid, 0);
	}
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_UP )
	{
		mousedown = 0;
	}
	if ( btn == GLUT_MIDDLE_BUTTON && btn_state == GLUT_DOWN )
	{
		printf(" :: middle button down\n");
	}
	if ( btn == GLUT_RIGHT_BUTTON && btn_state == GLUT_DOWN )
	{
		printf(" :: right button down\n");
	}
#endif
}

void handle_menu(int whichone)
{
	/* Handle the selected item */
	switch (whichone)
	{
		case MNU_FLAT:
			iprint("doing flat shading\n");
			glShadeModel(GL_FLAT);
			break;
		case MNU_SMOOTH:
			iprint("doing smooth shading\n");
			glShadeModel(GL_SMOOTH);
			break;
		case MNU_CHEATON:
			iprint("set cheating ON\n");
			cheating = 1;
			break;
		case MNU_CHEATOFF:
			iprint("set cheating OFF\n");
			cheating = 0;
			break;
		case MNU_QUIT:
			iprint("menu :: quit entry chosen, exiting\n");
			exit(0);
			break;
	}
}
////////////////////////////////////// Handlers <

////////////////////////////////////// main function and init >
int buildMazeMenu()
{
	return 1;
#if 0
	int rows, columns, currentRow, block, thisBlock;
	int status;
	char filename[255];
	FILE *in;
#ifdef MAZEDEBUG_ON
	int i,j;
#endif

	filename[0] = '\0';
	strcat(filename, FILE_MAZE_BASEDIR);
	strcat(filename, FILE_MAZE);
	    
	iprint("[maze] Loading maze from \"%s\" ... \n", filename);
	if ((in = fopen(filename, "r")) == NULL)
	{
		iprint("[maze] Unable to open the file [file: %s]\n", filename);
		return 0;
	}

	/* Load stuff */
	while (!feof(in)) // <- obsolete
	{
		status = fscanf(in, " %i %i \n", &rows, &columns);
		dprint("[maze] Maze has dimensions %i x %i\n", rows, columns);

		glPushMatrix();
			if (drawBase(rows, columns) < 1)
			{
				return 0;
			}
		glPopMatrix();

		//createMazeArray(rows, columns);

		if (status == EOF)
		{
			return EOF;
		}
		if (status < 2)
		{
			iprint("ERROR @ loadNDrawMaze :: Reading of dimensions failed!\n");
			return 0;
		}

		world_h = rows;
		world_w = columns;
		iprint("[maze] world_h:%i world_w:%i\n",world_h, world_w);
		
		/* Load vertices and draw the thing */
		for (currentRow = 0; currentRow < rows; currentRow++)
		{
			for (block = 0; block < columns; block++)
			{
				fscanf(in, " %i ", &thisBlock);
				if (thisBlock == 1)
				{
					dprint("W");
					mazeArray[block][currentRow] = 1;
					glPushMatrix();
						/*** Transpose the blocks to the right spots ***/
						glTranslatef(block - (columns*0.5)+0.5, 0.5f, currentRow - (rows*0.5)+0.5);
						/* Now make it a 1x1 unit [cube is 2x2x2] */
						glScaled(0.5,0.5,0.5);
						drawCube(texture_wall);
					glPopMatrix();

				} else
				{
					/* skip -> passage */
					mazeArray[block][currentRow] = 0;
					dprint(" ");
				}
			}
			fscanf(in, " \n");
			dprint("\n");
		}
	}
#endif
}

int main(int argc, char **argv)
{
	int submenu_mazes, submenu_shading, submenu_cheating;
#ifdef FOG_ON
	GLfloat color[4] = {0.70f, 0.70f, 0.70f, 1.00};
#endif

	/* If prog was called with param, only print info to console */
	if (argc > 1)
	{
		if (strcmp(argv[1], "--version") == 0)
		{
			//printf("aMaze version %s\n", VERSION);
			printf(PROGINFO);
		} else
		{
			printf("aMaze\n\nUsage: just run the executable. Everything will be loaded automagically\n\nParams:\n        --version    prints version info\n");
		}
		printf("\n%s\n", PROGINFO);
		return 0;
	}

	/*** Initialize glut and the window ***/
	glutInit(&argc, argv);
	/* Use rgb, double buffering and hidden surface removal */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	//glutCreateWindow("Project aMaze [ Michiel Scholten | mbscholt@cs.vu.nl | 1204467 ]");
	//glutCreateWindow("Project aMaze [ Michiel Scholten | mbscholt@aquariusoft.org ]");
	//glutCreateWindow("Project aMaze [ 2004-01-12 v0.2.02 | amaze@aquariusoft.org ]");
	glutCreateWindow(PROGINFO);


	/* Background color of the screen - "the sky" */
#ifdef FOG_ON
	glClearColor(0.7f, 0.7f, 0.7f, 1.0);
#endif
#ifndef FOG_ON
	//glClearColor(0.0f, 0.0f, 0.5f, 1.0f);	//darkblue
	glClearColor(0.0f, 0.5f, 0.9f, 1.0f);	//lighter blue
#endif

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);

	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_LIGHT_MODEL_AMBIENT);
	
	/* Light related settings */
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT0_POS);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LIGHT0_AMBIENT);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT0_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT0_SPECULAR);

	glLightfv(GL_LIGHT1, GL_POSITION, LIGHT1_POS);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT1_AMBIENT);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT1_DIFFUSE);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LIGHT1_SPECULAR);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, 1.0 );
	//glEnable(GL_COLOR_MATERIAL);
	//glShadeModel(GL_SMOOTH);
	//glDepthFunc(GL_LEQUAL);
	
	/* set various event callback functions */
	glutReshapeFunc(reshape_now);
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(renderScene);
	/* Enable special keys like GLUT_KEY_LEFT */
	glutSpecialFunc(specialKeyFunc);

	/* Load the texture[s] */
	loadTextures();
	
	/*** menu > ***/
	/* Build submenu with maze-switcher */
	submenu_mazes = glutCreateMenu(handle_menu);
	//glutAddMenuEntry("default",MNU_NONE);
	if (buildMazeMenu() < 1)
	{
		printf("Error while loading mazelist. Exiting...\n");
		return -1;
	}

	submenu_shading = glutCreateMenu(handle_menu);
	glutAddMenuEntry("flat",MNU_FLAT);
	glutAddMenuEntry("smooth",MNU_SMOOTH);

	submenu_cheating = glutCreateMenu(handle_menu);
	glutAddMenuEntry("on",MNU_CHEATON);
	glutAddMenuEntry("off",MNU_CHEATOFF);

	glutCreateMenu(handle_menu);
	glutAddSubMenu("mazes", submenu_mazes);
	glutAddMenuEntry("-----------------------", MNU_NONE);
	glutAddSubMenu("shading", submenu_shading);
	glutAddSubMenu("ghostmode", submenu_cheating);
	glutAddMenuEntry("center view to player",MNU_CENTER);
	glutAddMenuEntry("-----------------------", MNU_NONE);
	glutAddMenuEntry("quit [q, esc]", MNU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	/*** menu < ***/

	/* Enable textureGen for normal-looking textures on the dino model :) */
//	textureGen( 1 );

#ifdef FOG_ON
	//GLfloat color[4] = {0.70, 0.70, 0.70, 1.00};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_START, 10.50);
	glFogf(GL_FOG_END, 20.00);
	glFogf(GL_FOG_DENSITY, 0.05f);
	//glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_MODE, GL_EXP);
#endif	
	/* Load the various parts of the game */
	if (loadWorld() < 1)
	{
		/* Something went wrong while loading base and maze */
		printf("Error while doing loadWorld! Exiting...\n");
		return -1;
	} else
	{
		iprint("World loaded\n");
	}

	if (loadPlayer() < 1)
	{
		/* Something went wrong while loading the dino */
		printf("Error while doing loadPlayer! Exiting...\n");
		return -1;
	} else
	{
		iprint("Player loaded\n");
	}

	if (findFirstFreePlace() < 1)
	{
		/* We didn't find a passageway, error in maze */
		printf("Maze only contains walls. Exiting...\n");
		return -1;
	}

	centerViewOnPlayer();

	/* now loop */
	glutTimerFunc(30, rotateFunction, 0);	//movements of dino's limbs
	glutMainLoop();

	return 0;
}
////////////////////////////////////// main function and init <

