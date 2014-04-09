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
 * Version 2003-11-18
 * Michiel Scholten [ mbscholt@cs.vu.nl ]
 */

double time;
double theta;
int mousedown = 0;

/* Define vertex */
typedef GLfloat point3[3];

point3 pyramid[5] = {{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0,-1.0, -1.0}, {-1.0,-1.0,-1.0},	/* base */
	{0.0, 1.0, 0.0}};									/* top */

point3 cube[8] = {{-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0}, {-1.0,1.0,-1.0}, {-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}};
point3 colors[6] = {{175.0, 0.0, 0.0}, {0.0, 175.0, 0.0}, {0.0, 0.0, 175.0}, {50.0, 50.0, 0.0}, {0.0, 50.0, 50.0}, {0.0, 0.0, 50.0}};

void idle(void)
{
	/* Get the cpu loaded for 100% while updating continously :) */
	glutPostRedisplay();
}


void rotate(int value)
{
	if (mousedown)
	{
		/* Every 30msec, but only if value != 0 [it is when the mouse button is pushed] */
		theta += 10.8;
		if ( theta >= 360.0 )
		{
			theta -= 360.0;
		}
		glutTimerFunc(30, rotate, 1);
	}
}

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

void drawPyramid()
{
	point3 base[4];
	point3 triangle[3];
	/* we want a pyramid with a square base, every side a different colour */
	//glColor3ub(0.0, 175.0, 0.0);
	/* Draw the square base */
//	copyPoint3v(pyramid, base, 4);
//	drawSquare(base, colors);

	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glVertex3fv(pyramid[0]);
		glVertex3fv(pyramid[1]);
		glVertex3fv(pyramid[2]);
		glVertex3fv(pyramid[3]);
	glEnd();

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

#if 0
	/* Draw the sides */
	copyPoint3(&pyramid[0], &triangle[0]);
	copyPoint3(&pyramid[1], &triangle[1]);
	copyPoint3(&pyramid[4], &triangle[2]);
	drawTriangle(&triangle, &colors);
	
	copyPoint3(&pyramid[1], &triangle[0]);
	copyPoint3(&pyramid[2], &triangle[1]);
	copyPoint3(&pyramid[4], &triangle[2]);
	drawTriangle(&triangle, &colors);
	
	copyPoint3(&pyramid[2], &triangle[0]);
	copyPoint3(&pyramid[3], &triangle[1]);
	copyPoint3(&pyramid[4], &triangle[2]);
	drawTriangle(&triangle, &colors);
	
	copyPoint3(&pyramid[3], &triangle[0]);
	copyPoint3(&pyramid[0], &triangle[1]);
	copyPoint3(&pyramid[4], &triangle[2]);
	drawTriangle(&triangle, &colors);
#endif	
	/*
	glBegin(GL_POLYGON);
		glVertex3i(-5, -5);
		glVertex3i(-5,  5);
		glVertex3i( 5,  5);
		glVertex3i( 5, -5);
	glEnd();
	*/
	/*
	glColor3ub( 175.0, 0.0, 0.0 );
	glPopMatrix();
	glBegin(GL_POLYGON);
		glVertex2i(-5, -5);
		glVertex2i(-5,  5);
		glVertex2i( 5,  5);
		glVertex2i( 5, -5);
	glEnd();
	glPushMatrix();
	*/
}

int drawCube()
{
	point3 vertices[4];
	point3 color;

	copyPoint3(&cube[0], &vertices[0]);
	copyPoint3(&cube[1], &vertices[3]);
	copyPoint3(&cube[2], &vertices[2]);
	copyPoint3(&cube[3], &vertices[1]);
	copyPoint3(&colors[0], &color); 
//	drawSquare(&cube, &color);
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

void display(void)
{
//	double thetar;
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
	gluLookAt(-0.6, -0.6, 2.0,	// <- eye
			0.0, 0.0, 0.0,	// <- center
			0.0, 1.0, 0.0);	// <- up
#endif
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0,	// <- eye
			0.0, 0.0, 0.0,	// <- center
			0.0, 1.0, 0.0);	// <- up
	
	//drawSquare();

	glPushMatrix();
	//drawFrame();
#if 0
	glTranslatef(0.0f, 0.0f, 3.0f);
#endif
		glRotatef(theta, 0, 1, 0);
		drawPyramid();
	glPopMatrix();
	//drawCube();
	glFlush();
	glutSwapBuffers();
}

int loadModel()
{
	/* Load a model from file, in our case the F16 plane
	 * Check for existence of file, return 0 if error, otherwise 1 [or the number of vertices or something?]
	 */
	//scanf();
}

void keyboard(unsigned char key, int x, int y)
{
	/* When q, Q or ESC is pressed, exit the program */
	if ( key == 'q' || key == 'Q' || key == 27)
	{
		exit(0);
	}
}

void mouse(int btn, int btn_state, int x, int y)
{
	/* Print the states of the mouse buttons */
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		mousedown = 1;
		glutTimerFunc(30, rotate, 0);
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

void reshape_now(GLsizei w, GLsizei h)
{
	/* adjust clipping box */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w/h, 2.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	/* adjust viewport and draw the picture */

	if (h < w)
	{
		glViewport( w / 2 - h / 2, 0, h, h);
	} else
	{
		glViewport( 0, h / 2 - w / 2, w, w);
	}
}
int main(int argc, char **argv)
{
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

#if 0
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
/*	glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);*/
	//glFrustum(-2.0, 2.0, 1.5, -1.5, 1.0, 60.0);  // good enough?
	//gluPerspective(1.0, 5.0, 5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
#endif
	glEnable(GL_DEPTH_TEST);
	
	/* set various event callback functions */
	glutReshapeFunc(reshape_now);
	glutIdleFunc(idle);
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

	glutTimerFunc(30, rotate, 0);
	
	/* now loop */
	glutMainLoop();
}


