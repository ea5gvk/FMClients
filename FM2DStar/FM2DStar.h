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

#if !defined(FM2DSTAR_H)
#define FM2DSTAR_H

#include "Conf.h"
#include "FM2DStarDefines.h"
#include "DummyRepeaterThread.h"

using namespace std;

class CFM2DStar {
public :
    CFM2DStar(const string& confFile);

    int run();

private :
    int dropRoot();
    void readParams();

    CDongleThread * createDongleThread();

    CConf m_conf;

    std::string m_callsign;
    std::string m_suffix;
    bool m_daemon;

    unsigned int m_logDisplayLevel;
    unsigned int m_logFileLevel;
    std::string  m_logFilePath;
    std::string  m_logFileRoot;

    std::string  m_hostAdress;
    unsigned int m_hostPort;
    std::string  m_localAddress;
    unsigned int m_localPort;

    std::string m_dummyRptrCallsign;
    std::string m_dummyRptrBand;

    DONGLE_TYPE  m_dongleType;
    std::string  m_dongleAddress;
    unsigned int m_donglePort;
    std::string  m_dongleSerialPort;

    CDummyRepeaterThread * m_thread;
};

#endif