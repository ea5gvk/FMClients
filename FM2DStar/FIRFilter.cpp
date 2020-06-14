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

#include "FIRFilter.h"
#include <string.h>
#include <cassert>

CFIRFilter::CFIRFilter(const float* taps, int tapsLength)
{
    assert(taps != NULL);

    m_taps = new float[tapsLength];
    m_buffer = new float[tapsLength];
    m_tapsLength = tapsLength;

    ::memcpy(m_taps, taps, tapsLength * sizeof(float));
    ::memset(m_buffer, 0, tapsLength * sizeof(float));
    m_currentBufferPosition = 0U;
}

CFIRFilter::~CFIRFilter()
{
    delete[] m_taps;
    delete[] m_buffer;
}

float CFIRFilter::process(float input)
{
    float output = 0.0f;
    unsigned int iTaps = 0U;

    // Buffer latest sample into delay line
    m_buffer[m_currentBufferPosition] = input;

    for(unsigned int i = m_currentBufferPosition; i >= 0U; i--)
    {
        output += m_taps[iTaps++] * m_buffer[i];
    }

    for(unsigned int i = m_tapsLength - 1U; i > m_currentBufferPosition; i--)
    {
        output += m_taps[iTaps++] * m_buffer[i];
    }
    
    m_currentBufferPosition = (m_currentBufferPosition + 1) % m_tapsLength;

    return output;
}

void CFIRFilter::reset()
{
    m_currentBufferPosition = 0U;
    ::memset(m_buffer, 0, m_tapsLength * sizeof(float));
}

