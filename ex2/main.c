#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <util/readtex.h>

/*
 * Computer Graphics - Exercise 2
 * Solar System
 *
 * Version 2003-11-28
 * Michiel Scholten [ mbscholt@cs.vu.nl | 1204467 ]
 */

/* Displaylists */
#define F16_MODEL 1

/* Menu-entries */
#define MNU_QUIT 4
#define MNU_SMOOTH 5
#define MNU_FLAT 6
#define MNU_VER 7
#define MNU_HOR 8
#define MNU_TEX_FULL 9
#define MNU_TEX_COMP 10
#define MNU_TEX_GRID 11
#define MNU_TEXMODE_MOD 12
#define MNU_TEXMODE_REP 13
#define MNU_TEXMODE_OFF 14

/* Switches for output to console */
#define DEBUG_OFF
#define INFO_OFF

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

#define NUMBER_OF_TEXTURES 1

/* Globally used variables */
double time;
int mousedown = 0;
double theta_pyramid, theta_cube, theta_f16;
int animate = 0;
int texturesEnabled = 1;
int pyr_x = 1;
int pyr_y = 0;

RGBImage *pTexture;
int textureId;
double texRowX = 1.0;
double texRowY = 1.0;
double rowDivide = 0.0;

float textureMode = GL_MODULATE;	// GL_REPLACE

/* Define vertex */
typedef GLfloat point3[3];

/* Struct to define material properties */
typedef struct
{
	float ambient[4];	/* params parameter in glMaterialfv call */
	float diffuse[4];
	float specular[4];
	float shininess;	/* Must be in range [0,128] */
} Material_t;


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

////////////////////////////////////// Lights >
#if 0
const float LIGHT0_POS[] =  {-1.5f, 5.0f, -15.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};  
const float LIGHT0_SPECULAR[] = {0.6f, 0.6f, 0.6f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, -10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.6, 0.6f, 0.6f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.8f, 0.4f, 0.4f, 1.0f};
////
const float LIGHT0_POS[] =  {-11.5f, 15.0f, -15.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};  
const float LIGHT0_SPECULAR[] = {0.6f, 0.6f, 0.6f, 1.0f};

const float LIGHT1_POS[] =  {14.0f, -11.5f, -110.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.6, 0.6f, 0.6f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.8f, 0.4f, 0.4f, 1.0f};
////
const float LIGHT0_POS[] =  {0.0f, 4.0f, 0.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};  
const float LIGHT0_SPECULAR[] = {0.6f, 0.6f, 0.6f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, -10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.8f, 0.4f, 0.4f, 1.0f};
#endif
const float LIGHT0_POS[] =  {2.0f, 4.0f, 2.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};  
const float LIGHT0_SPECULAR[] = {0.3f, 0.3f, 0.3f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, 10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.2f, 0.2f, 0.2f, 1.0f};
////////////////////////////////////// Lights <

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

void rotateF16(int value)
{
	if (animate)
	{
		/* Every 10msec, but only if user has enabled the animation with the spacebar */
		theta_f16 += 4;
		if ( theta_f16 >= 360.0 )
		{
			theta_f16 -= 360.0;
		}
		glutTimerFunc(10, rotateF16, 1);
		glutPostRedisplay();
	}
}
////////////////////////////////////// Rotating functions <

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
void drawPyramid()
{
	point3 normalVector;
	/* We want a pyramid with a square base, every side a different colour */

	/* Texture stuff */
	if (texturesEnabled) glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);	// target, pname, param
	glBindTexture(GL_TEXTURE_2D, textureId);

	/* Draw the square base */

	/*
	 * "Computer"
	 * 0 1
	 * 1 1
	 * 1 .5
	 * 0 .5
	 *
	 * "Computer
	 * Graphics"
	 * 0 1
	 * 1 1
	 * 1 0
	 * 0 0 
	 *
	 * 4x2 grid
	 * 0 2
	 * 4 2
	 * 4 0
	 * 0 0
	 */
	
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

int drawCube()
{
	point3 normalVector;

	/* Bottom */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[1], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[1]);
		glVertex3fv(cube[0]);
	glEnd();

	/* Front */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[6]);
		glVertex3fv(cube[7]);
	glEnd();

	/* Top */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[6]);
		glVertex3fv(cube[7]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[2]);
	glEnd();

	/* Back */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[3]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[2]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[0]);
		glVertex3fv(cube[1]);
	glEnd();

	/* Right */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[4]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[1]);
		glVertex3fv(cube[2]);
		glVertex3fv(cube[6]);
	glEnd();

	/* Left */
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colors[5]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight);
		calculateNormal(cube[4], cube[5], cube[6], normalVector);
		glNormal3fv(normalVector);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[0]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[7]);
	glEnd();
	return 1;
}

void display(void)
{
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
}

void reshape_now(GLsizei w, GLsizei h)
{
	double whRatio = (double)h / (double)w;
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
int loadModel(char *filename)
{
	int vertex, nrVertices;
	char identifier[20]; // string of maxlen 20
	GLfloat vertex_x, vertex_y, vertex_z, normal_x, normal_y, normal_z;
	int status;
	
	FILE *in;
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
		}
		glEnd();	/* Done with this figure */
	}
	iprint("done\n");
	return 1;
}

int loadTextures()
{
	pTexture = LoadRGB("../textures/reptile.rgb");
	glGenTextures(NUMBER_OF_TEXTURES, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// wrap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// wrap vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//target, level, components, width, height, border, format, type, *pixels
	glTexImage2D(GL_TEXTURE_2D, 0, pTexture->components, pTexture->sizeX, pTexture->sizeY, 0,  pTexture->format, GL_UNSIGNED_BYTE, pTexture->data);

	iprint("Texture \"../textures/text.rgb\" loaded\n");
	return 1;
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
			glutTimerFunc(20, rotateCube, 0);
			glutTimerFunc(10, rotateF16, 0);
		}
	}
}

void mouse(int btn, int btn_state, int x, int y)
{
	/* Print the states of the mouse buttons */
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		mousedown = 1;
		glutTimerFunc(30, rotatePyramid, 0);
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
		case 1: /* item 1 */
			iprint("menu :: entry 1\n");
			break;
		case 2: /* item 2 */
			iprint("menu :: entry 2\n");
			break;
		case 3: /* item 3 */
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
	char f16_files[8][40]= {"../models/f-16/afterburner.sgf", "../models/f-16/body.sgf", "../models/f-16/bomb.sgf", "../models/f-16/cockpit.sgf",
		"../models/f-16/rockets.sgf", "../models/f-16/tailfin.sgf", "../models/f-16/tailwings.sgf", "../models/f-16/wings.sgf"};
	int i, submenu_shading, submenu_rotation, submenu_textures, submenu_texmode;
			
	/* initialize glut and the window */
	glutInit(&argc, argv);
	/* Use rgb, double buffering and hidden surface removal */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Exercise 2 [Solar system | mbscholt@cs.vu.nl | 1204467]");

	/* Background color of the screen - "the sky" */
	//glClearColor(0.0, 0.0, 0.5, 1.0);
	glClearColor(0.0f, 0.5f, 0.9f, 1.0f);

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
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

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
	glutAddMenuEntry("menuentry 1", 1);
	glutAddMenuEntry("menuentry 2", 2);
	glutAddMenuEntry("menuentry 3", 3);
//	glutAddMenuEntry("-", 0);
	glutAddSubMenu("shading", submenu_shading);
	glutAddSubMenu("rotationtype", submenu_rotation);
	glutAddSubMenu("textures", submenu_textures);
	glutAddSubMenu("texture mode", submenu_texmode);
//	glutAddMenuEntry("-", 0);
	glutAddMenuEntry("quit [q, esc]", MNU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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
	
	/* now loop */
	glutMainLoop();

	return 0;
}
////////////////////////////////////// main function and init <

