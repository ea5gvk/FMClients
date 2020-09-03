/*
 *   Copyright (C) 2009-2014,2016,2018 by Jonathan Naylor G4KLX
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

#include "FMNetwork.h"

#include <cstdio>
#include <cassert>
#include <cstring>

CFMNetwork::CFMNetwork(unsigned int port) :
m_socket(port),
m_addr(),
m_addrLen(0U)
{
}

CFMNetwork::~CFMNetwork()
{
}

bool CFMNetwork::open()
{
	::fprintf(stdout, "Opening FM network connection\n");

	return m_socket.open();
}

bool CFMNetwork::write(const unsigned char* data, unsigned int length)
{
	if (m_addrLen == 0U)
		return true;

	assert(data != NULL);

	return m_socket.write(data, length, m_addr, m_addrLen);
}

unsigned int CFMNetwork::read(unsigned char* data, unsigned int len)
{
	sockaddr_storage addr;
	unsigned int addrlen;
	int length = m_socket.read(data, len, addr, addrlen);
	if (length <= 0)
		return 0U;

	m_addr    = addr;
	m_addrLen = addrlen;

	if (::memcmp(data, "FMP", 3U) == 0) {			// A poll
		write(data, length);
		return 0U;
	} else if (len >=  3 && (::memcmp(data, "FMD", 3U) || ::memcmp(data, "FME", 3U))) {
		return length;
	} else {
		return 0U;
	}
}

void CFMNetwork::end()
{
	m_addrLen = 0U;
}

void CFMNetwork::close()
{
	m_socket.close();

	::fprintf(stdout, "Closing FM network connection\n");
}
