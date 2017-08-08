#include "light_stage.h"


LightStage::LightStage()
{
	colour = new PowerSupply*[12];
	white = new PowerSupply*[12];
	lamp = new FixtureRGB16*[12][28];
	ip = "10.37.211.0";

	for (size_t i = 0; i < 12; i++) {
		ip.replace(10, ip.size() - 1, std::to_string(2 * i));
		colour[i] = new PowerSupply(ip);
		ip.replace(10, ip.size() - 1, std::to_string(2 * i + 1));
		white[i] = new PowerSupply(ip);
	}

	for (size_t i = 0; i < 12; i++) {
		for (size_t j = 0; j < 28;) {
			lamp[i][j] = new FixtureRGB16(3 * j, 0, 0, 0, 0, 0, 0);
			colour[i]->addFixture(lamp[i][j]);
			j++;
			lamp[i][j] = new FixtureRGB16(3 * (j - 1), 0, 0, 0, 0, 0, 0);
			white[i]->addFixture(lamp[i][j]);
			j++;

		}
		colour[i]->go();
		white[i]->go();
	}
}


LightStage::~LightStage()
{
	delete[] colour;
	delete[] white;
	delete[] lamp;
}

FixtureRGB16 ** LightStage::operator[](int i)
{
	return lamp[i];
}

void LightStage::go(int i)
{
	if ((i < 0) || (i > 23)) {
		std::cout << "PDS index out of range" << std::endl;
	}
	if (i % 2 == 0) {
		colour[i / 2]->go();
	}
	else {
		colour[(i - 1) / 2 + 1]->go();
	}
}

void LightStage::go()
{
	for (size_t i = 0; i < 12; i++) {
		colour[i]->go();
		white[i]->go();
	}
}

void LightStage::adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2) {
	for (size_t i = 0; i < 12; i++) {
		for (size_t j = 0; j < 28; j++) {
			lamp[i][j]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	go();
}
