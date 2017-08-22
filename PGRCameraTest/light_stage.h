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
	FixtureLED ** operator[](int i);

	/*access lamp via virtual arc coordinates*/
	FixtureLED * operator()(int arc, int index);

	/*apply setting to one particular psd unit*/
	void go(int i);

	/*apply setting to all psd units*/
	void go();

	/*apply the same 6-channel setting to all lights*/
	void adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustAll(uint8_t config[6]);
	void adjustAll(const uint8_t config[6]);

	/*apply the same 6-channel setting to all RGB lights*/
	void adjustRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustRGB(uint8_t config[6]);
	void adjustRGB(const uint8_t config[6]);

	/*apply the same 6-channel setting to all white lights*/
	void adjustWhite(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustWhite(uint8_t config[6]);
	void adjustWhite(const uint8_t config[6]);

	/*apply the same 6-channel setting to a real arc*/
	void adjustRealArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int selection = -1);
	void adjustRealArc(int index, uint8_t config[6], int selection = -1);
	void adjustRealArc(int index, const uint8_t config[6], int selection = -1);

	/*apply the same 6-channel setting to a virtual arc*/
	void adjustVirtualArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void adjustVirtualArc(int index, uint8_t config[6], int selection = -1);
	void adjustVirtualArc(int index, const uint8_t config[6], int selection = -1);

	/*Functions used during testing stage*/
	void rotation(int i);
	void readBuffer();
	void setBuffer();
	void setBuffer(uint8_t * source[12][28]);

	/*Save existing lamp setting to file*/
	void saveMap(string filename);

	/*Load lamp setting from file*/
	void loadMap(string filename);

	/*Get the pointer to default configuration*/
	uint8_t * getDeault();

private:
	PowerSupply ** Colour;
	PowerSupply ** White;
	FixtureLED *(*Lamp)[28];
	uint8_t Matrix[12][28][6];
	uint8_t MatrixOffsetH;
	uint8_t MatrixOffsetV;
	uint8_t Buffer[12][28][6];
	uint8_t bufferOffset;
	string ip;
	uint8_t defaultConfig[6];
};
