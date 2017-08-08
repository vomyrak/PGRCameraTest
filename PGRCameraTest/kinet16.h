//
//  kinet16.h
//
//	enables 16-bit control
//
//  created by Husheng Deng on 30 Jun 17
//
//  Based on kinet.h created by Michael Dewberry on 12/19/12.
//
//  Port of kinet.py by Giles Hall
//  http://github.com/vishnubob/kinet
//

#include "kinet.h"

class FixtureRGB16: public FixtureRGB
{
public:
	FixtureRGB16(int address, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t r2 = 0, uint8_t g2 = 0, uint8_t b2 = 0);
	uint8_t get_red2() const;
	uint8_t get_green2() const;
	uint8_t get_blue2() const;

	void set_red(uint8_t r);
	void set_green(uint8_t g);
	void set_blue(uint8_t b);
	void set_red2(uint8_t r2);
	void set_green2(uint8_t g2);
	void set_blue2(uint8_t b2);

	void set_rgb2(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);
	void FixtureRGB16::updateFrame(uint8_t* packets) const;
	string FixtureRGB16::get_str() const;

protected:
	int _address;
	unsigned char _values[6];
	const KinetProtocol *inter;
};