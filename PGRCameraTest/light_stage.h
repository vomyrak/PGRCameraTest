#pragma once
#include "Fixture.h"
#include <fstream>
#include <cstring>

class LightStage
{
public:
	LightStage();
	~LightStage();
	/*access lamp via real arc coordinates*/
	FixtureRGB ** operator[](int i);
	/*access lamp via virtual arc coordinates*/
	FixtureRGB * operator()(int arc, int index);
	/*apply setting to one particular psd unit*/
	void go(int i);
	/*apply setting to all psd units*/
	void go();
	/*apply the same 6-channel setting to all lights*/
	void adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustAll(uint8_t config[6]);
	void adjustAll(const uint8_t config[6]);

	void adjustRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustRGB(uint8_t config[6]);

	void adjustWhite(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustWhite(uint8_t config[6]);

	void adjustRealArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int selection = -1);
	void adjustRealArc(int index, uint8_t config[6], int selection = -1);

	void adjustVirtualArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustVirtualArc(int index, uint8_t config[6]);


	void rotation(int i);
	void readBuffer();
	void setBuffer();
	void setBuffer(uint8_t * source[12][28]);

	void saveMap(string filename);
	void loadMap(string filename);

	uint8_t * getDeault();
private:
	PowerSupply ** Colour;
	PowerSupply ** White;
	FixtureRGB *(*Lamp)[28];
	uint8_t Matrix[12][28][6];
	uint8_t MatrixOffsetH;
	uint8_t MatrixOffsetV;
	uint8_t Buffer[12][28][6];
	uint8_t bufferOffset;
	string ip;
	uint8_t defaultConfig[6] = {4, 4, 3, 0, 128, 0};
};
