#ifndef __READSELF_H__
#define __READSELF_H__

#include "little_endian.h"

//PROGRAM TYPES

#define PT_NULL 	0
#define PT_LOAD 	1
#define PT_DYNAMIC 	2
#define PT_INTERP 	3
#define PT_NOTE 	4
#define PT_SHLIB 	5
#define PT_PHDR 	6
#define PT_TLS 		7
#define PT_LOOS 	0x60000000
#define PT_HIOS 	0x6fffffff
#define PT_LOPROC 	0x70000000
#define PT_HIPROC 	0x7fffffff

//PROGRAM FLAGS

#define PF_X 		0x1 		//Execute
#define PF_W 		0x2 		//Write
#define PF_R 		0x4 		//Read
#define PF_MASKOS 	0x0ff00000 	//Unspecified
#define PF_MASKPROC 	0xf0000000 	//Unspecified



const char *prg_type(u32 type){
	if(type==PT_NULL)
		return "PT_NULL";
	else if(type==PT_LOAD)
		return "PT_LOAD";
	else if(type==PT_DYNAMIC)
		return "PT_DYNAMIC";
	else if(type==PT_INTERP)
		return "PT_INTERP";
	else if(type==PT_NOTE)
		return "PT_NOTE";
	else if(type==PT_SHLIB)
		return "PT_SHLIB";
	else if(type==PT_PHDR)
		return "PT_PHDR";
	else if(type==PT_TLS)
		return "PT_TLS";
	else if(type==PT_LOOS)
		return "PT_LOOS";
	else if(type==PT_HIOS)
		return "PT_HIOS";
	else if(type==PT_LOPROC)
		return "PT_LOPROC";
	else if(type==PT_HIPROC)
		return "PT_HIPROC";
	else
		return "PT_UNKNOWN";
}


const char *prg_flags(u32 flags){
	if(flags==PF_X)
		return "PF_X";
	else if(flags==PF_W)
		return "PF_W";
	else if(flags==PF_R)
		return "PF_DYNAMIC";
	else if(flags==PF_MASKOS)
		return "PF_MASKOS";
	else if(flags==PF_MASKPROC)
		return "PF_MASKPROC";
	else
		return "PF_UNKNOWN";
}

#endif
