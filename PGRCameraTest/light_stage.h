#pragma once
#include "kinet16.h"
#include <fstream>
#include <cstring>

class LightStage
{
public:
	LightStage();
	~LightStage();
	/*access lamp via real arc coordinates*/
	Old_FixtureRGB16 ** operator[](int i);
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
	/*access lamp via virtual arc coordinates*/
	Old_FixtureRGB16 * operator()(int arc, int index);
	void rotation(int i);
	void readRealBuffer();
	void readVirtualBuffer();
	void setRealBuffer();
	void setVirtualBuffer();
	void setRealBuffer(uint8_t * source[12][28]);
	void setVirtualBuffer(uint8_t * source[12][28]);
	void saveMap(string filename);
	void loadMap(string filename);
	const uint8_t * getDeault();
private:
	Old_PowerSupply ** colour;
	Old_PowerSupply ** white;
	Old_FixtureRGB16 *(*lamp)[28];
	uint8_t* matrix[12][28];
	uint8_t realBuffer[12][28][6];
	uint8_t realOffset;
	uint8_t virtualOffset;
	uint8_t virtualBuffer[12][28][6];
	string ip;
	const uint8_t defaultConfig[6] = {4, 4, 3, 0, 128, 0};
};
