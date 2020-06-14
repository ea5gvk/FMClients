/*
 *	Copyright (C) 2014,2015 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	ENCODECALLBACK_H
#define	ENCODECALLBACK_H

enum PTT_STATE {
	PS_NONE,
	PS_RECEIVE,
	PS_TRANSMIT
};

class IEncodeCallback {
public:
	virtual void encodeCallback(const unsigned char* ambe, unsigned int length, PTT_STATE state) = 0;

private:
};

#endif
