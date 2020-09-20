/*
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

#if !defined(FIRFILTER_H)
#define FIRFILTER_H

#if defined(_WIN32) || defined(_WIN64)
#include <WS2tcpip.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

class CFIRFilter
{
public :
    //Constructor
    CFIRFilter(const float* taps, int tapsLength);

    // Destructor
    ~CFIRFilter();

    // Processing
    float process(float sample);

    void reset();

private:
    float* m_taps;
    unsigned int    m_tapsLength;
    float* m_buffer;
    unsigned int    m_currentBufferPosition;
};

#endif //cfirfilter_h
