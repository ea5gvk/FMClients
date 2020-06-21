/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "RepeaterProtocolHandler.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <cassert>
#include <cstring>

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CRepeaterProtocolHandler::CRepeaterProtocolHandler(const std::string& gatewayAddress, unsigned int gatewayPort, const std::string& localAddress, unsigned int localPort) :
m_socket(gatewayAddress, gatewayPort, localAddress, localPort),
m_outId(0U),
m_outSeq(0U),
m_type(NETWORK_NONE),
m_inId(0U),
m_buffer(NULL),
m_length(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];

	// wxDateTime now = wxDateTime::UNow();
	// ::srand(now.GetMillisecond());
}

CRepeaterProtocolHandler::~CRepeaterProtocolHandler()
{
	delete[] m_buffer;
}

bool CRepeaterProtocolHandler::open()
{
	return m_socket.open();
}

bool CRepeaterProtocolHandler::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x20U;

	// Create a random id for this transmission
	m_outId = (::rand() % 65535U) + 1U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	buffer[7] = 0U;

	buffer[8]  = header.getFlag1();
	buffer[9]  = header.getFlag2();
	buffer[10] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[11 + i] = header.getRptCall1()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[19 + i] = header.getRptCall2()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[27 + i] = header.getYourCall()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[35 + i] = header.getMyCall1()[i];

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[43 + i] = header.getMyCall2()[i];

	// Get the checksum for the header
	CCCITTChecksumReverse csum;
	csum.update(buffer + 8U, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
	csum.result(buffer + 47U);

	m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump("Sending Header", buffer, 49U);
	return true;
#else
	for (unsigned int i = 0U; i < 2U; i++) {
		bool ret = m_socket.write(buffer, 49U);
		if (!ret)
			return false;
	}

	return true;
#endif
}

bool CRepeaterProtocolHandler::writeData(const unsigned char* data, unsigned int length, unsigned int errors, bool end)
{
	assert(data != NULL);
	assert(length == DV_FRAME_LENGTH_BYTES || length == DV_FRAME_MAX_LENGTH_BYTES);

	unsigned char buffer[30U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x21U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	buffer[7] = m_outSeq;
	if (end)
		buffer[7] |= 0x40U;			// End of data marker

	buffer[8] = errors;

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

	::memcpy(buffer + 9U, data, length);

#if defined(DUMP_TX)
	CUtils::dump("Sending Data", buffer, length + 9U);
	return true;
#else
	return m_socket.write(buffer, length + 9U);
#endif
}

bool CRepeaterProtocolHandler::writeBusyHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x22U;

	// Create a random id for this header
	m_outId = (::rand() % 65535U) + 1U;

	buffer[5] = m_outId / 256U;		// Unique session id
	buffer[6] = m_outId % 256U;

	buffer[7] = 0U;

	buffer[8]  = header.getFlag1();
	buffer[9]  = header.getFlag2();
	buffer[10] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[11 + i] = header.getRptCall1()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[19 + i] = header.getRptCall2()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[27 + i] = header.getYourCall()[i];

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[35 + i] = header.getMyCall1()[i];

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[43 + i] = header.getMyCall2()[i];

	// Get the checksum for the header
	CCCITTChecksumReverse csum;
	csum.update(buffer + 8U, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
	csum.result(buffer + 47U);

	m_outSeq = 0U;

#if defined(DUMP_TX)
	CUtils::dump("Sending Busy Header", buffer, 49U);
	return true;
#else
	return m_socket.write(buffer, 49U);
#endif
}

bool CRepeaterProtocolHandler::writeBusyData(const unsigned char* data, unsigned int length, unsigned int errors, bool end)
{
	assert(data != NULL);
	assert(length == DV_FRAME_LENGTH_BYTES || length == DV_FRAME_MAX_LENGTH_BYTES);

	unsigned char buffer[30U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x23U;

	buffer[5] = m_outId / 256U;	// Unique session id
	buffer[6] = m_outId % 256U;

	// If this is a data sync, reset the sequence to zero
	if (data[9] == 0x55 && data[10] == 0x2D && data[11] == 0x16)
		m_outSeq = 0U;

	buffer[7] = m_outSeq;
	if (end)
		buffer[7] |= 0x40U;			// End of data marker

	buffer[8] = errors;

	m_outSeq++;
	if (m_outSeq > 0x14U)
		m_outSeq = 0U;

	::memcpy(buffer + 9U, data, length);

#if defined(DUMP_TX)
	CUtils::dump("Sending Busy Data", buffer, length + 9U);
	return true;
#else
	return m_socket.write(buffer, length + 9U);
#endif
}

bool CRepeaterProtocolHandler::writePoll(const std::string& text)
{
	unsigned char buffer[40U];

	buffer[0] = 'D';
	buffer[1] = 'S';
	buffer[2] = 'R';
	buffer[3] = 'P';

	buffer[4] = 0x0A;				// Poll with text

	unsigned int length = text.length();

	for (unsigned int i = 0U; i < length; i++)
		buffer[5U + i] = text[i];

	buffer[5U + length] = 0x00;

#if defined(DUMP_TX)
	CUtils::dump("Sending Poll", buffer, 6U + length);
	return true;
#else
	return m_socket.write(buffer, 6U + length);
#endif
}

NETWORK_TYPE CRepeaterProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CRepeaterProtocolHandler::readPackets()
{
	m_type = NETWORK_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' && m_buffer[1] == 'S' && m_buffer[2] == 'R' && m_buffer[3] == 'P') {
		if (m_buffer[4] == 0x00U) {
			m_type = NETWORK_TEXT;
			return false;
		}

		if (m_buffer[4] == 0x01U) {
			m_type = NETWORK_TEMPTEXT;
			return false;
		}

		// Status data 1
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x00U) {
			m_type = NETWORK_STATUS1;
			return false;
		}

		// Status data 2
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x01U) {
			m_type = NETWORK_STATUS2;
			return false;
		}

		// Status data 3
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x02U) {
			m_type = NETWORK_STATUS3;
			return false;
		}

		// Status data 4
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x03U) {
			m_type = NETWORK_STATUS4;
			return false;
		}

		// Status data 5
		else if (m_buffer[4] == 0x04U && m_buffer[5] == 0x04U) {
			m_type = NETWORK_STATUS5;
			return false;
		}

		// Header data
		else if (m_buffer[4] == 0x20U) {
			unsigned short id = m_buffer[5] * 256U + m_buffer[6];

			// Are we listening for headers?
			if (m_inId != 0U)
				return true;

			m_inId = id;					// Take the stream id
			m_type = NETWORK_HEADER;
			return false;
		}

		// User data
		else if (m_buffer[4] == 0x21U) {
			unsigned short id = m_buffer[5] * 256U + m_buffer[6];

			// Check that the stream id matches the valid header, reject otherwise
			if (id != m_inId)
				return true;

			// Is this the last packet in the stream?
			if ((m_buffer[7] & 0x40) == 0x40)
				m_inId = 0U;

			m_type = NETWORK_DATA;
			return false;
		}

		else if (m_buffer[4] == 0x24U) {
			// Silently ignore DD data
		}
	}

	CUtils::dump("Unknown packet from the Gateway", m_buffer, m_length);

	return true;
}

CHeaderData* CRepeaterProtocolHandler::readHeader()
{
	if (m_type != NETWORK_HEADER)
		return NULL;

	// If the checksum is 0xFFFF then we accept the header without testing the checksum
	if (m_buffer[47U] == 0xFFU && m_buffer[48U] == 0xFFU)
		return new CHeaderData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES, false);

	// Header checksum testing is enabled
	CHeaderData* header = new CHeaderData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES, true);

	if (!header->isValid()) {
		CUtils::dump("Header checksum failure from the Gateway", m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);
		delete header;
		return NULL;
	}

	return header;
}

unsigned int CRepeaterProtocolHandler::readData(unsigned char* buffer, unsigned int length, unsigned char& seqNo)
{
	if (m_type != NETWORK_DATA)
		return 0U;

	unsigned int dataLen = m_length - 9U;

	// Is our buffer too small?
	if (dataLen > length)
		dataLen = length;

	seqNo = m_buffer[7U];

	::memcpy(buffer, m_buffer + 9U, dataLen);

	// Simple sanity checks of the incoming sync bits
	if (seqNo == 0U) {
		// Regenerate sync bytes
		buffer[9U]  = DATA_SYNC_BYTES[0U];
		buffer[10U] = DATA_SYNC_BYTES[1U];
		buffer[11U] = DATA_SYNC_BYTES[2U];
	} else if (::memcmp(buffer + 9U, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0) {
		// Sync bytes appearing where they shouldn't!
		buffer[9U]  = 0x70U;
		buffer[10U] = 0x4FU;
		buffer[11U] = 0x93U;
	}

	return dataLen;
}

void CRepeaterProtocolHandler::readText(std::string& text, LINK_STATUS& status, std::string& reflector)
{
	if (m_type != NETWORK_TEXT) {
		text = "                    ";
		reflector = "        ";
		status = LS_NONE;
		return;
	}

	text = std::string((char*)(m_buffer + 5U), 0U, 20U);

	status = LINK_STATUS(m_buffer[25U]);

	reflector = std::string((char*)(m_buffer + 26U), 0U, 8U);
}

void CRepeaterProtocolHandler::readTempText(std::string& text)
{
	if (m_type != NETWORK_TEMPTEXT) {
		text = "                    ";
		return;
	}

	text = std::string((char*)(m_buffer + 5U), 0U, 20U);
}

std::string CRepeaterProtocolHandler::readStatus1()
{
	if (m_type != NETWORK_STATUS1)
		return "";

	return std::string((char*)(m_buffer + 6U), 0U, 20U);
}

std::string CRepeaterProtocolHandler::readStatus2()
{
	if (m_type != NETWORK_STATUS2)
		return "";

	return std::string((char*)(m_buffer + 6U), 0U, 20U);
}

std::string CRepeaterProtocolHandler::readStatus3()
{
	if (m_type != NETWORK_STATUS3)
		return "";

	return std::string((char*)(m_buffer + 6U), 0U, 20U);
}

std::string CRepeaterProtocolHandler::readStatus4()
{
	if (m_type != NETWORK_STATUS4)
		return "";

	return std::string((char*)(m_buffer + 6U), 0U, 20U);
}

std::string CRepeaterProtocolHandler::readStatus5()
{
	if (m_type != NETWORK_STATUS5)
		return "";

	return std::string((char*)(m_buffer + 6U), 0U, 20U);
}

void CRepeaterProtocolHandler::reset()
{
	m_inId = 0U;
}

void CRepeaterProtocolHandler::close()
{
	m_socket.close();
}
