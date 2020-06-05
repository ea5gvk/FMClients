/*
 *   Copyright (C) 2015-2020 by Jonathan Naylor G4KLX
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

#include "Version.h"
#include "Log.h"
#include "FM2DStar.h"
#include "GitVersion.h"

#include <cstdio>
#include <vector>

#include <cstdlib>

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
const char* DEFAULT_INI_FILE = "FM2DStar.ini";
#else
const char* DEFAULT_INI_FILE = "/etc/FM2DStar.ini";
#endif

using namespace std;

static bool m_killed = false;
static int  m_signal = 0;

#if !defined(_WIN32) && !defined(_WIN64)
static void sigHandler(int signum)
{
	m_killed = true;
	m_signal = signum;
}
#endif

const char* HEADER1 = "This software is for use on amateur radio networks only,";
const char* HEADER2 = "it is to be used for educational purposes only. Its use on";
const char* HEADER3 = "commercial networks is strictly prohibited.";
const char* HEADER4 = "Copyright(C) 2015-2020 by Jonathan Naylor, G4KLX and others";

int main(int argc, char** argv)
{
	const char* iniFile = DEFAULT_INI_FILE;
	if (argc > 1) {
 		for (int currentArg = 1; currentArg < argc; ++currentArg) {
			std::string arg = argv[currentArg];
			if ((arg == "-v") || (arg == "--version")) {
				::fprintf(stdout, "FM2DStar version %s git #%.7s\n", VERSION, gitversion);
				return 0;
			} else if (arg.substr(0,1) == "-") {
				::fprintf(stderr, "Usage: FM2DStar [-v|--version] [filename]\n");
				return 1;
			} else {
				iniFile = argv[currentArg];
			}
		}
	}

#if !defined(_WIN32) && !defined(_WIN64)
	::signal(SIGINT,  sigHandler);
	::signal(SIGTERM, sigHandler);
	::signal(SIGHUP,  sigHandler);
#endif

	int ret = 0;

	do {
		m_signal = 0;

		CFM2DStar* fm2dstar = new CFM2DStar(string(iniFile));
		ret = fm2dstar->run();

		delete fm2dstar;

		if (m_signal == 2)
			::LogInfo("FM2DStar-%s exited on receipt of SIGINT", VERSION);

		if (m_signal == 15)
			::LogInfo("FM2DStar-%s exited on receipt of SIGTERM", VERSION);

		if (m_signal == 1)
			::LogInfo("FM2DStar-%s is restarting on receipt of SIGHUP", VERSION);
	} while (m_signal == 1);

	::LogFinalise();

	return ret;
}

CFM2DStar::CFM2DStar(const string& confFile) :
m_conf(confFile)
{

}

int CFM2DStar::run()
{
    return 0;
}