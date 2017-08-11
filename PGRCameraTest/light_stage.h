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
	FixtureRGB16 ** operator[](int i);
	/*apply setting to one particular psd unit*/
	void go(int i);
	/*apply setting to all psd units*/
	void go();
	/*apply the same 6-channel setting to all lights*/
	void adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	/*access lamp via virtual arc coordinates*/
	FixtureRGB16 * operator()(int arc, int index);
	void rotation(int i);
	void readRealBuffer();
	void readVirtualBuffer();
	void setRealBuffer();
	void setVirtualBuffer();
	void setRealBuffer(uint8_t * source[12][28]);
	void setVirtualBuffer(uint8_t * source[12][28]);
	void saveMap(string filename);
	void loadMap(string filename);
private:
	PowerSupply ** colour;
	PowerSupply ** white;
	FixtureRGB16 *(*lamp)[28];
	uint8_t* matrix[12][28];
	uint8_t realBuffer[12][28][6];
	uint8_t realOffset;
	uint8_t virtualOffset;
	uint8_t virtualBuffer[12][28][6];
	string ip;
};
