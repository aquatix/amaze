#include <GL/glut.h>
#include <stdio.h>

#include <util/readtex.h>
#if 0
#include "../util/readtex.h"
#include </home/mbscholt/mydocs/vu/20032004/cg/exercises/util/glutil.h>
#include </home/mbscholt/xc_home/mydocs/vu/20032004/cg/exercises/util/glutil.h>
#include "util/glutil.h"
#endif

/*
 * Computer Graphics - Exercise 2
 * Solar System
 *
 * Version 2003-11-26
 * Michiel Scholten [ mbscholt@cs.vu.nl | 1204467 ]
 */

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

double time;
int mousedown = 0;
double theta_pyramid, theta_cube, theta_f16;
int animate = 0;
int pyr_x = 1;
int pyr_y = 0;

RGBImage *texture;

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
point3 pyramid[5] =
{
	{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0,-1.0, -1.0}, {-1.0,-1.0,-1.0},	/* base */
	{0.0, 1.0, 0.0}									/* top */
};

Material_t pyramidMaterial =
{
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialAmbient */
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialDiffuse */
	{0.6f, 0.6f, 0.6f, 1.0f},	/* materialSpecular */
	0				/* shininess */
};
////////////////////////////////////// Pyramid settings <

////////////////////////////////////// Cube settings >
point3 cube[8] =
{
	{-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0}, {-1.0,1.0,-1.0}, {-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}
};

Material_t cubeMaterial =
{
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialAmbient */
	{0.0f, 0.0f, 0.0f, 1.0f},	/* materialDiffuse */ 
	{0.3f, 0.3f, 0.3f, 1.0f},	/* materialSpecular */
	0				/* shininess */
}; 
////////////////////////////////////// Cube settings <


point3 colors[6] =
{
	{175.0, 0.0, 0.0}, {0.0, 175.0, 0.0}, {0.0, 0.0, 175.0}, {50.0, 50.0, 0.0}, {0.0, 50.0, 50.0}, {50.0, 0.0, 50.0}
};

////////////////////////////////////// Lights >
const float LIGHT0_POS[] =  {-1.5f, 5.0f, -15.0f, 1.0f};
const float LIGHT0_AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};
const float LIGHT0_DIFFUSE[] = {0.2f, 0.2f, 0.2f, 1.0f};  
const float LIGHT0_SPECULAR[] = {0.6f, 0.6f, 0.6f, 1.0f};

const float LIGHT1_POS[] =  {4.0f, -1.5f, -10.0f, 1.0f};
const float LIGHT1_AMBIENT[] =  {0.6, 0.6f, 0.6f, 1.0f};
const float LIGHT1_DIFFUSE[] =  {0.6f, 0.6f, 0.6f, 1.0f};
const float LIGHT1_SPECULAR[] =  {0.8f, 0.4f, 0.4f, 1.0f};
////////////////////////////////////// Lights <

#if 0
/* Clear color */
const GLfloat CLEAR_COLOR[4] = {0.5f, 0.85f, 1.0f, 1.0f};
#endif

void idle(void)
{
	/* Get the cpu loaded for 100% while updating continously :) | -> Not used in this exercise */
//	glutPostRedisplay();
}

void setMaterial(Material_t *material, GLenum mode)
{
	glMaterialfv(mode, GL_AMBIENT, material->ambient);
	glMaterialfv(mode, GL_DIFFUSE, material->diffuse);
	glMaterialfv(mode, GL_SPECULAR, material->specular);
	glMaterialf(mode, GL_SHININESS, material->shininess);
}

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
		theta_cube += 2;
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

////////////////////////////////////// Drawing >
void drawPyramid()
{
	/* We want a pyramid with a square base, every side a different colour */
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	setMaterial(&pyramidMaterial, GL_FRONT_AND_BACK);

	/* Draw the square base */
	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(pyramid[0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(pyramid[1]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(pyramid[2]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(pyramid[3]);
	glEnd();

	/* Draw the 4 sides */
	glBegin(GL_POLYGON);
		glColor3fv(colors[1]);
		glVertex3fv(pyramid[0]);
		glVertex3fv(pyramid[1]);
		glVertex3fv(pyramid[4]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[2]);
		glVertex3fv(pyramid[1]);
		glVertex3fv(pyramid[2]);
		glVertex3fv(pyramid[4]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[3]);
		glVertex3fv(pyramid[2]);
		glVertex3fv(pyramid[3]);
		glVertex3fv(pyramid[4]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[4]);
		glVertex3fv(pyramid[3]);
		glVertex3fv(pyramid[0]);
		glVertex3fv(pyramid[4]);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);

	/* Done :) */
}

int drawCube()
{
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	setMaterial(&cubeMaterial, GL_FRONT_AND_BACK);

	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[1]);
		glVertex3fv(cube[0]);
		glNormal3f(0.5,0.5,0.5);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[1]);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[6]);
		glVertex3fv(cube[7]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[2]);
		glVertex3fv(cube[6]);
		glVertex3fv(cube[7]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[2]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[3]);
		glVertex3fv(cube[2]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[0]);
		glVertex3fv(cube[1]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[1]);
		glVertex3fv(cube[2]);
		glVertex3fv(cube[6]);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3fv(colors[5]);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[0]);
		glVertex3fv(cube[3]);
		glVertex3fv(cube[7]);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

#if 0
/*
int drawSquare(point3 *vertices, point3 colors)
*/
int drawSquare(int a, int b, int c, int d)
{
	/* set the color of the square */
	//glColor3ub(colors);
	glColor3fv(colors);
	/* now draw the square */
/*
printf("square\n");
printf("vertex 0: %i\n", vertices[0]);
printf("vertex 1: %i\n", vertices[1]);
printf("vertex 2: %i\n", vertices[2]);
printf("vertex 3: %i\n", vertices[3]);
	glBegin(GL_QUADS);
		glVertex3fv(vertices[0]);
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[2]);
		glVertex3fv(vertices[3]);
	glEnd();
*/

	glBegin(GL_QUADS);
		;
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[2]);
		glVertex3fv(vertices[3]);
	glEnd();
}

int drawTriangle(point3 *vertices, point3 colors)
{
	/* set the color of the square */
	glColor3fv(colors);
	/* now draw the triangle */
	/*
	glPushMatrix();
	glBegin(GL_POLYGON);
		glVertex3fv(vertices[0]);
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[2]);
	glEnd();
	glPopMatrix();
	*/
	glBegin(GL_POLYGON);
		glVertex3fv(vertices[0]);
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[2]);
	glEnd();
}
#endif

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
			//glTranslatef(1.0f, 0.0f, 3.0f);
			glRotatef(-theta_f16, 0.0, 1.0, 0.0);
			glTranslatef(0.0f, 0.0f, 5.0f);
			//glRotatef(theta_f16, 0.0, 1.5, 0);
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
	glFrustum(-0.3, 0.3, -whRatio * 0.3 ,whRatio * 0.3 ,0.3 ,100);
	glMatrixMode(GL_MODELVIEW);
#if 0
	/* adjust clipping box *//*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w/h, 2.0, 40.0);
	glMatrixMode(GL_MODELVIEW);*/
	/* adjust viewport and draw the picture */
	if (h < w)
	{
		glViewport( w / 2 - h / 2, 0, h, h);
	} else
	{
		glViewport( 0, h / 2 - w / 2, w, w);
	}
#endif
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

		glEnable(GL_COLOR_MATERIAL);

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
			glDisable(GL_COLOR_MATERIAL);	/* Just to be sure */
			return -1;
		}
		/* Load vertices and draw the thing */
		//glColor3fv(colors[0]);
		for (vertex = 0; vertex < nrVertices; vertex++)
		{
			fscanf(in, " %f %f %f %f %f %f\n", &vertex_x, &vertex_y, &vertex_z, &normal_x, &normal_y, &normal_z);
			glNormal3f(normal_x, normal_y, normal_z);
			glVertex3f(vertex_x, vertex_y, vertex_z);
		}
		glEnd();	/* Done with this figure */
		glDisable(GL_COLOR_MATERIAL);
	}
	iprint("done\n");
	return 1;
}

int loadTextures()
{
//	LoadRGB("../textures/text.rgb");
	texture = LoadRGB("../textures/text.rgb");	
	iprint("Texture \"../textures/text.rgb\" loaded\n");
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
	switch (whichone)
	{
		case 1: /* item 1 */
			printf("menu :: entry 1\n");
			break;
		case 2: /* item 2 */
			printf("menu :: entry 2\n");
			break;
		case 3: /* item 3 */
			printf("menu :: entry 3\n");
			break;
		case MNU_FLAT:
			dprint("doing flat shading\n");
			glShadeModel(GL_FLAT);
			break;
		case MNU_SMOOTH:
			dprint("doing smooth shading\n");
			glShadeModel(GL_SMOOTH);
			break;
		case MNU_VER:
			dprint("doing vertical rotation of pyramid\n");
			pyr_x = 1;
			pyr_y = 0;
			break;
		case MNU_HOR:
			dprint("doing horizontal rotation of pyramid\n");
			pyr_x = 0;
			pyr_y = 1;
			break;
		case MNU_TEX_FULL:
			dprint("using full texture\n");
			/**/
			break;
		case MNU_TEX_COMP:
			dprint("using half texture [only \"computer\"]\n");
			/**/
			break;
		case MNU_TEX_GRID:
			dprint("using texture grid [4x2]\n");
			/**/
			break;
		case MNU_QUIT:
			printf("menu :: quit entry chosen, exiting\n");
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
	int i, submenu_shading, submenu_rotation, submenu_textures;
			
	/* initialize glut and the window */
	glutInit(&argc, argv);
	/* Use rgb, double buffering and hidden surface removal */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Exercise 2 [Solar system | mbscholt@cs.vu.nl | 1204467]");

	//glClearColor(0.0, 0.0, 0.5, 1.0);
	glClearColor(0.0, 0.5, 0.9, 1.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	/* set various event callback functions */
	glutReshapeFunc(reshape_now);
//	glutIdleFunc(idle);
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

	glutCreateMenu(handle_menu);
	glutAddMenuEntry("menuentry 1", 1);
	glutAddMenuEntry("menuentry 2", 2);
	glutAddMenuEntry("menuentry 3", 3);
//	glutAddMenuEntry("-", 0);
	glutAddSubMenu("shading",submenu_shading);
	glutAddSubMenu("rotationtype",submenu_rotation);
	glutAddSubMenu("textures",submenu_textures);
//	glutAddMenuEntry("-", 0);
	glutAddMenuEntry("quit [q, esc]", MNU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Load the F16 into a display list | void glNewList(GLuint listID, GLenum mode); */
	glNewList(F16_MODEL, GL_COMPILE);
	for (i = 0; i < 8; i++)
	{
		/* Default color [some shade of gray */
		glColor3f(.75, .5, .5);
		/* Use some custom colors for various parts */
		if (strcmp("../models/f-16/cockpit.sgf", f16_files[i]) == 0) glColor3f(.0, .0, .7);
		if (strcmp("../models/f-16/rockets.sgf", f16_files[i]) == 0) glColor3f(1.0, .0, .0);
		if (strcmp("../models/f-16/bomb.sgf", f16_files[i]) == 0) glColor3f(.0, 1.0, .0);
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

