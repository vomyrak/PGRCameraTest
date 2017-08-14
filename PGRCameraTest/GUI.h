#pragma once
#define NO_GLUI_LIB_PRAGMA

#include "Windows.h"
#include "gl\GL.h"
#include "gl\GLU.h"
#include "GL\freeglut.h"
#include "GL\glui.h"
class GUI
{
public:
	GUI(int argc, char** argv);
	~GUI();
	static void glu_objects();
	void init();
	static void quit_menu(int id) {
		if (id == 1) {
			exit(0);
		}
	}
private:
	int   wireframe = 0;
	int   segments = 8;
	int   main_window;
};

