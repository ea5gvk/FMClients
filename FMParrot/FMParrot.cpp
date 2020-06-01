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

#include "FMNetwork.h"
#include "FMParrot.h"
#include "StopWatch.h"
#include "Version.h"
#include "Parrot.h"
#include "Thread.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
	if (argc == 1) {
		::fprintf(stderr, "Usage: FMParrot <port>\n");
		return 1;
	}

	unsigned int port = ::atoi(argv[1U]);
	if (port == 0U) {
		::fprintf(stderr, "FMParrot: invalid port number - %s\n", argv[1U]);
		return 1;
	}

	CFMParrot parrot(port);
	parrot.run();

	return 0;
}

CFMParrot::CFMParrot(unsigned int port) :
m_port(port)
{
}

CFMParrot::~CFMParrot()
{
}

void CFMParrot::run()
{
	CParrot parrot(180U);
	CFMNetwork network(m_port);

	bool ret = network.open();
	if (!ret)
		return;

	CStopWatch stopWatch;
	stopWatch.start();

	CTimer watchdogTimer(1000U, 0U, 1500U);
	CTimer turnaroundTimer(1000U, 2U);

	CStopWatch playoutTimer;
	unsigned int count = 0U;
	bool playing = false;

	::fprintf(stdout, "Starting FMParrot-%s\n", VERSION);

	for (;;) {
		unsigned char buffer[339U];

		unsigned int len = network.read(buffer, 339U);
		if (len > 0U) {
			if (len >= 3 && ::memcmp(buffer, "FME", 3U) == 0) {
				::fprintf(stdout, "Received end of transmission\n");
				turnaroundTimer.start();
				watchdogTimer.stop();
				parrot.end();
			} else {
				parrot.write(buffer, len);
				watchdogTimer.start();
			}			
		}

		if (turnaroundTimer.isRunning() && turnaroundTimer.hasExpired()) {
			if (!playing) {
				playoutTimer.start();
				playing = true;
				count = 0U;
				::fprintf(stdout, "Started\n");
			}

			// A frame every 21 ms
			unsigned int wanted = playoutTimer.elapsed()  / 21000;
			while (count < wanted) {
				len = parrot.read(buffer);
				if (len > 0U) {
					network.write(buffer, len);
					//::fprintf(stdout, "Count %d, Wanted %d\n", count, wanted);
					count ++ ;
				} else {
					::fprintf(stdout, "Finished\n");
					parrot.clear();
					network.end();
					turnaroundTimer.stop();
					playing = false;
					count = wanted;
				}
			}
		}

		unsigned int ms = stopWatch.elapsedMilliSeconds();
		stopWatch.start();

		watchdogTimer.clock(ms);
		turnaroundTimer.clock(ms);

		if (watchdogTimer.isRunning() && watchdogTimer.hasExpired()) {
			turnaroundTimer.start();
			watchdogTimer.stop();
			parrot.end();
		}

		if (ms < 5U)
			CThread::sleep(5U);
	}

	network.close();
}
