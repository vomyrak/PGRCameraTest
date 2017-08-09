#pragma once
#include "kinet16.h"

class LightStage
{
public:
	LightStage();
	~LightStage();
	FixtureRGB16 ** operator[](int i);
	/*apply setting to one particular psd unit*/
	void go(int i);
	/*apply setting to all psd units*/
	void go();
	/*apply the same 6-channel setting to all lights*/
	void adjustAll(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);

private:
	PowerSupply ** colour;
	PowerSupply ** white;
	FixtureRGB16 *(*lamp)[28];
	string ip;
};