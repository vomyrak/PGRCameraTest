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
		for (size_t j = 0; j < 28;) {
			lamp[i][j] = new FixtureRGB16(3 * j, 0, 0, 0, 0, 0, 0);
			colour[i]->addFixture(lamp[i][j]);
			matrix[i][j] = lamp[i][j]->get_config();
			j++;
			lamp[i][j] = new FixtureRGB16(3 * (j - 1), 0, 0, 0, 0, 0, 0);
			white[i]->addFixture(lamp[i][j]);
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

FixtureRGB16 * LightStage::operator()(int arc, int index)
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
