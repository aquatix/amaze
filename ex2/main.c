#include <GL/glut.h>
#include <stdio.h>

#include </home/mbscholt/mydocs/vu/20032004/cg/exercises/util/glutil.h>
/*
#include </home/mbscholt/xc_home/mydocs/vu/20032004/cg/exercises/util/glutil.h>
#include "util/glutil.h"
*/

/*
 * Computer Graphics - Exercise 2
 * Solar System
 *
 * Version 2003-11-21
 * Michiel Scholten [ mbscholt@cs.vu.nl ]
 */

#define F16_MODEL 0
#define DEBUG

#ifdef DEBUG
#define dprint printf
#endif
#ifndef DEBUG
#define dprint (void)
#endif

double time;
int mousedown = 0;
double theta_pyramid, theta_cube, theta_f16;
int animate = 0;

/* Define vertex */
typedef GLfloat point3[3];

point3 pyramid[5] = {{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0,-1.0, -1.0}, {-1.0,-1.0,-1.0},	/* base */
	{0.0, 1.0, 0.0}};									/* top */

point3 cube[8] = {{-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0}, {-1.0,1.0,-1.0}, {-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}};
point3 colors[6] = {{175.0, 0.0, 0.0}, {0.0, 175.0, 0.0}, {0.0, 0.0, 175.0}, {50.0, 50.0, 0.0}, {0.0, 50.0, 50.0}, {50.0, 0.0, 50.0}};

void idle(void)
{
	/* Get the cpu loaded for 100% while updating continously :) */
//	glutPostRedisplay();
}


////////////////////////////////////// Rotating functions >
void rotatePyramid(int value)
{
	if (mousedown && animate)
	{
		/* Every 30msec, but only if the left mouse button is pushed. Rotating ones every 3sec */
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
		/* Every 20msec, but only if user has enabled it with the spacebar */
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
		/* Every 10msec, but only if user has enabled it with the spacebar */
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

////////////////////////////////////// Ish >
int copyPoint3v(point3 *src, point3 *dest, int number)
{
	int i;
	for (i=0; i<number; i++)
	{
		copyPoint3(*dest[i], *src[i]);
	}
	return 1;
}

int copyPoint3(point3 *src, point3 *dest)
{
/*
	int i;
	for (i = 0; i<4; i++)
	{
		*dest[i] = *src[i];
	}
*/
	dest = src;
printf("point: %i\n", dest);
	return 1;
}
////////////////////////////////////// Ish <

////////////////////////////////////// Drawing >
void drawPyramid()
{
	point3 base[4];
	point3 triangle[3];
	/* we want a pyramid with a square base, every side a different colour */
	//glColor3ub(0.0, 175.0, 0.0);
	/* Draw the square base */
	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glVertex3fv(pyramid[0]);
		glVertex3fv(pyramid[1]);
		glVertex3fv(pyramid[2]);
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
	/* Done :) */
}

int drawCube()
{
#if 0
	point3 vertices[4];
	point3 color;

	copyPoint3(&cube[0], &vertices[0]);
	copyPoint3(&cube[1], &vertices[3]);
	copyPoint3(&cube[2], &vertices[2]);
	copyPoint3(&cube[3], &vertices[1]);
	copyPoint3(&colors[0], &color); 
//	drawSquare(&cube, &color);
#endif

	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glVertex3fv(cube[4]);
		glVertex3fv(cube[5]);
		glVertex3fv(cube[1]);
		glVertex3fv(cube[0]);
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
#endif
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
////////////////////////////////////// Drawing <

void display(void)
{
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0,	// <- eye
			0.0, 0.0, 0.0,	// <- center
			0.0, 1.0, 0.0);	// <- up
	
	//drawSquare();

	glPushMatrix();
#if 0
	glTranslatef(0.0f, 0.0f, 3.0f);
#endif
		glRotatef(theta_pyramid, 0, 1, 0);
		glScaled(2.0,2.0,2.0);
		drawPyramid();

	glPopMatrix();

	glRotatef(theta_cube, 0.5, 1, 0);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 10.0f);
		glPushMatrix();
		//glScaled(4.0,4.0,4.0);
		//glRotatef(theta_cube, 0.5, 1, 0);	/* Some nice rotation */
		glRotatef(theta_cube, 1, 0, 0);
		drawCube();
		glPopMatrix();

		
		glPushMatrix();
			//glTranslatef(1.0f, 0.0f, 3.0f);
		glRotatef(theta_f16, 0.0, 1, 0);
			glTranslatef(0.0f, 0.0f, 3.0f);
			//glRotatef(theta_f16, 0.0, 1.5, 0);
			drawCube();
			//drawF16();
			//glCallList(F16_MODEL);
		glPopMatrix();
	glPopMatrix();
	


	
	glFlush();
	glutSwapBuffers();
}

////////////////////////////////////// Loading stuff >
#if 0
int loadModel(char *filename[], int nrOfFiles, int identifier)
{
	/* Load a model from file, in our case the F16 plane
	 * Check for existence of file, return 0 if error, otherwise 1 [or the number of vertices or something?]
	 * If succesfull, load everything in a list
	 */
	int i;
	FILE *in;
	for (i = 0; i < nrOfFiles; i++)
	{
		printf("%i\n", i);
		if ((in = fopen(filename[i], "r")) == NULL)
		{
			printf("error %i\n", i);
			printf("Unable to open the file [file #%i, name: %s]\n", i, filename[i]);
			return 0;
		}
		printf("good %i\n", i);
		printf("%s\n", *filename[i]);
	}
	//scanf();
}
#endif
int loadModel(char *filename)
{
	int vertex, nrVertices;
	char identifier[20]; // string of maxlen 20
	GLfloat vertex_x, vertex_y, vertex_z, normal_x, normal_y, normal_z;
	int thisType, status;
	
	FILE *in;
	printf("Loading \"%s\" ... \n", filename);
	if ((in = fopen(filename, "r")) == NULL)
	{
		printf("Unable to open the file [file: %s]\n", *filename);
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
			dprint("ERROR @ loadModel :: Reading of identifier and number failed!\n");
			return 0;
		}

		/* 
		 * Look which kind of figure we have to draw
		 * Chose from POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, QUADS, TRIANGLE_STRIP, TRIANGLE_FAN, QUAD_STRIP or POLYGON
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
			printf("ERROR @ loadModel :: unknown identifier!\n");
			return -1;
		}
		/* Load vertices and draw the thing */
		glColor3fv(colors[0]);
		for (vertex = 0; vertex < nrVertices; vertex++)
		{
			fscanf(in, " %f %f %f %f %f %f\n", &vertex_x, &vertex_y, &vertex_z, &normal_x, &normal_y, &normal_z);
			//glNormal3f(normal_x, normal_y, normal_z);
			glVertex3f(vertex_x, vertex_y, vertex_z);
		}
		glEnd();
	}
	printf("done\n");
	return 1;
}
////////////////////////////////////// Loading stuff <

////////////////////////////////////// Handlers >
void keyboard(unsigned char key, int x, int y)
{
	/* When q, Q or ESC is pressed, exit the program */
	if ( key == 'q' || key == 'Q' || key == 27)
	{
		exit(0);
	}
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
		case 4:
			printf("menu :: quit entry chosen, exiting\n");
			exit(0);
			break;
	}
	
}
////////////////////////////////////// Handlers <

void reshape_now(GLsizei w, GLsizei h)
{
	double whRatio = (double)h / (double)w;
	glViewport(0, 0, w, h);
	printf("%f\n", whRatio);
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

int main(int argc, char **argv)
{
#if 0
	char[8][15] f16_files = {"afterburner.sgf", "body.sgf", "bomb.sgf", "cockpit.sgf",
		"rockets.sgf", "tailfin.sgf", "tailwings.sgf", "wings.sgf"};
	int i;
#endif
	char f16_files[8][40]= {"../models/f-16/afterburner.sgf", "../models/f-16/body.sgf", "../models/f-16/bomb.sgf", "../models/f-16/cockpit.sgf",
		"../models/f-16/rockets.sgf", "../models/f-16/tailfin.sgf", "../models/f-16/tailwings.sgf", "../models/f-16/wings.sgf"};
	int i;
			
	/* the menus */

	/* initialize glut and the window */
	glutInit(&argc, argv);
	/* Use rgb, double buffering and hidden surface removal */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Exercise 2 [Solar system | mbscholt@cs.vu.nl | 1204467]");

	glClearColor(0.0, 0.0, 0.5, 1.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	/* set various event callback functions */
	glutReshapeFunc(reshape_now);
//	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	/* create menu */
	glutCreateMenu(handle_menu);
	glutAddMenuEntry("entry 1", 1);
	glutAddMenuEntry("entry 2", 2);
	glutAddMenuEntry("entry 3", 3);
	glutAddMenuEntry("quit [q, esc]", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//glutTimerFunc(30, rotatePyramid, 0);
	
	/* Load the F16 into a list */
	//void glNewList(GLuint listID, GLenum mode);
	glNewList(F16_MODEL, GL_COMPILE);
	for (i = 0; i < 8; i++)
	{
		if (loadModel(&f16_files[i]) == 0)
		{
			printf("ERROR while loading %s\n", f16_files[i]);
			exit(1);
		}
	}
#if 0
		loadModel("../models/f-16/afterburner.sgf");
		loadModel("../models/f-16/body.sgf");
		loadModel("../models/f-16/bomb.sgf");
		loadModel("../models/f-16/cockpit.sgf");
		loadModel("../models/f-16/rockets.sgf");
		loadModel("../models/f-16/tailfin.sgf");
		loadModel("../models/f-16/tailwings.sgf");
		loadModel("../models/f-16/wings.sgf");
	//loadModel(&f16_files, 8, F16_MODEL);
#endif
	void glEndList(void);
	
	/* now loop */
	glutMainLoop();
}

