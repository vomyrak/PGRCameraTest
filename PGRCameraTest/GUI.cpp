#include "GUI.h"



GUI::GUI(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//GLUT_DEPTH initialises depth mode
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("sphere");
	//glutReshapeFunc(reshape);
	glutMainLoop();
}


GUI::~GUI()
{
}
