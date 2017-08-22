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
//  Depreciated and merged to fixture.cpp

#include "kinet16.h"
#include <iostream>


Old_FixtureRGB16::Old_FixtureRGB16(int address, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
	:Old_FixtureRGB(address, r, g, b)
{
	_address = getAddress();
	set_red(r);
	set_green(g);
	set_blue(b);
	_values[1] = r2;
	_values[3] = g2;
	_values[5] = b2;
	inter = get_protocol();
}

uint8_t Old_FixtureRGB16::get_red() const
{
	return _values[0];
}

uint8_t Old_FixtureRGB16::get_green() const
{
	return _values[2];
}

uint8_t Old_FixtureRGB16::get_blue() const
{
	return _values[4];
}

uint8_t Old_FixtureRGB16::get_red2() const
{
	return _values[1];
}

uint8_t Old_FixtureRGB16::get_green2() const
{
	return _values[3];
}

uint8_t Old_FixtureRGB16::get_blue2() const
{
	return _values[5];
}


uint8_t * Old_FixtureRGB16::get_config()
{
	return _values;
}

void Old_FixtureRGB16::get_config(uint8_t (&table)[6])
{
	for (auto i = 0; i < 6; i++) {
		table[i] = _values[i];
	}
}

void Old_FixtureRGB16::set_config(uint8_t * config, uint8_t *matrix[336])
{
	std::cout << unsigned(*config) << std::endl;
	Old_FixtureRGB16::set_red(*config);
	std::cout << unsigned(*config) << std::endl;
	Old_FixtureRGB16::set_green(*(config + 1));
	Old_FixtureRGB16::set_blue(*(config + 2));
	_values[1] = *(config + 3);
	_values[3] = *(config + 4);
	_values[5] = *(config + 5);
}

void Old_FixtureRGB16::set_config(uint8_t config[6])
{
	uint8_t temp[6];
	for (int i = 0; i < 6; i++) {
		temp[i] = config[i];
	}
	Old_FixtureRGB16::set_red(temp[0]);
	Old_FixtureRGB16::set_green(temp[1]);
	Old_FixtureRGB16::set_blue(temp[2]);
	_values[1] = temp[3];
	_values[3] = temp[4];
	_values[5] = temp[5];
}

void Old_FixtureRGB16::set_red(uint8_t r) {
	Old_FixtureRGB::set_red(r);
	_values[0] = Old_FixtureRGB::_values[0];
}

void Old_FixtureRGB16::set_green(uint8_t g) {
	Old_FixtureRGB::set_green(g);
	_values[2] = Old_FixtureRGB::_values[1];
}

void Old_FixtureRGB16::set_blue(uint8_t b) {
	Old_FixtureRGB::set_blue(b);
	_values[4] = Old_FixtureRGB::_values[2];
}

void Old_FixtureRGB16::set_red2(uint8_t r2)
{
	_values[1] = r2;
}

void Old_FixtureRGB16::set_green2(uint8_t g2)
{
	_values[3] = g2;
}

void Old_FixtureRGB16::set_blue2(uint8_t b2)
{
	_values[5] = b2;
}

void Old_FixtureRGB16::set_rgb2(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2)
{
	Old_FixtureRGB::set_rgb(r, g, b);
	_values[0] = Old_FixtureRGB::get_red();
	_values[2] = Old_FixtureRGB::get_green();
	_values[4] = Old_FixtureRGB::get_blue();
	set_red2(r2);
	set_green2(g2);
	set_blue2(b2);
}

void Old_FixtureRGB16::updateFrame(uint8_t* packets) const
{
	memcpy(packets + (*inter).getHeaderSize() + _address, _values, 6);
}

string Old_FixtureRGB16::get_str() const
{
	std::stringstream ss;
	ss << int(_values[0]) << "-" << int(_values[2]) << "-" << int(_values[4]) << "-" << int(_values[1]) << "-" << int(_values[3]) << "-" << int(_values[5]);
	return ss.str();
}
