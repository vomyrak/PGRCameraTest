#include "GUI.h"



GUI::GUI(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//GLUT_DEPTH initialises depth mode
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	int window = glutCreateWindow("sphere");

	
	glutCreateMenu(quit_menu);
	glutAddMenuEntry("quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//glutReshapeFunc(reshape);
	init();
	glutDisplayFunc(glu_objects);
	glutCreateMenu(quit_menu);
	//glutCreateSubWindow(window, 50, 50, 50, 50);
	//init();
	//glutDisplayFunc(glu_objects);
	GLUI *glui = GLUI_Master.create_glui("sphere");
	glui->add_checkbox("123");
	new GLUI_Checkbox(glui, "wireframe", &wireframe);
	(new GLUI_Spinner(glui, "Segments:", &segments))
		->set_int_limits(3, 60);

	glui->set_main_gfx_window(window);
	glui->set_current_draw_buffer();
	glui->set_viewport();

	glutMainLoop();
}


GUI::~GUI()
{
}

void GUI::glu_objects() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//inidialise depth buffer also
	glFlush();
	//gluNewQuadric();		//GLUquadricObj*, creates a pointer to the object
	//gluDeleteQuadric(gluNewQuadric());	//delete an obj
	//decltype(gluNewQuadric()) newobject = gluNewQuadric();
	//gluQuadricDrawStyle(newobject, GL_FILL);	//set drawing style GLU_POINT/LINE/FILL/SILHOUETTE
	//gluQuadricNormals(newobject, GLU_FLAT);	//specify normal mode
	//gluQuadricTexture(newobject, GL_FALSE);	//specify texture coordinates
											//gluSphere(*obj, radius, slices, stacks) creates a sphere, using slices line of longitude, and stacks lines of latitude
											//gluCylinder(*obj, base, top, left, slices, stacks) renders a centered cylinder of height aligned with z axis whose base is in the plane z=0
											//with specified base and top radii
											//gluDisk(*obj, inner, outer, slices, rings) reners a disk in the plane z = 0 by inner and outer radii. formed from concentric rings
											//gluPartialDisk(*obj, inner, outer, slices, rings, start, angle) the disk of wedge of angle degrees removed, starting at start.
	//gluSphere(newobject, 1.0, 12, 12);
	//glutSolidSphere(1.0, 10, 10);
}

void GUI::init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);		//enable depth mode and depth buffer
}
