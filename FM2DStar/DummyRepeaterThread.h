/*
 *   Copyright (C) 2010,2011,2012,2014,2015 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterThread_H
#define	DummyRepeaterThread_H

#include "RepeaterProtocolHandler.h"
#include "FM2DStarDefines.h"
#include "SlowDataEncoder.h"
#include "SlowDataDecoder.h"
#include "EncodeCallback.h"
#include "DecodeCallback.h"
// #include "AudioCallback.h"
#include "DongleThread.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "Timer.h"
#include "Thread.h"
#include "RingBuffer.h"

#include <string>

enum CLIENT_STATE {
	CLIENT_RECEIVE,
	CLIENT_TRANSMIT,
	CLIENT_WANT_RECEIVE
};

class CDummyRepeaterThread : public CThread, /*public IAudioCallback,*/ public IEncodeCallback, public IDecodeCallback {
public:
	CDummyRepeaterThread();
	virtual ~CDummyRepeaterThread();

	virtual void readCallback(const float* input, unsigned int nSamples, int id);
	virtual void writeCallback(float* input, unsigned int& nSamples, int id);

	virtual void encodeCallback(const unsigned char* ambe, unsigned int length, PTT_STATE state);
	virtual void decodeCallback(const float* audio, unsigned int length);

	virtual void setCallsign(const std::string& callsign1, const std::string& callsign2);
	virtual void setDongle(CDongleThread* dongle);
	virtual void setProtocol(CRepeaterProtocolHandler* protocol);
	virtual void setMessage(const std::string& text);
	virtual void setBleep(unsigned int bleep, unsigned int volume);

	virtual void setYour(const std::string& your);
	virtual void setRpt1(const std::string& rpt1, const std::string& band);
	virtual void setRpt2(const std::string& rpt2, const std::string& band);

	virtual bool setTransmit(bool transmit);

	virtual void entry();
	virtual void  kill();

private:
	CDongleThread*                m_dongle;
	CSlowDataEncoder              m_slowDataEncoder;
	CSlowDataDecoder              m_slowDataDecoder;
	CRepeaterProtocolHandler*     m_protocol;
	bool                          m_stopped;
	CRingBuffer<float>        m_decodeAudio;
	CRingBuffer<unsigned char>    m_encodeData;
	CLIENT_STATE                  m_transmit;
	std::string                      m_callsign1;
	std::string                      m_callsign2;
	std::string                      m_your;
	std::string                      m_rpt1;
	std::string                      m_rpt2;
	std::string                      m_message;
	unsigned int                  m_frameCount;
	unsigned int                  m_networkSeqNo;
	bool                          m_killed;
	bool                          m_started;
	CTimer                        m_watchdog;
	CTimer                        m_poll;
	unsigned int                  m_clockCount;
	bool                          m_busy;
	bool                          m_localTX;
	bool                          m_externalTX;

	void receive();
	void transmit();

	void resetReceiver();
	void processHeader(CHeaderData* header);
	bool processFrame(const unsigned char* buffer, unsigned char seqNo);

	void serviceNetwork();

	void checkController();
};

#endif
