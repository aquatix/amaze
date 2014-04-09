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
 * Version 2003-11-10
 * Michiel Scholten [ mbscholt@cs.vu.nl ]
 */

double time;
double theta;

void idle(void)
{
	/* Get the cpu loaded for 100% while updating continously :) */

	/* use the VU-made function */
	//double t = Wallclock();
	double t = time + 0.02;
	/* update only every 1/100th of a second */
	if ( t < time + 0.01) return;
	time = t;
	theta += 2;
	if ( theta >= 360.0 )
	{
		theta -= 360.0;
	}
	glutPostRedisplay();
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
	if ( btn_state == GLUT_DOWN )
	{
		printf("mouse :: x = %i, y = %i", x, y);
	}

	/* Print the states of the mouse buttons */
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		printf(" :: left button down\n");
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
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
*/
	/* adjust viewport and draw the picture */
//	glViewport(0, 0, min(h,w), min(h,w));

	if (h < w)
	{
		glViewport( w / 2 - h / 2, 0, h, h);
	} else
	{
		glViewport( 0, h / 2 - w / 2, w, w);
	}
	glLoadIdentity();
//	glFlush();
}

void drawPyramid()
{
	/* we want a pyramid with a square base, every side a different colour */
	glColor3ub( 0.0, 175.0, 0.0);
	glPopMatrix();
	/* Draw the square base */
	glBegin(GL_POLYGON);
		glVertex3i(-5, -5);
		glVertex3i(-5,  5);
		glVertex3i( 5,  5);
		glVertex3i( 5, -5);
	glEnd();
	
	glColor3ub( 175.0, 0.0, 0.0 );
	/* now draw it */
	glPopMatrix();
	glBegin(GL_POLYGON);
		glVertex2i(-5, -5);
		glVertex2i(-5,  5);
		glVertex2i( 5,  5);
		glVertex2i( 5, -5);
	glEnd();
	glPushMatrix();
}

void drawSquare()
{
	/* we want a red square, so pick a nice red */
	glColor3ub( 175.0, 0.0, 0.0 );
	/* now draw it */
	glPopMatrix();
	glBegin(GL_POLYGON);
		glVertex2i(-5, -5);
		glVertex2i(-5,  5);
		glVertex2i( 5,  5);
		glVertex2i( 5, -5);
	glEnd();
	glPushMatrix();
}

void drawFrame()
{
	glRotatef( theta, 0.0, 0.0, 1.0 );

	/* blue wireframe rotating square */
	glColor3ub( 0.0, 0.0, 255.0 );
	//glPopMatrix();
	glBegin(GL_LINE_LOOP);
	//glBegin(GL_POLYGON);
		glVertex2i(-5, -5);
		glVertex2i(-5,  5);
		glVertex2i( 5,  5);
		glVertex2i( 5, -5);
	glEnd();
}

void display(void)
{
	double thetar;
	/* Get a nice darkblue as background */
	glClear(GL_COLOR_BUFFER_BIT);

	drawSquare();

	drawFrame();
	glutSwapBuffers();
	/* convert degrees to radians */
/*
	thetar = theta * ((2.0 * 3.14159) / 360.0);
	glVertex2f( cos(thetar), sin(thetar));
	glVertex2f(-sin(thetar), cos(thetar));
	glVertex2f(-cos(thetar),-sin(thetar));
	glVertex2f( sin(thetar),-cos(thetar));
	glEnd();
*/

	glFlush();
}

int main(int argc, char **argv)
{
	/* the menus */

	/* initialize glut and the window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("exercise 2 - solar system");

	glClearColor(0.0, 0.0, 0.5, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
/*	glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);*/
//	glFrustrum(
	glMatrixMode(GL_MODELVIEW);

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

	/* now loop */
	glutMainLoop();
}
