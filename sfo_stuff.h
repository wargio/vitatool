#ifndef __SFO_STUFF__
#define __SFO_STUFF__
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tools.h"
#include "types.h"


#define SFO_0x4		0x4
#define SFO_0x8		0x8
#define SFO_0x12	0x12
#define SFO_0x16	0x16
#define SFO_0x30	0x30
#define SFO_0x32	0x32

#define UNKN		0x0004
#define UTF8		0x0204
#define INTG		0x0404

void analizeSFO(u8*,u32,u32);

#endif
