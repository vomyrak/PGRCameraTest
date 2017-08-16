#include "light_stage.h"

LightStage::LightStage()
{
	Colour = new PowerSupply*[12];
	White = new PowerSupply*[12];
	Lamp = new FixtureRGB*[12][28];
	ip = "10.37.211.0";

	for (size_t i = 0; i < 12; i++) {
		ip.replace(10, ip.size() - 1, std::to_string(2 * i));
		Colour[i] = new PowerSupply(ip);
		ip.replace(10, ip.size() - 1, std::to_string(2 * i + 1));
		White[i] = new PowerSupply(ip);
		for (size_t j = 0; j < 28;) {
			Lamp[i][j] = new FixtureRGB(3 * j, Matrix[i][j]);
			Colour[i]->addFixture(Lamp[i][j]);
			j++;
			Lamp[i][j] = new FixtureRGB(3 * (j - 1), Matrix[i][j]);
			White[i]->addFixture(Lamp[i][j]);
			j++;


		}
		Colour[i]->go();
		White[i]->go();
	}
	MatrixOffsetH = 0;
	MatrixOffsetV = 0;
	//setBuffer();
	//setVirtualBuffer();
	//bufferOffset = 0;
	//virtualOffset = 0;
}


LightStage::~LightStage()
{
	delete[] Colour;
	delete[] White;
	delete[] Lamp;
	delete[] Matrix;

}

FixtureRGB ** LightStage::operator[](int i)
{
	return Lamp[i];
}

void LightStage::go(int i)
{
	if ((i < 0) || (i > 23)) {
		std::cout << "PDS index out of range" << std::endl;
	}
	if (i % 2 == 0) {
		Colour[i / 2]->go();
	}
	else {
		White[(i - 1) / 2 + 1]->go();
	}
}

void LightStage::go()
{
	for (size_t i = 0; i < 12; i++) {
		Colour[i]->go();
		White[i]->go();
	}
}

void LightStage::adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2) {
	for (size_t i = 0; i < 12; i++) {
		for (size_t j = 0; j < 28; j++) {
			Lamp[i][j]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	go();
}

void LightStage::adjustAll(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			Lamp[i][j]->set_config(config);
		}
	}
	go();
}

void LightStage::adjustAll(const uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			Lamp[i][j]->set_rgb2(*config, *(config + 1), *(config + 2), *(config + 3), *(config + 4), *(config + 5));
		}
	}
	go();
}

void LightStage::adjustRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j]->set_rgb2(r, g, b, r2, g2, b2);
		}
		Colour[i]->go();
	}
}

void LightStage::adjustRGB(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j]->set_config(config);
		}
		Colour[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j + 1]->set_rgb2(r, g, b, r2, g2, b2);
		}
		White[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t config[6])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j + 1]->set_config(config);
		}
		White[i]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int selection)
{
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2 + 1]->set_rgb2(r, g, b, r2, g2, b2);
		}
	}
	if (selection == -1) {
		Colour[index]->go();
		White[index]->go();
	}
	else if (selection == 0) {
		Colour[index]->go();
	}
	else if (selection == 1) {
		White[index]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t config[6], int selection)
{
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2]->set_config(config);
		}
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2 + 1]->set_config(config);
		}
	}
	if (selection == -1) {
		Colour[index]->go();
		White[index]->go();
	}
	else if (selection == 0) {
		Colour[index]->go();
	}
	else if (selection == 1) {
		White[index]->go();
	}
}

void LightStage::adjustVirtualArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	for (int j = 0; j < 14; j++) {
		this->operator()(index, j)->set_rgb2(r, g, b, r2, g2, b2);
	}
	Colour[index / 2]->go();
	White[index / 2]->go();
}

void LightStage::adjustVirtualArc(int index, uint8_t config[6])
{
	for (int j = 0; j < 14; j++) {
		this->operator()(index, j)->set_config(config);
	}
	Colour[index / 2]->go();
	White[index / 2]->go();
}

FixtureRGB * LightStage::operator()(int arc, int index)
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
	return Lamp[arc][index];
}

void LightStage::rotation(int dir)
{
	setBuffer();
	
	if (dir == 0) {
		if (MatrixOffsetH == 0) {
			MatrixOffsetH += 12;
		}
		MatrixOffsetH -= 1;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 28; j++) {
				Lamp[i][j]->setValuePtr(Matrix[(i + MatrixOffsetH) % 12][j]);
			}
		}
	}
	else {
		if (MatrixOffsetH == 12) {
			MatrixOffsetH -= 12;
		}
		MatrixOffsetH += 1;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 28; j++) {
				Lamp[i][j]->setValuePtr(Matrix[(i + MatrixOffsetH) % 12][j]);
			}
		}
	}
	go();
}

void LightStage::readBuffer()
{
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 28; j++) {
			Lamp[i][j]->setValuePtr(Buffer[i][j]);
		}
	}
}



void LightStage::setBuffer()
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				if (Matrix[i][j][k] != Buffer[i][j][k]) {
					Buffer[i][j][k] = Matrix[i][j][k];
				}
			}
		}
	}
}

void LightStage::setBuffer(uint8_t * source[12][28])
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				Buffer[i][j][k] = source[i][j][k];
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
				outfile << unsigned(Matrix[i][j][k]) << " ";
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
				Matrix[i][j][k] = temp;
			}
		}
	}
	infile.close();
}

uint8_t * LightStage::getDeault()
{
	return defaultConfig;
}
