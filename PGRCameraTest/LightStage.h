#pragma once
#define _CRT_SECURE_NO_WARNINGS
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

	/*set the same 16-bit setting to all lights by value*/
	void adjustAll(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2);
	/*set the same 16-bit setting to all lights by array*/
	void adjustAll(uint8_t config[6]);
	/*set the same 16-bit setting to all lights by array*/
	void adjustAll(const uint8_t config[6]);

	/*apply the same 16-bit setting to all RGB lights by value*/
	void adjustRGB(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2);
	/*apply the same 16-bit setting to all RGB lights by array*/
	void adjustRGB(uint8_t config[6]);
	/*apply the same 16-bit setting to all RGB lights by array*/
	void adjustRGB(const uint8_t config[6]);

	/*apply the same 16-bit setting to all white lights by value*/
	void adjustWhite(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2);
	/*apply the same 16-bit setting to all white lights by array*/
	void adjustWhite(uint8_t config[6]);
	/*apply the same 16-bit setting to all white lights by array*/
	void adjustWhite(const uint8_t config[6]);

	/*apply the same 16-bit setting to a real arc by value*/
	void adjustRealArc(int index, uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2, int selection = -1);
	/*apply the same 16-bit setting to a real arc by array*/
	void adjustRealArc(int index, uint8_t config[6], int selection = -1);
	/*apply the same 16-bit setting to a real arc by array*/
	void adjustRealArc(int index, const uint8_t config[6], int selection = -1);

	/*apply the same 16-bit setting to a virtual arc by value*/
	void adjustVirtualArc(int index, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	/*apply the same 16-bit setting to a virtual arc by array*/
	void adjustVirtualArc(int index, uint8_t config[6], int selection = -1);
	/*apply the same 16-bit setting to a virtual arc by array*/
	void adjustVirtualArc(int index, const uint8_t config[6], int selection = -1);

	/*Save existing lamp setting to file*/
	void saveMap(char * filename);
	/*Load lamp setting from file*/
	bool loadMap(char * filename);
	/*Load voronoi-format map from file*/
	bool loadVoronMap(char * filename);

	/*Get the pointer to default 16-bit setting*/
	uint8_t * getDefault();

	/*Set the default value setting*/
	void setDefault(uint8_t (&config)[6]);
	/*Set the default value setting*/
	void LightStage::setDefault(uint8_t * config);


	//Functions used during testing stage//
	/*Allows rotation of current setting map by i virtual arcs*/
	void rotation(int i);

	/*Copy setting map from the buffer matrix to main matrix*/
	void readBuffer();
	/*Copy setting map from main matrix to buffer matrix*/
	void setBuffer();
	/*Copy setting map from a setting matrix to buffer matrix*/
	void setBuffer(uint8_t * source[12][28]);

	float getScale();

	void setScale(float input);

	uint8_t * getviaMatrix(int arc, int index);

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
	uint8_t defaultConfig[6] = { 4, 0, 4, 128, 3, 0 };
	float scale = 1.0f;
};
