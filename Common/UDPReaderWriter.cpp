/*
 *   Copyright (C) 2006-2012 by Jonathan Naylor G4KLX
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

#include "UDPReaderWriter.h"
#include "Utils.h"
#include "Log.h"

#include <cassert>

#if !defined(__WINDOWS__)
#include <cerrno>
#endif


CUDPReaderWriter::CUDPReaderWriter(const std::string& remoteAddress, unsigned int remotePort, const std::string& localAddress, unsigned int localPort) :
m_remoteAddress(remoteAddress),
m_remotePort(remotePort),
m_localAddress(localAddress),
m_localPort(localPort),
m_remAddr(),
m_remAddrLen(),
m_socket(localPort)
{
	assert(!remoteAddress.empty());
	assert(remotePort > 0U);

	CUDPSocket::lookup(m_remoteAddress, m_remotePort, m_remAddr, m_remAddrLen);
}

CUDPReaderWriter::~CUDPReaderWriter()
{
}

bool CUDPReaderWriter::open()
{
	return m_socket.open();
}

int CUDPReaderWriter::read(unsigned char* buffer, unsigned int length)
{
	sockaddr_storage addr;
	unsigned int addrlen;
	int result = m_socket.read(buffer, length, addr, addrlen);

	if (!CUDPSocket::match(m_remAddr, addr)) {
		result = 0;
		LogMessage("Packet received from an invalid source");
	}

	return result;
}

bool CUDPReaderWriter::write(const unsigned char* buffer, unsigned int length)
{
	return m_socket.write(buffer, length, m_remAddr, m_remAddrLen);
}

void CUDPReaderWriter::close()
{
	m_socket.close();
}

