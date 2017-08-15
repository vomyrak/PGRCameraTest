#include "test_interface.h"



test_interface::test_interface(Camera &cam, LightStage &ls, Property (&propty)[10]) {
	light_value[0] = ls[0][0]->get_red();
	light_value[1] = ls[0][0]->get_green();
	light_value[2] = ls[0][0]->get_blue();
	light_value[3] = ls[0][0]->get_red2();
	light_value[4] = ls[0][0]->get_green2();
	light_value[5] = ls[0][0]->get_blue2();
	active_Old_Fixture = 0b11111111;
	active_channel = 0b00000111;
	active_param = 0b1111111111;	
	camera = &cam;
	stage = &ls;
	prop = &propty[0];
	param_default[0] = 0;
	param_default[1] = 0;
	param_default[2] = 1024;
	param_default[3] = 0;
	param_default[4] = 100;
	param_default[5] = 1;
	param_default[6] = 30;
	param_default[7] = 0;
	param_default[8] = 80;
	param_default[9] = 482;
	param_default[10] = 762;
	param_set = false;
	channel_set = false;
}

void test_interface::set_file_name() {
	file_name.str("");
	file_name << "LT_";
	file_name << (*stage)[0][0]->get_str();

	if (active_Old_Fixture != 0) {
		file_name << "_";
		for (int i = 0; i < 8; i++) {
			if ((active_Old_Fixture & (1 << i)) == 1) {
				file_name << 1;
			}
			else {
				file_name << 0;
			}
		}
	}
	if (active_channel != 0) {
		file_name << "_";
		if ((active_channel & 0b1) != 0) {
			file_name << "1";
		}
		if ((active_channel & 0b10) != 0) {
			file_name << "2";
		}
		if ((active_channel & 0b100) != 0) {
			file_name << "3";
		}
	}
	if (active_param != 0) {
		file_name << "_";
		if ((active_param & 0b1000000000) != 0) {
			file_name << "B";
		}
		if ((active_param & 0b100000000) != 0) {
			file_name << "A";
		}
		if ((active_param & 0b10000000) != 0) {
			file_name << "S";
		}
		if ((active_param & 0b1000000) != 0) {
			file_name << "H";
		}
		if ((active_param & 0b100000) != 0) {
			file_name << "N";
		}
		if ((active_param & 0b10000) != 0) {
			file_name << "M";
		}
		if ((active_param & 0b1000) != 0) {
			file_name << "T";
		}
		if ((active_param & 0b100) != 0) {
			file_name << "G";
		}
		if ((active_param & 0b10) != 0) {
			file_name << "F";
		}
		if ((active_param & 0b1) != 0) {
			file_name << "W";
		}
	}
}

string test_interface::get_file_name() {
	return file_name.str();
}

void test_interface::set_active_Old_Fixture(int control) {
	active_Old_Fixture &= (control & 0xFF);
}

uint8_t test_interface::get_active_Old_Fixture() {
	return active_Old_Fixture;
}

void test_interface::set_active_param(int control) {
	active_param &= (control & 0xFFFF);
}

uint16_t test_interface::get_active_param() {
	return active_param;
}

void test_interface::set_active_channel(int control) {
	active_channel &= (control & 0xFF);
}

uint8_t test_interface::get_active_channel() {
	return active_channel;
}

test_interface::~test_interface()
{
}
/*/
void test_interface::apply_active_Old_Fixture() {
	for (auto i = 0; i < 8; i++) {
		if ((active_Old_Fixture & (1 << (7 - i))) == 0) {
			Old_Fixture[i]->set_rgb2(0, 0, 0, 0, 0, 0);
		}
	}
}
*/
/*
void test_interface::apply_active_channel() {
	for (auto i = 0; i < 8; i++) {
		if ((active_channel & 0b1) == 0) {
			Old_Fixture[i]->set_blue(0);
			Old_Fixture[i]->set_blue2(0);
		}
		if ((active_channel & 0b10) == 0) {
			Old_Fixture[i]->set_green(0);
			Old_Fixture[i]->set_green2(0);
		}
		if ((active_channel & 0b100) == 0) {
			Old_Fixture[i]->set_red(0);
			Old_Fixture[i]->set_red2(0);
		}
	}
}
*/
void test_interface::apply_active_param() {
	for (auto i = 9; i > 0; i--) {
		if ((active_param & (1 << i)) == 0) {
			if (i == 0) {
				prop[9].valueA = int(param_default[9]);
				prop[9].valueB = int(param_default[10]);
			}
			else if (i == 7) {
				prop[2].valueA = int(param_default[2]);
			}
			else {
				prop[9 - i].absValue = param_default[9 - i];
			}
			camera->SetProperty(prop + 9 - i);
		}
	}
}

void test_interface::set_light_value(int r, int g, int b, int r2, int g2, int b2) {
	if ((active_channel & 0b100) == 0) {
		r = 0; 
		r2 = 0;
	}
	if ((active_channel & 0b10) == 0) {
		g = 0;
		g2 = 0;
	}
	if ((active_channel & 0b1) == 0) {
		b = 0;
		b2 = 0;
	}
	light_value[0] = r;
	light_value[1] = r2;
	light_value[2] = g;
	light_value[3] = g2;
	light_value[4] = b;
	light_value[5] = b2;
	for (auto i = 0; i < 8; i++) {
		if ((active_Old_Fixture & (1 << (7 - i))) != 0) {
			//Old_Fixture[i]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
}

void test_interface::set_param_value(int index, float A, float B) {
	if ((active_param & (1 << (9 - index))) == 0) {}
	else {
		if (index == 9) {
			(prop + index)->valueA = int(A);
			(prop + index)->valueB = int(B);
		}
		else if (index == 2) {
			(prop + index)->valueA = int(A);
		}
		else {
			(prop + index)->absValue = A;
		}
		camera->SetProperty(prop + index);
	}
}

void test_interface::set_camera_step(float low, float high, int para, int numOfPic) {
	if ((active_param & (1 << para)) == 0) {
		cout << "Property " << para << " not active" << endl;
	}
	else {
		param_to_set = para;
		min = low;
		max = high;
		nCapture = numOfPic;
		stepsize = (max - min) / (nCapture - 1);
		if (para == 9) {
			stepsize = int(stepsize);
		}
		param_set = true;
		channel_set = false;
	}

}

void test_interface::set_light_step(float low, float high, int channel, int numOfPic) {
	if (channel > 0) {
		if ((active_channel & channel) == 0) {
			cout << "Channel " << channel << " not active" << endl;
		}
		else {
			channel_to_set = channel;
			min = low;
			max = high;
			nCapture = numOfPic;
			stepsize = (max - min) / (nCapture - 1);
			channel_set = true;
			param_set = false;
		}
	}
	else {
		if ((active_channel & 0b111) == 0) {
			cout << "Channels not active" << endl;
		}
		else {
			channel_to_set = channel;
			min = low;
			max = high;
			nCapture = numOfPic;
			stepsize = (max - min) / (nCapture - 1);
			channel_set = true;
			param_set = false;
		}
	}
}