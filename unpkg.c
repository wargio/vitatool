// Copyright 2012       Grazioli Giovanni <wargio@libero.it>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tools.h"
#include "types.h"
#include "little_endian.h"

static int read_only = 0;

static u8 *pkg = NULL;
const u32 PKG_HDR = 0x53434500;

void Extract(const char* name, u32 offset, u32 size){
	printf("Extracting %s..\n",name);
	memcpy_to_file(name,pkg+offset,size);
}

void unpkg(){
	if(read_only!=0)
		printf("Read Only Mode!\n");
	printf("Reading...\n");

	u32 HDR		= be32(pkg);
	u16 type	= le16(pkg+0x0a);
	u32 data1_size	= le32(pkg+0x10)-0x30;
	u32 data2_size	= le32(pkg+0x18)-0x70;
	u32 pkg_size	= le32(pkg+0x20);
	u32 data2_ver	= le32(pkg+(data1_size+0x10));
	if(HDR!=PKG_HDR)
		fail("\nERROR! Not a PlayStation Vita Update File (%08x)",HDR);

	if(type!=3)
		fail("Not a PSVITA System Package (%X)",type);
	dmsg("[PKG][SIZE]  %10u Bytes\n",pkg_size);
	printf("Found: data1 %10u Bytes\n",data1_size);
	
	if(read_only==0)
		Extract("data1", 0x30 , data1_size);
	printf("Found: data2 %10u Bytes\n",data2_size);
	dmsg("[DATA2][VER] %08x\n",data2_ver);
	if(read_only==0)
		Extract("data2", 0x60 + data1_size + 0x50, data2_size);
	
	
	
	
	printf("Done!\n");
}


int main(int argc, char *argv[]){

	if (argc == 2) {
		pkg = mmap_file(argv[1]);
		unpkg();

	} else if(argc == 3) {
		if (strcmp(argv[1], "-d") != 0)
			if(strcmp(argv[1], "-r") !=0)
				if(strcmp(argv[1], "-rd") !=0)
					fail("invalid option: %s", argv[1]);
		
		if(strcmp(argv[1], "-d")==0)
			set_dbg_flag();

		if(strcmp(argv[1], "-r")==0)
			read_only=1;

		if(strcmp(argv[1], "-rd")==0){
			read_only=1;
			set_dbg_flag();
		}

		pkg = mmap_file(argv[2]);
		dmsg("[PKG File] %s\n",argv[2]);
		unpkg();

	}else {
		fail("usage: %s ps_vita_system_package.pkg\n"
			"	-d	| debug messages\n"
			"	-r	| read only mode\n"
			"	-rd	| read only + debug messages"
			,argv[0]);
	}


	return 0;
}
