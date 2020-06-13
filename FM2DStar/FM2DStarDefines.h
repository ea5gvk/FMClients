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

#if !defined(FM2DSTARDEFINES_H)
#define FM2DSTARDEFINES_H

enum DONGLE_TYPE {
	DT_DVDONGLE,
	DT_DV3000_NETWORK,
	DT_DV3000_SERIAL,
	DT_STARDV_NETWORK,
	DT_STARDV_NETWORK2,
	DT_DVMEGA_AMBE
};

#define IS_NETWORK_DONGLE(dongle) (dongle == DT_DV3000_NETWORK || dongle == DT_STARDV_NETWORK || dongle == DT_STARDV_NETWORK || dongle == DT_STARDV_NETWORK2)

#endif