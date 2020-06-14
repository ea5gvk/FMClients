/*
 *	Copyright (C) 2014 by Jonathan Naylor, G4KLX
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

#if	!defined(DECODECALLBACK_H)
#define	DECODECALLBACK_H


class IDecodeCallback {
public:
	virtual void decodeCallback(const float* audio, unsigned int length) = 0;

private:
};

#endif
