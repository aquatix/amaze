#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <util/readtex.h>

/*
 * Computer Graphics - Final project
 * aMaze
 *
 * Michiel Scholten [ mbscholt@cs.vu.nl | 1204467 ]
 */
#define VERSION "2004-01-08 v0.0.05"
#define PROGINFO "[ Michiel Scholten | mbscholt@cs.vu.nl | 1204467 ]"

/*** Displaylists ***/
typedef enum
{
	MODEL_PLAYER, MODEL_WORLD
} displaylists;

/*** Menu-entries ***/
typedef enum
{
	MNU_QUIT, MNU_SMOOTH, MNU_FLAT, MNU_VER, MNU_HOR, MNU_TEX_FULL, MNU_TEX_COMP,
	MNU_TEX_GRID, MNU_TEXMODE_MOD, MNU_TEXMODE_REP, MNU_TEXMODE_OFF,
	MNU_BOGUS_1, MNU_BOGUS_2, MNU_BOGUS_3
} menus;

/*** Switches for output to console ***/
#define DEBUG
#define INFO

#ifdef DEBUG
#define dprint printf
#endif
#ifndef DEBUG
#define dprint (void)
#endif

#ifdef INFO
#define iprint printf
#endif
#ifndef INFO
#define iprint (void)
#endif

/*** Maze ***/
#define FILE_MAZE "maze.fil"

/*** Texture defining stuff ***/
#define NUMBER_OF_TEXTURES 1

#define FILE_PLAYER_BASEDIR "../models/dino/"
char player_files[][40] = {"arm.sgf", "body.sgf", "eye.sgf", "leg.sgf"};
#define PLAYER_NR_FILES 4

#define TEXTURES_BASEDIR "../textures/"
#define TEXT_REPTILE "reptile.rgb"
#define TEXT_GROUND "brick.rgb"
#define TEXT_WALL "rock.rgb"

/*** Globally used variables ***/
/* Counting fps */
int frame=0,time,timebase=0;
int font=(int)GLUT_BITMAP_8_BY_13;
char s[30];

double window_h=0, window_w=0;

#if 0
double time;
double theta_pyramid, theta_cube, theta_f16;
#endif
int mousedown = 0;
int animate = 1;
int texturesEnabled = 1;
int pyr_x = 1;
int pyr_y = 0;

RGBImage *pTexture_reptile;
RGBImage *pTexture_ground;
RGBImage *pTexture_wall;
typedef enum
{
	texture_reptile, texture_ground, texture_wall
} textureIds;

double texRowX = 1.0;
double texRowY = 1.0;
double rowDivide = 0.0;

float textureMode = GL_MODULATE;	// GL_REPLACE

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
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}
////////////////////////////////////// FPS display <


#if 0
////////////////////////////////////// Pyramid settings >
/* Vertices */
point3 pyramid[5] =
{
	{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0,-1.0, -1.0}, {-1.0,-1.0,-1.0},	/* base */
	{0.0, 1.0, 0.0}									/* top */
};

/* Material */
Material_t pyramidMaterial =
{
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialAmbient */
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialDiffuse */
	{0.6f, 0.6f, 0.6f, 1.0f},	/* materialSpecular */
	20				/* shininess */
};
////////////////////////////////////// Pyramid settings <

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

/* Colors used by the pyramid and cube */
point3 colors[6] =
{
	{175.0, 0.0, 0.0}, {0.0, 175.0, 0.0}, {0.0, 0.0, 175.0}, {50.0, 50.0, 0.0}, {0.0, 50.0, 50.0}, {50.0, 0.0, 50.0}
};

point3 specularLight = {.5, .5, .5};
#endif
////////////////////////////////////// Lights >
const float LIGHT0_POS[] =  {2.0f, 4.0f, 2.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_SPECULAR[] = {0.3f, 0.3f, 0.3f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, 10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.2f, 0.2f, 0.2f, 1.0f};
////////////////////////////////////// Lights <

#if 0
////////////////////////////////////// Rotating functions >
void rotatePyramid(int value)
{
	if (mousedown && animate)
	{
		/*
		 * Every 30msec, but only if the left mouse button is pushed and animation is enabled by user.
		 * Rotating once every 3sec
		 */
		theta_pyramid += 3.6;
		if ( theta_pyramid >= 360.0 )
		{
			theta_pyramid -= 360.0;
		}
		glutTimerFunc(30, rotatePyramid, 1);
		glutPostRedisplay();
	}
}

void rotateCube(int value)
{
	if (animate)
	{
		/* Every 20msec, but only if user has enabled the animation with the spacebar */
		theta_cube += 2; //was 2
		if ( theta_cube >= 360.0 )
		{
			theta_cube -= 360.0;
		}
		glutTimerFunc(20, rotateCube, 1);
		glutPostRedisplay();
	}
}

void rotatePlayer(int value)
{
	if (animate)
	{
		/* Every 10msec, but only if user has enabled the animation with the spacebar */
		theta_f16 += 4;
		if ( theta_f16 >= 360.0 )
		{
			theta_f16 -= 360.0;
		}
		glutTimerFunc(10, rotatePlayer, 1);
		glutPostRedisplay();
	}
}
////////////////////////////////////// Rotating functions <
#endif
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
void drawBase()
{
	/* Draw the base of the world */

	/* So, draw a nice square: */

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

	
#if 0
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[0], pyramid[1], pyramid[2], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, texRowY);
		glVertex3fv(pyramid[0]);
		glTexCoord2f(texRowX, texRowY);
		glVertex3fv(pyramid[1]);
		glTexCoord2f(texRowX, rowDivide);
		glVertex3fv(pyramid[2]);
		glTexCoord2f(0.0, rowDivide);
		glVertex3fv(pyramid[3]);
	glEnd();
#endif
}

int loadNDrawMaze()
{
	/* Load the maze datafile and draw it [ -> will be loaded into the world displaylist ] */
	int vertex, nrVertices;
	char identifier[20]; // string of maxlen 20
	GLfloat vertex_x, vertex_y, vertex_z, normal_x, normal_y, normal_z;
	int status;
	FILE *in;
	
	/* Texture stuff */
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, texture_wall);

	iprint("Loading maze from \"%s\" ... \n", FILE_MAZE);
	if ((in = fopen(FILE_MAZE, "r")) == NULL)
	{
		iprint("Unable to open the file [file: %s]\n", FILE_MAZE);
		return 0;
	}
	/* Load stuff */
	while (!feof(in))
	{
		status = fscanf(in, " %s %i \n", &identifier, &nrVertices);
		dprint(" - read type %s, nr of vertices is %i\n", identifier, nrVertices);

		if (status == EOF)
		{
			//
		}
	}
	return 1;
}

void drawPlayer()
{
	/* Draw the player, default is the dino */
}


#if 0
void drawPyramid()
{
	point3 normalVector;
	/* We want a pyramid with a square base, every side a different colour */

	/* Texture stuff */
	if (texturesEnabled) glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, textureId);

	/* Draw the square base */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[0], pyramid[1], pyramid[2], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, texRowY);
		glVertex3fv(pyramid[0]);
		glTexCoord2f(texRowX, texRowY);
		glVertex3fv(pyramid[1]);
		glTexCoord2f(texRowX, rowDivide);
		glVertex3fv(pyramid[2]);
		glTexCoord2f(0.0, rowDivide);
		glVertex3fv(pyramid[3]);
	glEnd();
//	glDisable(GL_TEXTURE_2D);

	/* Draw the 4 sides */
	/* Front */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[0], pyramid[1], pyramid[4], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(pyramid[0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(pyramid[1]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(pyramid[4]);
	glEnd();

	/* Right */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[1], pyramid[2], pyramid[4], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(pyramid[1]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(pyramid[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(pyramid[4]);
	glEnd();

	/* Back */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[3]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[2], pyramid[3], pyramid[4], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(pyramid[2]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(pyramid[3]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(pyramid[4]);
	glEnd();

	/* Left */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[4]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(pyramid[3], pyramid[0], pyramid[4], normalVector);
		glNormal3fv(normalVector);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(pyramid[3]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(pyramid[0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(pyramid[4]);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	/* Done :) */
}
#endif

void renderScene(void)
{
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 0
	/* FPS stuff */
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		sprintf(s,"FPS:%4.2f",
		frame*1000.0/(time-timebase));
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
#endif
	drawBase();
	
	glutSwapBuffers();

#if 0
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0,	// <- eye
		0.0, 0.0, 0.0,		// <- center
		0.0, 1.0, 0.0);		// <- up
	
	/* Pyramid > */
	glPushMatrix();
		//glRotatef(theta_pyramid, 1, 0, 0);
		glRotatef(theta_pyramid, pyr_x, pyr_y, 0);
		glScaled(2.0,2.0,2.0);
		drawPyramid();
	glPopMatrix();
	/* Pyramid < */

	glPushMatrix();
	glRotatef(theta_cube, 0.5, 1, 0);
		glTranslatef(0.0f, 0.0f, 10.0f);
		/* Cube > */
		glPushMatrix();
			//glRotatef(theta_cube, 0.5, 1, 0);	/* Some nice rotation */
			glRotatef(theta_cube, 1, 0, 0);		/* Rotation around x-axis [as stated on website */
			drawCube();
		glPopMatrix();
		/* Cube < */

		/* F16 > */
		glPushMatrix();
			glRotatef(-theta_f16, 0.0, 1.0, 0.0);
			glTranslatef(0.0f, 0.0f, 5.0f);
			glRotatef(-90, 0.0, 1.0, 0.0);
			glScaled(2.0,2.0,2.0);
			//drawCube();
			glCallList(F16_MODEL);
		glPopMatrix();
		/* F16 < */
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
#endif
}

void reshape_now(GLsizei w, GLsizei h)
{
	double whRatio = (double)h / (double)w;
	window_h = h;
	window_w = w;

	glViewport(0, 0, w, h);
	dprint("%f\n", whRatio);
	fflush(stdout);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.3, 0.3, -whRatio * 0.3, whRatio * 0.3, 0.3, 100);
	glMatrixMode(GL_MODELVIEW);
}
////////////////////////////////////// Drawing <

////////////////////////////////////// Loading stuff >
int loadModel(char *filename, int texture)
{
	int vertex, nrVertices;
	char identifier[20]; // string of maxlen 20
	GLfloat vertex_x, vertex_y, vertex_z, normal_x, normal_y, normal_z;
	int status;
	FILE *in;
	
	/* Texture stuff */
	if (texture > -1) glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, texture);

	iprint("Loading \"%s\" ... \n", filename);
	if ((in = fopen(filename, "r")) == NULL)
	{
		iprint("Unable to open the file [file: %s]\n", filename);
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
			glTexCoord2f(1.0, 0.0); //random placement of textures
		}
		glEnd();	/* Done with this figure */
	}
	iprint("done\n");
	return 1;
}

int loadTexture(char *filename, int texId, RGBImage *pTex)
{
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
#if 0
#define TEXTURES_BASEDIR "../textures/"
#define TEXT_REPTILE "reptile.rgb"
#define TEXT_GROUND "brick.rgb"
#define TEXT_WALL "rock.rgb"
#endif
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

#if 0
	pTexture_reptile = LoadRGB(filename);
	glGenTextures(NUMBER_OF_TEXTURES, &texture_reptile);
	glBindTexture(GL_TEXTURE_2D, texture_reptile);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// wrap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// wrap vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//target, level, components, width, height, border, format, type, *pixels
	glTexImage2D(GL_TEXTURE_2D, 0, pTexture_reptile->components, pTexture_reptile->sizeX, pTexture_reptile->sizeY, 0, pTexture_reptile->format, GL_UNSIGNED_BYTE, pTexture_reptile->data);
	iprint("Texture \"%s\" loaded\n", filename);

	filename[0] = '\0';
	strcat(filename, TEXTURES_BASEDIR);
	strcat(filename, TEXT_BRICK);
	pTexture_reptile = LoadRGB(filename);
	glGenTextures(NUMBER_OF_TEXTURES, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// wrap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// wrap vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//target, level, components, width, height, border, format, type, *pixels
	glTexImage2D(GL_TEXTURE_2D, 0, pTexture->components, pTexture->sizeX, pTexture->sizeY, 0, pTexture->format, GL_UNSIGNED_BYTE, pTexture->data);

	iprint("Texture \"../textures/rock.rgb\" loaded\n");

	filename[0] = '\0';
	strcat(filename, TEXTURES_BASEDIR);
	strcat(filename, TEXT_REPTILE);
	pTexture_reptile = LoadRGB("../textures/reptile.rgb");
	glGenTextures(NUMBER_OF_TEXTURES, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// wrap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// wrap vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//target, level, components, width, height, border, format, type, *pixels
	glTexImage2D(GL_TEXTURE_2D, 0, pTexture->components, pTexture->sizeX, pTexture->sizeY, 0, pTexture->format, GL_UNSIGNED_BYTE, pTexture->data);

	iprint("Texture \"../textures/reptile.rgb\" loaded\n");
#endif

	return 1;
}


int loadPlayer(void)
{
	int i;
	char filename[255];
	/* Load the player [dino] */
	for (i = 0; i < PLAYER_NR_FILES; i++)
	{
		filename[0]='\0';	//reset the string
		strcat(filename, FILE_PLAYER_BASEDIR);
		strcat(filename, player_files[i]);
		loadModel(filename, texture_reptile);
	}
}

int loadWorld(void)
{
	glNewList(MODEL_WORLD, GL_COMPILE);
		drawBase();
		loadNDrawMaze();
	glEndList();
}
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
//			glutTimerFunc(20, rotateCube, 0);
//			glutTimerFunc(10, rotatePlayer, 0);
		}
	}
}

void mouse(int btn, int btn_state, int x, int y)
{
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
}

void handle_menu(int whichone)
{
	/* Handle the selected item */
	switch (whichone)
	{
		case MNU_BOGUS_1: /* item 1 */
			iprint("menu :: entry 1\n");
			break;
		case MNU_BOGUS_2: /* item 2 */
			iprint("menu :: entry 2\n");
			break;
		case MNU_BOGUS_3: /* item 3 */
			iprint("menu :: entry 3\n");
			break;
		case MNU_FLAT:
			iprint("doing flat shading\n");
			glShadeModel(GL_FLAT);
			break;
		case MNU_SMOOTH:
			iprint("doing smooth shading\n");
			glShadeModel(GL_SMOOTH);
			break;
		case MNU_VER:
			iprint("doing vertical rotation of pyramid\n");
			pyr_x = 1;
			pyr_y = 0;
			break;
		case MNU_HOR:
			iprint("doing horizontal rotation of pyramid\n");
			pyr_x = 0;
			pyr_y = 1;
			break;
		case MNU_TEX_FULL:
			iprint("using full texture\n");
			texRowX = 1.0;
			texRowY = 1.0;
			rowDivide = 0.0;
			break;
		case MNU_TEX_COMP:
			iprint("using half texture [only \"computer\"]\n");
			texRowX = 1.0;
			texRowY = 1.0;
			rowDivide = 0.5;
			break;
		case MNU_TEX_GRID:
			iprint("using texture grid [4x2]\n");
			texRowX = 4.0;
			texRowY = 2.0;
			rowDivide = 0.0;
			break;
		case MNU_TEXMODE_MOD:
			iprint("using texture mode modulate\n");
			texturesEnabled = 1;
			textureMode = GL_MODULATE;
			break;
		case MNU_TEXMODE_REP:
			iprint("using texture mode replace\n");
			texturesEnabled = 1;
			textureMode = GL_REPLACE;
			break;
		case MNU_TEXMODE_OFF:
			iprint("using no textures\n");
			texturesEnabled = 0;
			break;
		case MNU_QUIT:
			iprint("menu :: quit entry chosen, exiting\n");
			exit(0);
			break;
	}
	
}
////////////////////////////////////// Handlers <

////////////////////////////////////// main function and init >
int main(int argc, char **argv)
{
	int i, submenu_shading, submenu_rotation, submenu_textures, submenu_texmode;

	/* If prog was called with param, only print info to console */
	if (argc > 1)
	{
		if (strcmp(argv[1], "--version") == 0)
		{
			printf("aMaze version %s\n", VERSION);
		} else
		{
			printf("aMaze\n\nUsage: just run the executable. Everything will be loaded automagically\n\nParams:\n        --version    prints version info\n");
		}
		printf("\n%s\n", PROGINFO);
		return 0;
	}


	/* initialize glut and the window */
	glutInit(&argc, argv);
	/* Use rgb, double buffering and hidden surface removal */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Project aMaze [ Michiel Scholten | mbscholt@cs.vu.nl | 1204467 ]");

	/* Background color of the screen - "the sky" */
	//glClearColor(0.0, 0.0, 0.5, 1.0);
	glClearColor(0.0f, 0.5f, 0.9f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);

	/* Enable special keys like GLUT_KEY_LEFT */
	//glutSpecialFunc(specialkeys);
	
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

	/* Load the texture[s] */
	loadTextures();
	
	/* create menu */
	submenu_shading = glutCreateMenu(handle_menu);
	glutAddMenuEntry("flat",MNU_FLAT);
	glutAddMenuEntry("smooth",MNU_SMOOTH);

	submenu_rotation = glutCreateMenu(handle_menu);
	glutAddMenuEntry("vertical",MNU_VER);
	glutAddMenuEntry("horizontal",MNU_HOR);

	submenu_textures = glutCreateMenu(handle_menu);
	glutAddMenuEntry("full texture",MNU_TEX_FULL);
	glutAddMenuEntry("computer",MNU_TEX_COMP);
	glutAddMenuEntry("grid",MNU_TEX_GRID);

	submenu_texmode = glutCreateMenu(handle_menu);
	glutAddMenuEntry("modulate",MNU_TEXMODE_MOD);
	glutAddMenuEntry("replace",MNU_TEXMODE_REP);
	glutAddMenuEntry("off",MNU_TEXMODE_OFF);

	glutCreateMenu(handle_menu);
	glutAddMenuEntry("menuentry 1", MNU_BOGUS_1);
	glutAddMenuEntry("menuentry 2", MNU_BOGUS_2);
	glutAddMenuEntry("menuentry 3", MNU_BOGUS_3);
	glutAddSubMenu("shading", submenu_shading);
	glutAddSubMenu("rotationtype", submenu_rotation);
	glutAddSubMenu("textures", submenu_textures);
	glutAddSubMenu("texture mode", submenu_texmode);
	glutAddMenuEntry("quit [q, esc]", MNU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	if (loadPlayer() < 1)
	{
		printf("Error while doing loadPlayer!\n");
		return -1;
	}
	
#if 0
	/* Load the F16 into a display list | void glNewList(GLuint listID, GLenum mode); */
	glNewList(F16_MODEL, GL_COMPILE);
	for (i = 0; i < 8; i++)
	{
		/* Default color [some shade of gray */
		point3 theColor = { .75, .5, .5};

		if (strcmp("../models/f-16/cockpit.sgf", f16_files[i]) == 0) { theColor[0] = .0f;theColor[1] = .0f; theColor[2] = .7f; };
		if (strcmp("../models/f-16/rockets.sgf", f16_files[i]) == 0) { theColor[0] = 1.0f;theColor[1] = .0f; theColor[2] = .0f; };
		if (strcmp("../models/f-16/bomb.sgf", f16_files[i]) == 0) { theColor[0] = .0f;theColor[1] = 1.0f; theColor[2] = .0f; };
		if (strcmp("../models/f-16/afterburner.sgf", f16_files[i]) == 0) { theColor[0] = .5f;theColor[1] = .25f; theColor[2] = .25f; };

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, theColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		/* Use some custom colors for various parts */
		if (loadModel(f16_files[i]) == 0)
		{
			printf("ERROR while loading %s\n", f16_files[i]);
			exit(1);
		}
	}
	glEndList();
#endif

	/* now loop */
	glutMainLoop();

	return 0;
}
////////////////////////////////////// main function and init <

