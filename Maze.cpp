#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>

float posX = 0, posY = 0;

float pointVertex2[] = {
	1280 * 0.75, 360
};

void renderScene(void)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(0, 0.8, 0);
	glBegin(GL_QUADS);
	glVertex2f(10 + posX, 9 + posY);
	glVertex2f(20 + posX, 9 + posY);
	glVertex2f(20 + posX, 25 + posY);
	glVertex2f(10 + posX, 25 + posY);
	glEnd();

	//glEnable(GL_POINT_SMOOTH);
	//glBegin(GL_ARRAY_BUFFER);
	//glVertexPointer(2, GL_FLOAT, 0, pointVertex2);
	//glPointSize(50);
	//glDrawArrays(GL_POINTS, 0, 1);
	//glEnd();
	//glDisable(GL_POINT_SMOOTH);

	glFlush();
	glutPostRedisplay();
	glutSwapBuffers();
}

void resizeScene(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
}

void bad_key_callback(unsigned char key, int x, int y)
{
	if(key == 27)
		exit(0);
	else
		std::cout << "spatne tlacitko" << std::endl;
}

void key_callback(int key, int x, int y)
{
	switch(key)
	{
	default:
		std::cout << "spatne tlacitko" << std::endl;
		break;

	case GLUT_KEY_UP:
		std::cout << "Nahoru" << std::endl;
		posY += 2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		std::cout << "Dolu" << std::endl;
		posY -= 2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		std::cout << "Doleva" << std::endl;
		posX -= 1.5f;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "Doprava" << std::endl;
		posX += 1.5f;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 360);
	glutCreateWindow("Maze Game");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(resizeScene);

	glutKeyboardFunc(bad_key_callback);
	gluOrtho2D(0.0, 400, 0.0, 400);
	glutSpecialFunc(key_callback);

	glutMainLoop();

	return 0;
}
