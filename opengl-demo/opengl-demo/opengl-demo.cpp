
#include "stdafx.h"
#include <glut.h>


typedef struct {
	double x;
	double y;
} Point2D;


void init(void)
{
	/*this function uses the glClearColor function, the effect of this function is to set the color, put it into the cache,
	GL_COLOR_BUFFER_BIT is a symbolic constant that is the value that glClearColor sets into the cache.
	The glClear function displays the cached color values*/

	glClearColor(1.0, 1.0, 1.0, 0.0);														// Background color, RGB is 111, white

	glMatrixMode(GL_PROJECTION);													// Set the projection method
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void drawLine(Point2D startPoint, Point2D endPoint)
{
	glBegin(GL_LINES);																				// Display a set of chord breaks whose endpoint coordinates are specified in the glVertex function

	glVertex2i(startPoint.x, startPoint.y);										// Specify starting point

	glVertex2i(endPoint.x, endPoint.y);											//Specify ending point

	glEnd();																									// end
}

void myDisplay(void)
{																						
	glClear(GL_COLOR_BUFFER_BIT);			

	Point2D startPoint = { 180.0, 15.0 };
	Point2D endPoint = { 10.0, 145.0 };

	glColor3f(0.0, 0.4, 0.2);																		// Set the RGB color of the line

	drawLine(startPoint, endPoint);													// Draw a two-dimensional line

	glFlush();																									// Refresh display buffer completion display

}

int main()
{
	// you can pass some control parameters to opengl here.
	int argc = 1;
	 char *argv[] = { (char *) "OpenGL Thread " };
	glutInit(&argc, argv);																		 // Initialize glut
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);				// Set display mode
	glutInitWindowPosition(50, 100);												// Set the opengl window display position
	glutInitWindowSize(400, 300);													// Set the opengl window size
	glutCreateWindow("OpenGL Window");								// Set the opengl window title
	init();																										//Initialize the drawing environment
	glutDisplayFunc(&myDisplay);													// Call display callback function drawing
	glutMainLoop();																				// Enter the glut cycle
	return 0;																								// Return at the end
}

