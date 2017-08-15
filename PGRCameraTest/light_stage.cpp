#include "light_stage.h"


LightStage::LightStage()
{
	colour = new Old_PowerSupply*[12];
	white = new Old_PowerSupply*[12];
	lamp = new Old_FixtureRGB16*[12][28];
	ip = "10.37.211.0";

	for (size_t i = 0; i < 12; i++) {
		ip.replace(10, ip.size() - 1, std::to_string(2 * i));
		colour[i] = new Old_PowerSupply(ip);
		ip.replace(10, ip.size() - 1, std::to_string(2 * i + 1));
		white[i] = new Old_PowerSupply(ip);
		for (size_t j = 0; j < 28;) {
			lamp[i][j] = new Old_FixtureRGB16(3 * j);
			colour[i]->addOld_Fixture(lamp[i][j]);
			matrix[i][j] = lamp[i][j]->get_config();
			j++;
			lamp[i][j] = new Old_FixtureRGB16(3 * (j - 1));
			white[i]->addOld_Fixture(lamp[i][j]);
			matrix[i][j] = lamp[i][j]->get_config();
			j++;


		}
		colour[i]->go();
		white[i]->go();
	}
	setRealBuffer();
	setVirtualBuffer();
	realOffset = 0;
	virtualOffset = 0;
}


LightStage::~LightStage()
{
	delete[] colour;
	delete[] white;
	delete[] lamp;
	delete[] matrix;

}

Old_FixtureRGB16 ** LightStage::operator[](int i)
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
		white[(i - 1) / 2 + 1]->go();
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

void LightStage::adjustAll(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			lamp[i][j]->set_config(config);
		}
	}
	go();
}

void LightStage::adjustAll(const uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			lamp[i][j]->set_rgb2(*config, *(config + 1), *(config + 2), *(config + 3), *(config + 4), *(config + 5));
		}
	}
	go();
}

void LightStage::adjustRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			lamp[i][2 * j]->set_rgb2(r, g, b, r2, g2, b2);
		}
		colour[i]->go();
	}
}

void LightStage::adjustRGB(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			lamp[i][2 * j]->set_config(config);
		}
		colour[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			lamp[i][2 * j + 1]->set_rgb2(r, g, b, r2, g2, b2);
		}
		white[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			lamp[i][2 * j + 1]->set_config(config);
		}
		white[i]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int selection)
{
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			lamp[index][i * 2]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			lamp[index][i * 2 + 1]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	if (selection == -1) {
		colour[index]->go();
		white[index]->go();
	}
	else if (selection == 0) {
		colour[index]->go();
	}
	else if (selection == 1) {
		white[index]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t config[6], int selection)
{
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			lamp[index][i * 2]->set_config(config);
		}
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			lamp[index][i * 2 + 1]->set_config(config);
		}
	}
	if (selection == -1) {
		colour[index]->go();
		white[index]->go();
	}
	else if (selection == 0) {
		colour[index]->go();
	}
	else if (selection == 1) {
		white[index]->go();
	}
}

void LightStage::adjustVirtualArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (int j = 0; j < 14; j++) {
		this->operator()(index, j)->set_rgb2(r, g, b, r2, g2, b2);
	}
	colour[index / 2]->go();
	white[index / 2]->go();
}

void LightStage::adjustVirtualArc(int index, uint8_t config[6])
{
	for (int j = 0; j < 14; j++) {
		this->operator()(index, j)->set_config(config);
	}
	colour[index / 2]->go();
	white[index / 2]->go();
}

Old_FixtureRGB16 * LightStage::operator()(int arc, int index)
{
	if ((arc < 0) || (arc > 23)) {
		std::cout << "Arc out of range" << std::endl;
		return nullptr;
	}
	if ((index < 0) || (index > 13)) {
		std::cout << "Index out of range" << std::endl;
		return nullptr;
	}
	if (arc % 2 == 1) {
		index += 14;
	}
	arc = arc / 2;
	return lamp[arc][index];
}

void LightStage::rotation(int dir)
{
	setRealBuffer();
	
	if (dir == 0) {
		realOffset = 11;
		readRealBuffer();
	}
	else {
		realOffset = 1;
		readRealBuffer();
	}
	go();
}

void LightStage::readRealBuffer()
{
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 28; j++) {
			for (int k = 0; k < 6; k++) {
				if (matrix[i][j][k] != realBuffer[(i + realOffset) % 12][j][k]) {
					matrix[i][j][k] = realBuffer[(i + realOffset) % 12][j][k];
				}
			}
		}
	}
}

void LightStage::readVirtualBuffer()
{
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 28; j++) {
			for (int k = 0; k < 6; k++) {
				if (matrix[i][j][k] != virtualBuffer[(i + virtualOffset) % 12][j][k]) {
					matrix[i][j][k] = virtualBuffer[(i + virtualOffset) % 12][j][k];
				}
			}
		}
	}
}

void LightStage::setRealBuffer()
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				if (matrix[i][j][k] != realBuffer[i][j][k]) {
					realBuffer[i][j][k] = matrix[i][j][k];
				}
			}
		}
	}
}

void LightStage::setVirtualBuffer()
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				if (matrix[i][j][k] != virtualBuffer[i][j][k]) {
					virtualBuffer[i][j][k] = matrix[i][j][k];
				}
			}
		}
	}
}

void LightStage::setRealBuffer(uint8_t * source[12][28])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				realBuffer[i][j][k] = source[i][j][k];
			}
		}
	}
}

void LightStage::setVirtualBuffer(uint8_t * source[12][28])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				virtualBuffer[i][j][k] = source[i][j][k];
			}
		}
	}
}

void LightStage::saveMap(string filename)
{
	std::ofstream outfile;
	outfile.open(filename.c_str());
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				outfile << unsigned(matrix[i][j][k]) << " ";
			}
			outfile << "\n";
		}
		outfile << "\n";
	}
	outfile.close();
}

void LightStage::loadMap(string filename)
{
	std::ifstream infile;
	int temp;
	infile.open(filename.c_str());
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				infile >> temp;
				matrix[i][j][k] = temp;
			}
		}
	}
	infile.close();
}

const uint8_t * LightStage::getDeault()
{
	return defaultConfig;
}
