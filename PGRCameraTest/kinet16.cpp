//
//  kinet16.cpp
//
//  created by Husheng Deng on 30 Jun 17
//
//  Based on kinet.cpp created by Michael Dewberry on 12/19/12.
//
//  Based on kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//

#include "kinet16.h"
#include <iostream>


FixtureRGB16::FixtureRGB16(int address, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
	:FixtureRGB(address, r, g, b)
{
	_address = getAddress();
	_values[0] = get_red();
	_values[2] = get_green();
	_values[4] = get_blue();
	_values[1] = r2;
	_values[3] = g2;
	_values[5] = b2;
	inter = get_protocol();
}


uint8_t FixtureRGB16::get_red2() const
{
	return _values[1];
}

uint8_t FixtureRGB16::get_green2() const
{
	return _values[3];
}

uint8_t FixtureRGB16::get_blue2() const
{
	return _values[5];
}

void FixtureRGB16::set_red(uint8_t r) {
	FixtureRGB::set_red(r);
	_values[0] = get_red();
}

void FixtureRGB16::set_green(uint8_t g) {
	FixtureRGB::set_green(g);
	_values[2] = get_green();
}

void FixtureRGB16::set_blue(uint8_t b) {
	FixtureRGB::set_blue(b);
	_values[4] = get_blue();
}

void FixtureRGB16::set_red2(uint8_t r2)
{
	_values[1] = r2;
}

void FixtureRGB16::set_green2(uint8_t g2)
{
	_values[3] = g2;
}

void FixtureRGB16::set_blue2(uint8_t b2)
{
	_values[5] = b2;
}

void FixtureRGB16::set_rgb2(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	FixtureRGB::set_rgb(r, g, b);
	_values[4] = get_blue();
	_values[2] = get_green();
	_values[0] = get_red();
	set_red2(r2);
	set_green2(g2);
	set_blue2(b2);
}

void FixtureRGB16::updateFrame(uint8_t* packets) const
{
	
	memcpy(packets + (*inter).getHeaderSize() + _address, _values, 6);
}

string FixtureRGB16::get_str() const
{
	std::stringstream ss;
	ss << int(_values[0]) << "-" << int(_values[2]) << "-" << int(_values[4]) << "-" << int(_values[1]) << "-" << int(_values[3]) << "-" << int(_values[5]);
	return ss.str();
}
