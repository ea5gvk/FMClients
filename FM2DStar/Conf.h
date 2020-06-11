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

#if !defined(CONF_H)
#define	CONF_H

#include <string>
#include <vector>

class CConf
{
public:
  CConf(const std::string& file);
  ~CConf();

  bool read();

  // The General section
  std::string  getCallsign() const;
  std::string  getSuffix() const;
  bool         getDaemon() const;

  // The Log section
  unsigned int getLogDisplayLevel() const;
  unsigned int getLogFileLevel() const;
  std::string  getLogFilePath() const;
  std::string  getLogFileRoot() const;

  // The Network section
  std::string getHostAddress() const;
  unsigned int getHostPort() const;
  std::string getLocalAddress() const;
  unsigned int getLocalPort() const;

  // The dummy repeater section
  std::string getDummyRepeaterCallsign() const;
  std::string getDummyRepeaterBand() const;

private:
  std::string m_file;

  // The General section
  std::string m_callsign;
  std::string m_suffix;
  bool m_daemon;

  // The Log section
  unsigned int m_logDisplayLevel;
  unsigned int m_logFileLevel;
  std::string  m_logFilePath;
  std::string  m_logFileRoot;

  // The network section
  std::string m_hostAdress;
  unsigned int m_hostPort;
  std::string m_localAddress;
  unsigned int m_localPort;

  // The dummy repeater section
  std::string m_dummyRptrCallsign;
  std::string m_dummyRptrBand;
};

#endif
