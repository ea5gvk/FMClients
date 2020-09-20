/*
*   Copyright (C) 2020 by Jonathan Naylor G4KLX
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

#include "FMFileReader.h"
#include "WAVFileReader.h"
#include "FMNetwork.h"
#include "StopWatch.h"
#include "Version.h"
#include "Thread.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
	if (argc < 3) {
		::fprintf(stderr, "Usage: FMFileReader <file> <port>\n");
		return 1;
	}

	unsigned int port = ::atoi(argv[2U]);
	if (port == 0U) {
		::fprintf(stderr, "FMFileReader: invalid port number - %s\n", argv[1U]);
		return 1;
	}

	CFMFileReader FileReader(std::string(argv[1U]), port);
	FileReader.run();

	return 0;
}

CFMFileReader::CFMFileReader(const std::string& filename, unsigned int port) :
m_filename(filename),
m_port(port)
{
	CUDPSocket::startup();
}

CFMFileReader::~CFMFileReader()
{
	CUDPSocket::shutdown();
}

void CFMFileReader::run()
{
	CWAVFileReader reader(m_filename, 200U);

	bool ret = reader.open();
	if (!ret)
		return;

	if (reader.getChannels() > 1U) {
		reader.close();
		return;
	}

	if (reader.getSampleRate() != 8000U) {
		reader.close();
		return;
	}

	CFMNetwork network(m_port);

	ret = network.open();
	if (!ret) {
		reader.close();
		return;
	}

	CStopWatch stopWatch;
	stopWatch.start();

	CStopWatch playoutTimer;
	unsigned int count = 0U;

	::fprintf(stdout, "Starting FMFileReader-%s\n", VERSION);

	playoutTimer.start();
	::fprintf(stdout, "Started\n");

	bool running = true;
	while (running) {
		// A frame every 21 ms
		unsigned int wanted = playoutTimer.elapsed()  / 21000U;
		while (count < wanted) {
			float audio[200U];
			unsigned int len = reader.read(audio, 168U);
			if (len > 0U) {
				unsigned short buffer[200U];
				for (unsigned int i = 0U; i < len; i++)
					buffer[i] = (unsigned short)((audio[i] + 1.0F) * 32767.0F);
				network.write((unsigned char*)buffer, len * 2U);
				//::fprintf(stdout, "Count %d, Wanted %d\n", count, wanted);
				count++ ;
			} else {
				::fprintf(stdout, "Finished\n");
				network.end();
				running = false;
			}
		}

		unsigned int ms = stopWatch.elapsedMilliSeconds();
		stopWatch.start();

		if (ms < 5U)
			CThread::sleep(5U);
	}

	reader.close();
	network.close();
}
