#pragma once
//
//  Fixture.h
//
//	Created by Husheng Deng on 15 August 17 for
//  Imperial College London lightstage project.
//
//  Created based on kinet.h by Michael Dewberry on 12/19/12.
//  With modifications that specifically suit the need of the project.
//
//  Port of kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//
#ifndef _FIXTURE_H
#define _FIXTURE_H

#include <list>
#include <string>
#include <stdint.h>
#include <sstream>
#include <iostream>

using std::list;
using std::string;

class KinetProtocol
{
public:
	KinetProtocol(size_t headerSize, size_t dataSize, size_t numChannels, const unsigned char* headerBytes)
		: _headerSize(headerSize), _dataSize(dataSize), _numChannels(numChannels), _headerBytes(headerBytes)
	{
	}

	size_t getHeaderSize() const { return _headerSize; }
	size_t getDataSize() const { return _dataSize; }
	size_t getNumChannels() const { return _numChannels; }
	size_t getPacketSize() const { return _headerSize + _dataSize; }
	size_t getBufferSize() const { return getPacketSize() * _numChannels; }
	const unsigned char* getHeaderBytes() const { return _headerBytes; }

protected:
	size_t _headerSize;
	size_t _dataSize;
	size_t _numChannels;
	const unsigned char* _headerBytes;
};

class Fixture
{
public:
	virtual void updateFrame(uint8_t* packets) const = 0;
	virtual std::string getName() const = 0;
};

class FixtureLED : public Fixture
{
public:

	/*8-bit setting constructor that takes only one value for RGB each*/
	FixtureLED(int address, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);

	/*16-bit setting constructor that takes two values for RGB each*/
	FixtureLED(int address, uint8_t * ptr = nullptr, uint8_t r = 0, uint8_t r2 = 0, uint8_t g = 0, uint8_t g2 = 0, uint8_t b = 0, uint8_t b2 = 0);
	
	/*Returns the address of the fixture*/
	int getAddress() const { return _address; }
	/*Sets the address of the fixture*/
	void setAddress(int address) { _address = address; }

	/*Inherit from superclass*/
	virtual void updateFrame(uint8_t* packets) const;
	/*Inherit from superclass*/
	virtual std::string getName() const;

	/*Returns the pointer to the RGB array of the fixture*/
	uint8_t * getValue();

	/*Copy 16-bit current RGB values to array*/
	void getValue(uint8_t(&table)[6]);

	/*Copy 8-bit current RGB values to array*/
	void getValue(uint8_t(&table)[3]);


	
	/*Return MSB of red value*/
	uint8_t getRed() const;
	/*Return MSB of green value*/
	uint8_t getGreen() const;
	/*Return MSB of blue value*/
	uint8_t getBlue() const;
	/*Return LSB of red value*/
	uint8_t getRed2() const;
	/*Return LSB of green value*/
	uint8_t getGreen2() const;
	/*Return LSB of blue value*/
	uint8_t getBlue2() const;


	/*Set MSB of red value*/
	void setRed(uint8_t r);
	/*Set MSB of green value*/
	void setGreen(uint8_t g);
	/*Set MSB of blue value*/
	void setBlue(uint8_t b);
	/*Set LSB of red value*/
	void setRed2(uint8_t r2);
	/*Set LSB of green value*/
	void setGreen2(uint8_t g2);
	/*Set LSB of blue value*/
	void setBlue2(uint8_t b2);
	/*Set RGB values to fixture from array*/
	void setValue(uint8_t config[6]);
	/*Set RGB values to fixture from array*/
	void setValue(const uint8_t config[6]);
	/*Set 8-bit RGB values by value*/
	void setRGB(uint8_t r, uint8_t g, uint8_t b);
	/*Set 16-bit RGB values by value*/
	void setValue(uint8_t r, uint8_t r2, uint8_t g, uint8_t g2, uint8_t b, uint8_t b2);
	
	/*Print RGB values as a string*/
	string getStr() const;

	/*Return the pointer to the RGB array of the fixture*/
	uint8_t * getValuePtr();
	/*Set the pointer to the RGB array of the fixture*/
	void setValuePtr(uint8_t * ptr);

private:
	int _address;
	uint8_t * _values;
	int channelNumber;
};

class PowerSupply
{
public:
	PowerSupply();
	PowerSupply(const string strHost, const string strPort = "6038");
	~PowerSupply();

	bool connect(const string strHost, const string strPort = "6038");

	string getHost();
	int getPort();

	void addFixture(Fixture* pFix);
	void clearFixtures();
	int getFixtureCount() const { return _fixtures.size(); }

	void go();

	void switchToNewProtocol();

protected:
	void initializeBuffer();

	bool _connected;
	int _socket;
	list<Fixture*> _fixtures;
	uint8_t* _frame;
	string _host;
	int _port;
	const KinetProtocol* _proto;		//added for 16-bit control
};


//Original classes not used in the project
/*
class FixtureTile : public Fixture
{
public:
    FixtureTile(int startChannel, int width = 12, int height = 12);
    int getChannel() const { return _startChannel; }
    void setChannel(int channel) { _startChannel = channel; }
    void setVideoRect(int x, int y, int w, int h);
    void setSourceData(const uint8_t* sourceData, int sourceWidth, int sourceHeight, int sourceChannels);
    int getRotation() const { return _rotation; }
    void setRotation(int degrees) { _rotation = degrees; }

    virtual void updateFrame(uint8_t* packets) const;
    virtual std::string getName() const;

protected:
    int _startChannel;
    int _fixtureWidth;
    int _fixtureHeight;
    int _videoX;
    int _videoY;
    int _videoW;
    int _videoH;
    const uint8_t* _sourceData;
    int _sourceWidth;
    int _sourceHeight;
    int _sourceChannels;
    int _rotation;
};

class FixtureTile6 : public FixtureTile
{
public:
    FixtureTile6(int startChannel, int width = 12, int height = 12);
    virtual void updateFrame(uint8_t* packets) const;
};

class FixtureTileDC : public FixtureTile
{
public:
    FixtureTileDC(int startChannel, int width = 12, int height = 12);
    virtual void updateFrame(uint8_t* packets) const;
};
*/

#endif