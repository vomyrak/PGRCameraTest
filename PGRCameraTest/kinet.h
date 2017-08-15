//
//  kinet.h
//
//	modified by Husheng Deng on 30 Jun 17 for 16-bit control
//
//  Created by Michael Dewberry on 12/19/12.
//
//  Port of kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//

#ifndef _KINET_H
#define _KINET_H

#include <list>
#include <string>
#include <stdint.h>
#include <sstream>
#include <iostream>

using std::list;
using std::string;


// Just a data container for now. Todo: move all protocol logic into this class
class Old_KinetProtocol
{
public:
    Old_KinetProtocol(size_t headerSize, size_t dataSize, size_t numChannels, const unsigned char* headerBytes)
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

class Old_Fixture
{
public:
    virtual void updateFrame(uint8_t* packets) const = 0;
    virtual std::string getName() const = 0;
};

class Old_FixtureRGB : public Old_Fixture
{
public:
    Old_FixtureRGB(int address, uint8_t r=0, uint8_t g=0, uint8_t b=0);
    int getAddress() const { return _address; }
    void setAddress(int address) { _address = address; }
   
    virtual void updateFrame(uint8_t* packets) const;
	
    virtual std::string getName() const;
    
    unsigned char get_red() const;
    unsigned char get_green() const;
    unsigned char get_blue() const;
    
    void set_red(uint8_t r);
    void set_green(uint8_t g);
    void set_blue(uint8_t b);
    
    void set_rgb(uint8_t r, uint8_t g, uint8_t b);

	//newly implemented
	const Old_KinetProtocol * get_protocol() const;

protected:
    int _address;
    unsigned char _values[3];
	const Old_KinetProtocol *inter;
};

class Old_FixtureTile : public Old_Fixture
{
public:
    Old_FixtureTile(int startChannel, int width = 12, int height = 12);
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
    int _Old_FixtureWidth;
    int _Old_FixtureHeight;
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

class Old_FixtureTile6 : public Old_FixtureTile
{
public:
    Old_FixtureTile6(int startChannel, int width = 12, int height = 12);
    virtual void updateFrame(uint8_t* packets) const;
};

class Old_FixtureTileDC : public Old_FixtureTile
{
public:
    Old_FixtureTileDC(int startChannel, int width = 12, int height = 12);
    virtual void updateFrame(uint8_t* packets) const;
};

class Old_PowerSupply
{
public:
    Old_PowerSupply();
    Old_PowerSupply(const string strHost, const string strPort = "6038");
    ~Old_PowerSupply();
    
    bool connect(const string strHost, const string strPort = "6038");
    
    string getHost();
    int getPort();
    
    void addOld_Fixture(Old_Fixture* pFix);
    void clearOld_Fixtures();
    int getOld_FixtureCount() const { return _Old_Fixtures.size(); }
    
    void go();
    
    void switchToNewProtocol();
        
protected:
    void initializeBuffer();
    
    bool _connected;
    int _socket;
    list<Old_Fixture*> _Old_Fixtures;
    uint8_t* _frame;
    string _host;
    int _port;
    const Old_KinetProtocol* _proto;		//added for 16-bit control
};

#endif