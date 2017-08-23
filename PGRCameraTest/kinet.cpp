//
//  kinet.cpp
//
//	modifed by Husheng Deng on 30 Jun 17 for 16-bit control
//
//  Created by Michael Dewberry on 12/19/12.
//
//  Based on kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//  
//  Depreciated and merged to fixture.cpp

#include "kinet.h"
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <sstream>
#include <iostream>
#include <math.h>
#include <fcntl.h>

#define ENABLE_LOGGING 0

const unsigned char oldHeaderBytes[] = {
    0x04, 0x01, 0xdc, 0x4a, 0x01, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00
};

const Old_KinetProtocol oldProtocol(21, 512, 1, oldHeaderBytes);

const unsigned char newHeaderBytes[] = {
    0x04, 0x01, 0xdc, 0x4a, 0x01, 0x00, 0x08, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x01, 0xFF, 0x00, 0xFF, 0x0F
};

const Old_KinetProtocol newProtocol(24, 512, 16, newHeaderBytes);


void Old_dump_buffer(unsigned n, const uint8_t* buf)
{
    unsigned c = 1;
    while (c < n)
    {
        fprintf(stderr, "%02X ", *buf++);
        if (c%16 == 0)
            fprintf(stderr, "\n");
        c++;
    }
    if (c%16 != 1)
        fprintf(stderr, "\n");
}

Old_PowerSupply::Old_PowerSupply()
{
    _connected = false;
    _frame = NULL;
    _proto = &oldProtocol;
    initializeBuffer();
}

Old_PowerSupply::Old_PowerSupply(const string strHost, const string strPort)
{    
    _connected = false;
    _frame = NULL;
    _proto = &oldProtocol;
    initializeBuffer();
    connect(strHost, strPort);
}

void Old_PowerSupply::switchToNewProtocol()
{
    if (_proto != &newProtocol)
    {
        _proto = &newProtocol;
        initializeBuffer();
    }
}

void Old_PowerSupply::initializeBuffer()
{
    if (_frame)
        free(_frame);
        
    _frame = (uint8_t*)malloc(_proto->getBufferSize() * sizeof(uint8_t));
    memset(_frame, _proto->getBufferSize(), 0);
    for (unsigned c = 0; c < _proto->getNumChannels(); c++)
    {
        memcpy(_frame + c * _proto->getPacketSize(), _proto->getHeaderBytes(), _proto->getHeaderSize());
        if (_proto->getNumChannels() > 1)
            _frame[c * _proto->getPacketSize() + 16] = c + 1;
    }
}

bool Old_PowerSupply::connect(const string strHost, const string strPort)
{
    struct addrinfo hints;
    struct addrinfo *pResult, *pr;
    int result;
    int sock;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    
    result = getaddrinfo(strHost.c_str(), strPort.c_str(), &hints, &pResult);
    if (result != 0) {
        fprintf(stderr, "getaddrinfo: %d\n", result);
        return false;
    }

    for (pr = pResult; pr != NULL; pr = pr->ai_next) {
        sock = socket(pr->ai_family, pr->ai_socktype, pr->ai_protocol);
        if (sock == -1)
            continue;
        
        if (::connect(sock, pr->ai_addr, pr->ai_addrlen) == 0)
            break;
#ifdef _WIN32
		closesocket(sock);
#else
        close(sock);
#endif
	}

    if (pr == NULL)
    {
        fprintf(stderr, "Could not connect to socket\n");
        return false;
    }
    
    _host = strHost;
    _port = ((sockaddr_in*)(pr->ai_addr))->sin_port;
    
    freeaddrinfo(pResult);
#ifdef _WIN32
	unsigned long on = 1;
	if (0 != ioctlsocket(sock, FIONBIO, &on)) {
		fprintf(stderr, "Could not enable non-blocking mode on socket\n");
	}
#else
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif    
    _socket = sock;
    _connected = true;
    
    return true;
}

Old_PowerSupply::~Old_PowerSupply()
{
    free(_frame);
}

string Old_PowerSupply::getHost()
{
    return _host;
}

int Old_PowerSupply::getPort()
{
    return _port;
}

void Old_PowerSupply::addOld_Fixture(Old_Fixture* pFix)
{
    _Old_Fixtures.push_back(pFix);
}

void Old_PowerSupply::clearOld_Fixtures()
{
    _Old_Fixtures.clear();
}

void Old_PowerSupply::go()
{
    if (!_connected)
    {
        fprintf(stderr, "Power supply is not connected.\n");
        return;
    }
    
    for (auto fix = _Old_Fixtures.begin(); fix != _Old_Fixtures.end(); fix++)
    {
        (*fix)->updateFrame(_frame);
    }
    
    for (unsigned channel = 0; channel < _proto->getNumChannels(); channel++)
    {
        send(_socket, (char*)_frame + _proto->getPacketSize() * channel, _proto->getPacketSize(), 0);
        
#if ENABLE_LOGGING
        std::cout << "Channel " << channel+1 << "\n";
        Old_dump_buffer(_proto->getHeaderSize() + 3, _frame + _proto->getPacketSize() * channel);
#endif
    }
}

Old_FixtureRGB::Old_FixtureRGB(int address, uint8_t r, uint8_t g, uint8_t b)
: Old_Fixture()
, _address(address)
{
    _values[0] = r;
    _values[1] = g;
    _values[2] = b;
	inter = &oldProtocol;
}

void Old_FixtureRGB::updateFrame(uint8_t* packets) const
{
    memcpy(packets + oldProtocol.getHeaderSize() + _address, _values, 3);
}


std::string Old_FixtureRGB::getName() const
{
    std::ostringstream out;
    out << getAddress();
    return out.str();
}

uint8_t Old_FixtureRGB::get_red() const
{
    return _values[0];
}

uint8_t Old_FixtureRGB::get_green() const
{
    return _values[1];
}

uint8_t Old_FixtureRGB::get_blue() const
{
    return _values[2];
}

void Old_FixtureRGB::set_red(uint8_t r)
{
    _values[0] = r;
}

void Old_FixtureRGB::set_green(uint8_t g)
{
    _values[1] = g;
}

void Old_FixtureRGB::set_blue(uint8_t b)
{
    _values[2] = b;
}

void Old_FixtureRGB::set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    set_red(r);
    set_green(g);
    set_blue(b);
}

//newly implemented
const Old_KinetProtocol * Old_FixtureRGB::get_protocol() const
{
	return inter;
}

Old_FixtureTile::Old_FixtureTile(int startChannel, int width, int height)
: _startChannel(startChannel)
, _Old_FixtureWidth(width)
, _Old_FixtureHeight(height)
, _videoX(0)
, _videoY(0)
, _videoW(width)
, _videoH(height)
, _sourceData(NULL)
, _rotation(0)
{
}

void Old_FixtureTile::setVideoRect(int x, int y, int w, int h)
{
    _videoX = x;
    _videoY = y;
    _videoW = w;
    _videoH = h;
}

void Old_FixtureTile::setSourceData(const uint8_t* sourceData, int sourceWidth, int sourceHeight, int sourceChannels)
{
    _sourceData = sourceData;
    _sourceWidth = sourceWidth;
    _sourceHeight = sourceHeight;
    _sourceChannels = sourceChannels;
}

void Old_FixtureTile::updateFrame(uint8_t* packets) const
{
    if (_sourceData == NULL)
        return;
    
    int tileX, tileY;
    uint8_t* pIndex;
    
    tileX = _Old_FixtureWidth-1;
    tileY = _Old_FixtureHeight/2-1;
    pIndex = packets + newProtocol.getPacketSize() * (_startChannel-1) + newProtocol.getHeaderSize();
    
    int xscale = (int)floor(_videoW/(_Old_FixtureWidth*1.0));
    int yscale = (int)floor(_videoH/(_Old_FixtureHeight*1.0));

    while (tileY < _Old_FixtureHeight)
    {
#if ENABLE_LOGGING
        std::cout << "Writing x: " << tileX << " y: " << tileY << " to address " << (void*)pIndex << "\n";
#endif
        int scale = 8;
        
        int xx = _videoX + tileX * xscale;
        int yy = _videoY + tileY * yscale;
        
        if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
            memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
        } else {
            memset(pIndex, 0, 3);
        }
        
        pIndex += 3;
        tileX--;

        if (tileX < 0)
        {
            tileX =  _Old_FixtureWidth-1;
            if (tileY == 0) {
                tileY = _Old_FixtureHeight/2;
                pIndex = packets + newProtocol.getPacketSize() * (_startChannel) + newProtocol.getHeaderSize();
            } else if (tileY < _Old_FixtureHeight/2) {
                tileY--;
            } else {
                tileY++;
            }
        }
    }
}

Old_FixtureTile6::Old_FixtureTile6(int startChannel, int width, int height)
: Old_FixtureTile(startChannel, width, height)
{
}

void Old_FixtureTile6::updateFrame(uint8_t* packets) const
{
    if (_sourceData == NULL)
        return;
    
    int tileX, tileY, startx, endx;
    uint8_t* pIndex;
    
    startx = 0;
    endx = _Old_FixtureWidth/2-1;
    tileX = startx;
    tileY = _Old_FixtureHeight-1;
    pIndex = packets + newProtocol.getPacketSize() * (_startChannel-1) + newProtocol.getHeaderSize();
    
    int xscale = (int)floor(_videoW/(_Old_FixtureWidth*1.0));
    int yscale = (int)floor(_videoH/(_Old_FixtureHeight*1.0));
    
    while(1)
    {
#if ENABLE_LOGGING
        std::cout << "Writing x: " << tileX << " y: " << tileY << " to address " << (void*)pIndex << "\n";
#endif

        int xx = _videoX + tileX * xscale;
        int yy = _videoY + tileY * yscale;
        
        if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
            memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
        } else {
            memset(pIndex, 0, 3);
        }
        
        pIndex += 3;
        tileX++;
        
        if (tileX > endx)
        {
            tileY--;
            if (tileY < 0) {
                if (endx == _Old_FixtureWidth - 1)
                    break;
                startx = endx + 1;
                endx = _Old_FixtureWidth - 1;
                tileY = _Old_FixtureHeight - 1;
                pIndex = packets + newProtocol.getPacketSize() * (_startChannel) + newProtocol.getHeaderSize();
            }
            tileX = startx;
        }
    }
    
}

Old_FixtureTileDC::Old_FixtureTileDC(int startChannel, int width, int height)
: Old_FixtureTile(startChannel, width, height)
{
}

void Old_FixtureTileDC::updateFrame(uint8_t* packets) const
{
    if (_sourceData == NULL)
        return;

    int tileX, tileY;
    uint8_t* pIndex;

    int quadrant = 0;

    tileX = _Old_FixtureWidth/2-1;
    tileY = 0;
    pIndex = packets + oldProtocol.getPacketSize() * (_startChannel-1) + oldProtocol.getHeaderSize();

    int xscale = (int)floor(_videoW/(_Old_FixtureWidth*1.0));
    int yscale = (int)floor(_videoH/(_Old_FixtureHeight*1.0));

    int tileYtop = _Old_FixtureHeight/2;
    int tileXright = -1;

    while (true)
    {
#if ENABLE_LOGGING
        std::cout << "Writing x: " << tileX << " y: " << tileY << " to address " << (void*)pIndex << "\n";
#endif

        int xx, yy;

        int rotation = getRotation() % 360;
        if (rotation % 90 != 0)
            rotation = 0;

        if (rotation == 0) {
            xx = _videoX + tileX * xscale;
            yy = _videoY + (_Old_FixtureHeight - tileY - 1) * yscale;
        } else if (rotation == 90) {
            xx = _videoX + (_Old_FixtureHeight - tileY - 1) * yscale;
            yy = _videoY + tileX * xscale;
        } else if (rotation == 180) {
            xx = _videoX + (_Old_FixtureWidth - tileX - 1) * xscale;
            yy = _videoY + tileY * yscale;
        } else if (rotation == 270) {
            xx = _videoX + tileY * yscale;
            yy = _videoY + (_Old_FixtureWidth - tileX - 1) * xscale;
        }

        if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
            memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
        } else {
            memset(pIndex, 0, 3);
        }

        pIndex += 3;
        tileY++;

        if (tileY >= tileYtop)
        {
            tileX--;
            tileY = tileYtop - _Old_FixtureHeight/2;
            if (tileX <= tileXright) {
                if (quadrant == 0) {
                    quadrant = 1;
                    tileX = _Old_FixtureWidth/2 - 1;
                    tileY = _Old_FixtureHeight/2;
                    tileYtop = _Old_FixtureHeight;
                } else if (quadrant == 1) {
                    quadrant = 2;
                    tileX = _Old_FixtureWidth - 1;
                    tileY = _Old_FixtureHeight/2;
                    tileXright = _Old_FixtureWidth/2 - 1;
                } else if (quadrant == 2) {
                    quadrant = 3;
                    tileX = _Old_FixtureWidth - 1;
                    tileY = 0;
                    tileYtop = _Old_FixtureHeight/2;
                } else if (quadrant == 3) {
                    break;
                }
            }
        }
    }
}



std::string Old_FixtureTile::getName() const
{
    std::ostringstream out;
    out << "C" << _startChannel << "/" << (_startChannel + 1);
    return out.str();
}

