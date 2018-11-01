#include <GL/glut.h> 
#include <Windows.h>
#include<iostream>
#include "GLFW/glfw3.h"
#define WIN_WIDTH 600
#define WIN_HEIGHT 600
GLfloat angle = 10.0f;
GLfloat xDirection = 0.0f,yDirection = 0.0f,zDirection = 10.0f;
using namespace std;

void InitEnvironment()
{
	glEnable(GL_DEPTH);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(0, 0, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, 1, 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xDirection, yDirection, zDirection, 0, 0, 0, 0, 1, 0);
}

//实现立方体的旋转功能
void KeyBoards(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, -1, 0, 0);
		glutPostRedisplay();
		break;
	case 'a':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, 0, 0, -1);
		glutPostRedisplay();
		break;
	case 's':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, 1, 0, 0);
		glutPostRedisplay();
		break;
	case 'd':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, 0, 0, 1);
		glutPostRedisplay();
		break;
		//输入“Esc”时退出程序
	case 27:
		exit(0);
		break;
	}
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glutSolidDodecahedron();
	glutWireCube(5);
	glutSwapBuffers();
}

void RotateRect()
{
	angle += 1;
	if (angle >= 360)
	{
		angle = 0.0f;
	}
	Sleep(30);
	myDisplay();
}

void RotateFast()
{
	angle += 1;
	if (angle >= 360)
	{
		angle = 0.0f;
	}
	Sleep(3);
	myDisplay();
}

void OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(RotateFast);
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(NULL);
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(WIN_HEIGHT, WIN_WIDTH);
	glutCreateWindow("RotationCube");
	InitEnvironment();
	glutKeyboardFunc(&KeyBoards); 
	glutMouseFunc(&OnMouse);
	glutDisplayFunc(&myDisplay);  
	glutMainLoop();   
	return 0;
}
