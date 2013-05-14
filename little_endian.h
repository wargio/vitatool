// Copyright 2012       Grazioli Giovanni <wargio@libero.it>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#ifndef _ENDIANESS_TOOLS_H__
#define _ENDIANESS_TOOLS_H__
#include "types.h"

static inline u8 le8(u8 *p)
{
	return *p;
}

static inline u16 le16(u8 *p)
{
	u16 a;

	a  = p[1] << 8;
	a |= p[0];

	return a;
}

static inline u32 le32(u8 *p)
{
	u32 a;

	a  = p[3] << 24;
	a |= p[2] << 16;
	a |= p[1] <<  8;
	a |= p[0] <<  0;

	return a;
}

static inline u64 le64(u8 *p)
{
	u32 a, b;

	a = le32(p + 4);
	b = le32(p);

	return ((u64)a<<32) | b;
}

static inline void wle16(u8 *p, u16 v)
{
	p[1] = v >>  8;
	p[2] = v;
}

static inline void wle32(u8 *p, u32 v)
{
	p[3] = v >> 24;
	p[2] = v >> 16;
	p[1] = v >>  8;
	p[0] = v;
}

static inline void wle64(u8 *p, u64 v)
{
	wle32(p + 4, v);
	v >>= 32;
	wle32(p, v);
}

#endif
