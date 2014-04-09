#include <GL/glut.h>
#include <stdio.h>
#include </home/mbscholt/mydocs/vu/20032004/cg/exercises/util/glutil.h>
/*
#include <util/glutil.h>
*/

/*
 * Computer Graphics - Exercise 1
 *
 * Version 2003-10-08
 * Michiel Scholten [ mbscholt@cs.vu.nl ]
 */


void display(void)
{
/*	glClearColor(1.0, 0.2, 1.0, 1.0); roze */
	/* Get a nice darkblue as background */
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

}

void idle(void)
{
	/* Get the cpu loaded for 100% while updating continously :) */
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
	/* Print the states of the mouse buttons */
	if ( btn == GLUT_LEFT_BUTTON && btn_state == GLUT_DOWN )
	{
		printf("mouse :: left button down\n");
	}
	if ( btn == GLUT_MIDDLE_BUTTON && btn_state == GLUT_DOWN )
	{
		printf("mouse :: middle button down\n");
	}
	if ( btn == GLUT_RIGHT_BUTTON && btn_state == GLUT_DOWN )
	{
		printf("mouse :: right button down\n");
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

int main(int argc, char **argv)
{
	int sub_menu, top_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("exercise 1");

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	sub_menu = glutCreateMenu(handle_menu);
	glutAddMenuEntry("entry 1", 1);
	glutAddMenuEntry("entry 2", 2);
	glutAddMenuEntry("entry 3", 3);
	/*glutCreateMenu(top_menu);*/
	glutAddMenuEntry("Quit [q, ESC]", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
