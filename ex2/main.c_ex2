#include <GL/glut.h>
#include <stdio.h>
#include <util/glutil.h>
#include <util/readtex.h>

void display(void)
{
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_RGB);
	glutInitWindowSize (800,600);
	glutInitWindowPosition(0,0);
    glutCreateWindow ("exercise 2");

	glutIdleFunc(idle);
	glutDisplayFunc(display);

	glutMainLoop();
}
