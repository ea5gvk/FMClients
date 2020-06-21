/*
 *   Copyright (C) 2020 by Geoffrey Merck F4FXL - KC3FRA
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef Utils_H
#define Utils_H

#include <string>

// Change short endianness
#define SWAP_BYTES_16(a) ((((a) >> 8) & 0x00FFU) | (((a) << 8) & 0xFF00U))

//Pad string right (https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s02.html)
template<typename T> void pad(std::basic_string<T>& s,  T c, typename std::basic_string<T>::size_type n)
{
   if (n > s.length())
      s.append(n - s.length(), c);
};


enum TRISTATE {
	STATE_FALSE,
	STATE_TRUE,
	STATE_UNKNOWN
};

class CUtils {
public:
	static void dump(const std::string& title, const unsigned char* data, unsigned int length);
	static void dump(int level, const std::string& title, const unsigned char* data, unsigned int length);

	static void dump(const std::string& title, const bool* bits, unsigned int length);
	static void dump(int level, const std::string& title, const bool* bits, unsigned int length);

	static void byteToBitsBE(unsigned char byte, bool* bits);
	static void byteToBitsLE(unsigned char byte, bool* bits);

	static void bitsToByteBE(const bool* bits, unsigned char& byte);
	static void bitsToByteLE(const bool* bits, unsigned char& byte);

	static unsigned int compare(const unsigned char* bytes1, const unsigned char* bytes2, unsigned int length);

private:
};

#endif