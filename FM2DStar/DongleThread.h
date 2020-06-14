/*
 *   Copyright (C) 2010-2020 by Jonathan Naylor G4KLX
 *  *Copyright (C) 2020 by Geoffrey Merck F4FXL - KC3FRA
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

#if !defined(DONGLETHREAD_H)
#define	DONGLETHREAD_H

#include "EncodeCallback.h"
#include "DecodeCallback.h"
#include "RingBuffer.h"
#include "FIRFilter.h"
#include "Thread.h"

class CDongleThread : public CThread {
public:
	CDongleThread();
	virtual ~CDongleThread();

	virtual void setEncodeCallback(IEncodeCallback* callback);
	virtual void setDecodeCallback(IDecodeCallback* callback);

	virtual bool open() = 0;

	virtual void setEncode() = 0;
	virtual void setDecode() = 0;
	virtual void setIdle() = 0;

	virtual unsigned int getEncodeSpace() const;
	virtual unsigned int getDecodeSpace() const;

	virtual unsigned int writeEncode(const float* audio, unsigned int length);
	virtual unsigned int writeDecode(const unsigned char* ambe, unsigned int length);

	virtual void setBleep(unsigned int bleep, unsigned int volume);
	virtual void kill();

protected:
	IEncodeCallback*           m_encodeCallback;
	IDecodeCallback*           m_decodeCallback;
	CRingBuffer<float>     m_encodeAudio;
	CRingBuffer<unsigned char> m_decodeData;
	unsigned int               m_bleep;
	unsigned int			   m_bleepVolume;

	bool                       m_killed;

	void reset();

	void upSample(const float* audioIn, float* audioOut);
	void downSample(const float* audioIn, float* audioOut);

	void sendBleep();

private:
	CFIRFilter m_filter;
};

#endif

