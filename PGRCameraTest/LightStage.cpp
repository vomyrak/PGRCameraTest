#include "LightStage.h"

LightStage::LightStage()
{
	Colour = new PowerSupply*[12];
	White = new PowerSupply*[12];
	Lamp = new FixtureLED*[12][28];
	ip = "10.37.211.0";

	for (size_t i = 0; i < 12; i++) {
		ip.replace(10, ip.size() - 1, std::to_string(2 * i));
		Colour[i] = new PowerSupply(ip);
		ip.replace(10, ip.size() - 1, std::to_string(2 * i + 1));
		White[i] = new PowerSupply(ip);
		for (size_t j = 0; j < 28;) {
			Lamp[i][j] = new FixtureLED(3 * j, Matrix[i][j]);
			Colour[i]->addFixture(Lamp[i][j]);
			j++;
			Lamp[i][j] = new FixtureLED(3 * (j - 1), Matrix[i][j]);
			White[i]->addFixture(Lamp[i][j]);
			j++;
		}
		Colour[i]->go();
		White[i]->go();
	}
	MatrixOffsetH = 0;
	MatrixOffsetV = 0;
}


LightStage::~LightStage()
{

}

FixtureLED ** LightStage::operator[](int i)
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
		White[i / 2]->go();
	}
}

void LightStage::go()
{
	for (size_t i = 0; i < 12; i++) {
		Colour[i]->go();
		White[i]->go();
	}
}

void LightStage::adjustAll(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2) {
	for (size_t i = 0; i < 12; i++) {
		for (size_t j = 0; j < 28; j++) {
			Lamp[i][j]->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
		}
	}
	go();
}

void LightStage::adjustAll(uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			Lamp[i][j]->setValue(temp);
		}
	}
	go();
}

void LightStage::adjustAll(const uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			Lamp[i][j]->setValue(temp);
		}
	}
	go();
}

void LightStage::adjustRGB(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j]->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
		}
		Colour[i]->go();
	}
}

void LightStage::adjustRGB(uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j]->setValue(temp);
		}
		Colour[i]->go();
	}
}

void LightStage::adjustRGB(const uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j]->setValue(temp);
		}
		Colour[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2)
{
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j + 1]->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
		}
		White[i]->go();
	}
}

void LightStage::adjustWhite(uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j + 1]->setValue(temp);
		}
		White[i]->go();
	}
}

void LightStage::adjustWhite(const uint8_t config[6])
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 14; j++) {
			Lamp[i][2 * j + 1]->setValue(temp);
		}
		White[i]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2, int selection)
{
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2]->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
		}
		Colour[index]->go();
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2 + 1]->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
		}
		White[index]->go();
	}
}

void LightStage::adjustRealArc(int index, uint8_t config[6], int selection)
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2]->setValue(temp);
		}
		Colour[index]->go();
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2 + 1]->setValue(temp);
		}
		White[index]->go();
	}
}

void LightStage::adjustRealArc(int index, const uint8_t config[6], int selection)
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	if ((selection == -1) || (selection == 0)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2]->setValue(temp);
		}
		Colour[index]->go();
	}
	if ((selection == -1) || (selection == 1)) {
		for (int i = 0; i < 14; i++) {
			Lamp[index][i * 2 + 1]->setValue(temp);
		}
		White[index]->go();
	}
}

void LightStage::adjustVirtualArc(int index, uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2)
{
	for (int j = 0; j < 14; j++) {
		this->operator()(index, j)->setValue(r * scale, r2 * scale, g * scale, g2 * scale, b * scale, b2 * scale);
	}
	Colour[index / 2]->go();
	White[index / 2]->go();
}

void LightStage::adjustVirtualArc(int index, uint8_t config[6], int selection)
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	if ((selection == -1) || (selection == 0)) {
		for (int j = 0; j < 7; j++) {
			this->operator()(index, 2 * j)->setValue(temp);
		}
		Colour[index / 2]->go();
	}
	if ((selection == -1) || (selection == 1)) {
		for (int j = 0; j < 7; j++) {
			this->operator()(index, 2 * j + 1)->setValue(temp);
		}
		White[index / 2]->go();
	}
}

void LightStage::adjustVirtualArc(int index, const uint8_t config[6], int selection)
{
	uint8_t temp[6];
	for (auto i = 0; i < 6; i++) {
		temp[i] = config[i] * scale;
	}
	if ((selection == -1) || (selection == 0)) {
		for (int j = 0; j < 7; j++) {
			this->operator()(index, 2 * j)->setValue(temp);
		}
		Colour[index / 2]->go();
	}
	if ((selection == -1) || (selection == 1)) {
		for (int j = 0; j < 7; j++) {
			this->operator()(index, 2 * j + 1)->setValue(temp);
		}
		White[index / 2]->go();
	}
}

FixtureLED * LightStage::operator()(int arc, int index)
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

float LightStage::getScale()
{
	return scale;
}

void LightStage::setScale(float input)
{
	scale = input;
}



void LightStage::saveMap(char * filename)
{
	std::ofstream outfile;
	char temp[8] = {'.', 'v', 'a', 'l', 'm', 'a', 'p', '\0'};
	char temp2[9] = { 'a', '.', 'v', 'a', 'l', 'm', 'a', 'p', '\0' };
	if (strcmp(temp, filename + strlen(filename) - 7)) {
		strcat(filename, ".valmap");
	}
	outfile.open(filename);
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

bool LightStage::loadMap(char * filename)
{
	std::ifstream infile;
	int temp;
	infile.open(filename);
	if (!infile.is_open()) {
		return false;
	}
	for (auto i = 0; i < 12; i++) {
		for (auto j = 0; j < 28; j++) {
			for (auto k = 0; k < 6; k++) {
				infile >> temp;
				Matrix[i][j][k] = temp;
			}
		}
	}
	infile.close();
	return true;
}

bool LightStage::loadVoronMap(char * filename)
{
	std::ifstream infile;
	int tem;
	infile.open(filename);
	if (!infile.is_open()) {
		return false;
	}
	int no, arc, index;
	float r, g, b;
	for (auto i = 0; i < 336; i++) {
		infile >> no >> arc >> index >> r >> g >> b;
		operator()(arc, index)->setRed(r);
		operator()(arc, index)->setGreen(g);
		operator()(arc, index)->setBlue(b);
	}
	infile.close();
	return true;
}

uint8_t * LightStage::getDefault()
{
	return defaultConfig;
}

void LightStage::setDefault(uint8_t(&config)[6])
{
	for (auto i = 0; i < 6; i++) {
		defaultConfig[i] = config[i];
	}
}
