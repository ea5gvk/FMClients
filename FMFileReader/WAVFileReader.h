/*
 *   Copyright (C) 2002-2004,2006-2010,2020 by Jonathan Naylor G4KLX
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

#ifndef	WAVFileReader_H
#define WAVFileReader_H

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <WS2tcpip.h>
#include <windows.h>
#include <mmsystem.h>
#else
#include <cstdio>
#endif

enum WAVFORMAT {
	FORMAT_8BIT,
	FORMAT_16BIT,
	FORMAT_32BIT
};

class CWAVFileReader {
public:
	CWAVFileReader(const std::string& fileName, unsigned int blockSize);
	~CWAVFileReader();

	std::string     getFilename() const;

	bool         open();
	unsigned int read(float* data, unsigned int length);
	void         rewind();
	void         close();

	unsigned int getSampleRate() const;
	unsigned int getChannels() const;

private:
	std::string    m_fileName;
	unsigned int   m_blockSize;
	WAVFORMAT      m_format;
	unsigned int   m_channels;
	unsigned int   m_sampleRate;
	uint8_t*       m_buffer8;
	int16_t*       m_buffer16;
	float*         m_buffer32;
#if defined(_WIN32) || defined(_WIN64)
	HMMIO          m_handle;
	MMCKINFO       m_parent;
	MMCKINFO       m_child;
	LONG           m_offset;
#else
	FILE*          m_file;
	unsigned long  m_offset;
	unsigned int   m_length;
#endif
};

#endif
