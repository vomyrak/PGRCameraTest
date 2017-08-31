//
//  Fixture.h
//
//	Created by Husheng Deng on 15 August 17 for
//  Imperial College London lightstage project.
//
//  Created based on kinet.h by Michael Dewberry on 12/19/12.
//  With modifications that specifically suit the need of the project.
//
//  Based on kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//

#include "Fixture.h"
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

const KinetProtocol oldProtocol(21, 512, 1, oldHeaderBytes);

const unsigned char newHeaderBytes[] = {
	0x04, 0x01, 0xdc, 0x4a, 0x01, 0x00, 0x08, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x01, 0xFF, 0x00, 0xFF, 0x0F
};

const KinetProtocol newProtocol(24, 512, 16, newHeaderBytes);


void dump_buffer(unsigned n, const uint8_t* buf)
{
	unsigned c = 1;
	while (c < n)
	{
		fprintf(stderr, "%02X ", *buf++);
		if (c % 16 == 0)
			fprintf(stderr, "\n");
		c++;
	}
	if (c % 16 != 1)
		fprintf(stderr, "\n");
}

PowerSupply::PowerSupply()
{
	_connected = false;
	_frame = NULL;
	_proto = &oldProtocol;
	initializeBuffer();
}

PowerSupply::PowerSupply(const string strHost, const string strPort)
{
	_connected = false;
	_frame = NULL;
	_proto = &oldProtocol;
	initializeBuffer();
	connect(strHost, strPort);
}

void PowerSupply::switchToNewProtocol()
{
	if (_proto != &newProtocol)
	{
		_proto = &newProtocol;
		initializeBuffer();
	}
}

void PowerSupply::initializeBuffer()
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

bool PowerSupply::connect(const string strHost, const string strPort)
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

PowerSupply::~PowerSupply()
{
	free(_frame);
}

string PowerSupply::getHost()
{
	return _host;
}

int PowerSupply::getPort()
{
	return _port;
}

void PowerSupply::addFixture(Fixture* pFix)
{
	_fixtures.push_back(pFix);
}

void PowerSupply::clearFixtures()
{
	_fixtures.clear();
}

void PowerSupply::go()
{
	if (!_connected)
	{
		fprintf(stderr, "Power supply is not connected.\n");
		return;
	}

	for (auto fix = _fixtures.begin(); fix != _fixtures.end(); fix++)
	{
		(*fix)->updateFrame(_frame);
	}

	for (unsigned channel = 0; channel < _proto->getNumChannels(); channel++)
	{
		send(_socket, (char*)_frame + _proto->getPacketSize() * channel, _proto->getPacketSize(), 0);

#if ENABLE_LOGGING
		std::cout << "Channel " << channel + 1 << "\n";
		dump_buffer(_proto->getHeaderSize() + 3, _frame + _proto->getPacketSize() * channel);
#endif
	}
}

FixtureLED::FixtureLED(int address, uint8_t r, uint8_t g, uint8_t b)
	: Fixture()
	, _address(address)
{
	_values[0] = r;
	_values[1] = g;
	_values[2] = b;
	_values[3] = 0;
	_values[4] = 0;
	_values[5] = 0;
	channelNumber = 3;
}

FixtureLED::FixtureLED(int address, uint8_t * ptr, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
	: Fixture()
	, _address(address)
{
	if (ptr != nullptr) {
		_values = ptr;
	}
	_values[0] = r;
	_values[1] = r2;
	_values[2] = g;
	_values[3] = g2;
	_values[4] = b;
	_values[5] = b2;
	channelNumber = 6;
}

void FixtureLED::updateFrame(uint8_t* packets) const
{
	memcpy(packets + oldProtocol.getHeaderSize() + _address, _values, channelNumber);
}


std::string FixtureLED::getName() const
{
	std::ostringstream out;
	out << getAddress();
	return out.str();
}

uint8_t FixtureLED::getRed() const
{
	return _values[0];
}

uint8_t FixtureLED::getGreen() const
{
	return _values[2];
}

uint8_t FixtureLED::getBlue() const
{
	return _values[4];
}

uint8_t FixtureLED::getRed2() const
{
	return _values[1];
}

uint8_t FixtureLED::getGreen2() const
{
	return _values[3];
}

uint8_t FixtureLED::getBlue2() const
{
	return _values[5];
}

void FixtureLED::setRed(uint8_t r)
{
	_values[0] = r;
}

void FixtureLED::setGreen(uint8_t g)
{
	_values[2] = g;
}

void FixtureLED::setBlue(uint8_t b)
{
	_values[4] = b;
}

void FixtureLED::setRed2(uint8_t r2)
{
	_values[1] = r2;
}

void FixtureLED::setGreen2(uint8_t g2)
{
	_values[3] = g2;
}

void FixtureLED::setBlue2(uint8_t b2)
{
	_values[5] = b2;
}

void FixtureLED::setRGB(uint8_t r, uint8_t g, uint8_t b)
{
	if (channelNumber = 3) {
		setRed(r);
		setGreen(g);
		setBlue(b);
	}
	else {
		std::cout << "Setting failed. Use setValue for 6-channel lights" << std::endl;
	}
}

void FixtureLED::setValue(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	if (channelNumber = 6) {
		setRed(r);
		setGreen(g);
		setBlue(b);
		setRed2(r2);
		setGreen2(g2);
		setBlue2(b2);
	}
	else {
		std::cout << "Setting failed. Use setRGB for 3-channel lights" << std::endl;
	}
}

uint8_t * FixtureLED::getConfig()
{
	return _values;
}

void FixtureLED::getConfig(uint8_t(&table)[6])
{
	if (channelNumber == 6) {
		for (auto i = 0; i < 6; i++) {
			table[i] = _values[i];
		}
	}
	else {
		std::cout << "Copying failed. Use uint8_t[3] for 3-channel lights" << std::endl;
	}
}

void FixtureLED::getConfig(uint8_t(&table)[3])
{
	if (channelNumber == 3) {
		table[0] = _values[0];
		table[1] = _values[2];
		table[2] = _values[4];
	}
	else {
		std::cout << "Copying failed. Use uint8_t[6] for 6-channel lights" << std::endl;
	}
}

void FixtureLED::setConfig(uint8_t config[6])
{
	if (channelNumber == 6) {
		uint8_t temp[6];
		for (int i = 0; i < 6; i++) {
			temp[i] = config[i];
		}
		_values[0] = temp[0];
		_values[2] = temp[1];
		_values[4] = temp[2];
		_values[1] = temp[3];
		_values[3] = temp[4];
		_values[5] = temp[5];
	}
	else {
		uint8_t temp[3];
		for (int i = 0; i < 3; i++) {
			temp[i] = config[i];
		}
		_values[0] = temp[0];
		_values[2] = temp[1];
		_values[4] = temp[2];
	}
}

void FixtureLED::setConfig(const uint8_t config[6])
{
	if (channelNumber == 6) {
		uint8_t temp[6];
		for (int i = 0; i < 6; i++) {
			temp[i] = config[i];
		}
		_values[0] = temp[0];
		_values[2] = temp[1];
		_values[4] = temp[2];
		_values[1] = temp[3];
		_values[3] = temp[4];
		_values[5] = temp[5];
	}
	else {
		uint8_t temp[3];
		for (int i = 0; i < 3; i++) {
			temp[i] = config[i];
		}
		_values[0] = temp[0];
		_values[2] = temp[1];
		_values[4] = temp[2];
	}
}

/*
void FixtureLED::setConfig(uint8_t config[3])
{
	if (channelNumber == 3) {
		uint8_t temp[3];
		for (int i = 0; i < 3; i++) {
			temp[i] = config[i];
			_values[i] = temp[i];
		}
	}
	else {
		std::cout << "Copying failed. Use uint8_t[3] for 3-channel lights." << std::endl;
	}
}
*/
string FixtureLED::getStr() const
{
	std::stringstream ss;
	if (channelNumber == 6) {
		ss << unsigned(_values[0]) << "-" << unsigned(_values[2]) << "-" << unsigned(_values[4]) << "-" << unsigned(_values[1]) << "-" << unsigned(_values[3]) << "-" << unsigned(_values[5]);
	}
	else {
		ss << unsigned(_values[0]) << "-" << unsigned(_values[2]) << "-" << unsigned(_values[4]);
	}
	return ss.str();
}
uint8_t * FixtureLED::getValuePtr()
{
	return _values;
}

void FixtureLED::setValuePtr(uint8_t * ptr)
{
	_values = ptr;
}

//Original codes that are not used in the project
/*
FixtureTile::FixtureTile(int startChannel, int width, int height)
	: _startChannel(startChannel)
	, _fixtureWidth(width)
	, _fixtureHeight(height)
	, _videoX(0)
	, _videoY(0)
	, _videoW(width)
	, _videoH(height)
	, _sourceData(NULL)
	, _rotation(0)
{
}

void FixtureTile::setVideoRect(int x, int y, int w, int h)
{
	_videoX = x;
	_videoY = y;
	_videoW = w;
	_videoH = h;
}

void FixtureTile::setSourceData(const uint8_t* sourceData, int sourceWidth, int sourceHeight, int sourceChannels)
{
	_sourceData = sourceData;
	_sourceWidth = sourceWidth;
	_sourceHeight = sourceHeight;
	_sourceChannels = sourceChannels;
}

void FixtureTile::updateFrame(uint8_t* packets) const
{
	if (_sourceData == NULL)
		return;

	int tileX, tileY;
	uint8_t* pIndex;

	tileX = _fixtureWidth - 1;
	tileY = _fixtureHeight / 2 - 1;
	pIndex = packets + newProtocol.getPacketSize() * (_startChannel - 1) + newProtocol.getHeaderSize();

	int xscale = (int)floor(_videoW / (_fixtureWidth*1.0));
	int yscale = (int)floor(_videoH / (_fixtureHeight*1.0));

	while (tileY < _fixtureHeight)
	{
#if ENABLE_LOGGING
		std::cout << "Writing x: " << tileX << " y: " << tileY << " to address " << (void*)pIndex << "\n";
#endif
		int scale = 8;

		int xx = _videoX + tileX * xscale;
		int yy = _videoY + tileY * yscale;

		if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
			memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
		}
		else {
			memset(pIndex, 0, 3);
		}

		pIndex += 3;
		tileX--;

		if (tileX < 0)
		{
			tileX = _fixtureWidth - 1;
			if (tileY == 0) {
				tileY = _fixtureHeight / 2;
				pIndex = packets + newProtocol.getPacketSize() * (_startChannel)+newProtocol.getHeaderSize();
			}
			else if (tileY < _fixtureHeight / 2) {
				tileY--;
			}
			else {
				tileY++;
			}
		}
	}
}

FixtureTile6::FixtureTile6(int startChannel, int width, int height)
	: FixtureTile(startChannel, width, height)
{
}

void FixtureTile6::updateFrame(uint8_t* packets) const
{
	if (_sourceData == NULL)
		return;

	int tileX, tileY, startx, endx;
	uint8_t* pIndex;

	startx = 0;
	endx = _fixtureWidth / 2 - 1;
	tileX = startx;
	tileY = _fixtureHeight - 1;
	pIndex = packets + newProtocol.getPacketSize() * (_startChannel - 1) + newProtocol.getHeaderSize();

	int xscale = (int)floor(_videoW / (_fixtureWidth*1.0));
	int yscale = (int)floor(_videoH / (_fixtureHeight*1.0));

	while (1)
	{
#if ENABLE_LOGGING
		std::cout << "Writing x: " << tileX << " y: " << tileY << " to address " << (void*)pIndex << "\n";
#endif

		int xx = _videoX + tileX * xscale;
		int yy = _videoY + tileY * yscale;

		if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
			memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
		}
		else {
			memset(pIndex, 0, 3);
		}

		pIndex += 3;
		tileX++;

		if (tileX > endx)
		{
			tileY--;
			if (tileY < 0) {
				if (endx == _fixtureWidth - 1)
					break;
				startx = endx + 1;
				endx = _fixtureWidth - 1;
				tileY = _fixtureHeight - 1;
				pIndex = packets + newProtocol.getPacketSize() * (_startChannel)+newProtocol.getHeaderSize();
			}
			tileX = startx;
		}
	}

}

FixtureTileDC::FixtureTileDC(int startChannel, int width, int height)
	: FixtureTile(startChannel, width, height)
{
}

void FixtureTileDC::updateFrame(uint8_t* packets) const
{
	if (_sourceData == NULL)
		return;

	int tileX, tileY;
	uint8_t* pIndex;

	int quadrant = 0;

	tileX = _fixtureWidth / 2 - 1;
	tileY = 0;
	pIndex = packets + oldProtocol.getPacketSize() * (_startChannel - 1) + oldProtocol.getHeaderSize();

	int xscale = (int)floor(_videoW / (_fixtureWidth*1.0));
	int yscale = (int)floor(_videoH / (_fixtureHeight*1.0));

	int tileYtop = _fixtureHeight / 2;
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
			yy = _videoY + (_fixtureHeight - tileY - 1) * yscale;
		}
		else if (rotation == 90) {
			xx = _videoX + (_fixtureHeight - tileY - 1) * yscale;
			yy = _videoY + tileX * xscale;
		}
		else if (rotation == 180) {
			xx = _videoX + (_fixtureWidth - tileX - 1) * xscale;
			yy = _videoY + tileY * yscale;
		}
		else if (rotation == 270) {
			xx = _videoX + tileY * yscale;
			yy = _videoY + (_fixtureWidth - tileX - 1) * xscale;
		}

		if (xx >= 0 && xx < _sourceWidth && yy >= 0 && yy < _sourceHeight) {
			memcpy(pIndex, _sourceData + (xx + yy * _sourceWidth) * _sourceChannels, 3);
		}
		else {
			memset(pIndex, 0, 3);
		}

		pIndex += 3;
		tileY++;

		if (tileY >= tileYtop)
		{
			tileX--;
			tileY = tileYtop - _fixtureHeight / 2;
			if (tileX <= tileXright) {
				if (quadrant == 0) {
					quadrant = 1;
					tileX = _fixtureWidth / 2 - 1;
					tileY = _fixtureHeight / 2;
					tileYtop = _fixtureHeight;
				}
				else if (quadrant == 1) {
					quadrant = 2;
					tileX = _fixtureWidth - 1;
					tileY = _fixtureHeight / 2;
					tileXright = _fixtureWidth / 2 - 1;
				}
				else if (quadrant == 2) {
					quadrant = 3;
					tileX = _fixtureWidth - 1;
					tileY = 0;
					tileYtop = _fixtureHeight / 2;
				}
				else if (quadrant == 3) {
					break;
				}
			}
		}
	}
}



std::string FixtureTile::getName() const
{
	std::ostringstream out;
	out << "C" << _startChannel << "/" << (_startChannel + 1);
	return out.str();
}

*/