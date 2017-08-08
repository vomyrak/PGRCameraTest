#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <FlyCapture2.h>
#include "light_stage.h"

using namespace std;
using namespace FlyCapture2;

class test_interface
{
public:
	test_interface(Camera &cam, LightStage &ls, Property (&propty)[10]);
	void set_active_fixture(int control);
	uint8_t get_active_fixture();
	void set_active_channel(int control);
	uint8_t get_active_channel();
	void set_active_param(int control);
	uint16_t get_active_param();
	void set_file_name();
	string get_file_name();
	void apply_active_fixture();
	void apply_active_channel();
	void apply_active_param();
	void set_light_value(int r, int g, int b, int r2, int g2, int b2);
	void set_param_value(int index, float A, float B = 0);
	void set_camera_step(float low, float high, int para, int numOfPic);
	void set_light_step(float low, float high, int channel, int numOfPic);
	~test_interface();
	float max;
	float min;
	int nCapture;
	float stepsize;
	int channel_to_set;
	int param_to_set;
private:
	uint8_t light_value[6];
	stringstream file_name;
	uint8_t active_fixture;
	uint8_t active_channel;
	uint16_t active_param;
	Camera *camera;
	LightStage *stage;
	Property *prop;
	float param_default[11];
	bool param_set;
	bool channel_set;

};

