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
const char* HEADER4 = "Copyright(C) 2020 by Jonathan Naylor, G4KLX";
const char* HEADER5 = "Copyright(C) 2020 by Geoffrey Merck F4FXL - KC3FRA and others";

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
m_conf(confFile),
m_callsign(),
m_suffix(),
m_daemon(false),
m_logDisplayLevel(0U),
m_logFileLevel(0U),
m_logFilePath(),
m_logFileRoot(),
m_hostAdress(),
m_hostPort(0U),
m_localAddress(),
m_localPort()
{

}

int CFM2DStar::run()
{
	bool ret = m_conf.read();
	if (!ret) {
		::fprintf(stderr, "FM2DStar: cannot read the .ini file\n");
		return 1;
	}

#if !defined(_WIN32) && !defined(_WIN64)
	ret = ::LogInitialise(m_daemon, m_conf.getLogFilePath(), m_conf.getLogFileRoot(), m_conf.getLogFileLevel(), m_conf.getLogDisplayLevel());
#else
	ret = ::LogInitialise(false, m_conf.getLogFilePath(), m_conf.getLogFileRoot(), m_conf.getLogFileLevel(), m_conf.getLogDisplayLevel());
#endif
	if (!ret) {
		::fprintf(stderr, "FM2DStar: unable to open the log file\n");
		return 1;
	}

	LogInfo(HEADER1);
	LogInfo(HEADER2);
	LogInfo(HEADER3);
	LogInfo(HEADER4);
	LogInfo(HEADER5);

	LogMessage("FM2Dstar-%s is starting", VERSION);
	LogMessage("Built %s %s (GitID #%.7s)", __TIME__, __DATE__, gitversion);

	if(dropRoot() != 0)
		return 1;

	readParams();

	return 0;
}

int CFM2DStar::dropRoot()
{
	// If we are currently root...
	if ((m_daemon = m_conf.getDaemon()) && getuid() == 0) {
		struct passwd* user = ::getpwnam("mmdvm");
		if (user == NULL) {
			::fprintf(stderr, "Could not get the mmdvm user, exiting\n");
			return -1;
		}

		uid_t mmdvm_uid = user->pw_uid;
		gid_t mmdvm_gid = user->pw_gid;

		// Set user and group ID's to mmdvm:mmdvm
		if (::setgid(mmdvm_gid) != 0) {
			::fprintf(stderr, "Could not set mmdvm GID, exiting\n");
			return -1;
		}

		if (::setuid(mmdvm_uid) != 0) {
			::fprintf(stderr, "Could not set mmdvm UID, exiting\n");
			return -1;
		}

		// Double check it worked (AKA Paranoia)
		if (::setuid(0) != -1){
			::fprintf(stderr, "It's possible to regain root - something is wrong!, exiting\n");
			return -1;
		}
	}

	return 0;
} 

void CFM2DStar::readParams()
{
	m_callsign			= m_conf.getCallsign();
	m_suffix			= m_conf.getSuffix();
	m_daemon			= m_conf.getDaemon();
	m_logDisplayLevel	= m_conf.getLogDisplayLevel();
	m_logFileLevel		= m_conf.getLogFileLevel();
	m_logFilePath		= m_conf.getLogFilePath();
	m_logFileRoot		= m_conf.getLogFileRoot();
	m_hostAdress		= m_conf.getHostAddress();
	m_hostPort			= m_conf.getHostPort();
	m_localAddress		= m_conf.getLocalAddress();
	m_localPort			= m_conf.getLocalPort();
}