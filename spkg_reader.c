// Copyright 2012   Grazioli Giovanni <wargio@libero.it>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tools.h"
#include "little_endian.h"
#include "types.h"

#ifdef WIN32
#define MKDIR(x,y) mkdir(x)
#else
#define MKDIR(x,y) mkdir(x,y)
#endif

static u8 *pkg = NULL;

const u32 hdr_spkg = 0x53434500;

void readSPKG(){
	u32 offset=0;
	u32 hdr	         = be32(pkg);
	offset += sizeof(u32);
	u32 pkg_type	 = le32(pkg+offset);
	offset += sizeof(u32);
	if(hdr_spkg!=hdr)
		fail("[Unknown HDR] 0x%08X \nNot a PS VITA System PKG", hdr);
	if(pkg_type!=0x3)
		fail("[Unknown Type] 0x%X \nNot a PS VITA System PKG", pkg_type-0x80000000);

	u64 time         = le64(pkg+offset); offset += sizeof(u64); //??is it the time? it's static in all pkgs
	u64 hdr_size     = le64(pkg+offset); offset += sizeof(u64);
	u64 content_size = le64(pkg+offset); offset += sizeof(u64);
	u64 size         = le64(pkg+offset); offset += sizeof(u64);

	printf("Unknown      0x%016lx (%lu)\n",time,time);
	printf("Header  Size 0x%016lx (%lu Bytes)\n",hdr_size,hdr_size);
	printf("Content Size 0x%016lx (%lu Bytes)\n",content_size,content_size);
	printf("Package Size 0x%016lx (%lu Bytes)\n",size,size);
	
	offset = hdr_size;
	u32 unknown01 = le32(pkg+offset); offset += sizeof(u32);
	u32 unknown02 = le32(pkg+offset); offset += sizeof(u32);
	u64 unknown03 = le64(pkg+offset); offset += sizeof(u64);

	u64 version   = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown05 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown06 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown07 = le64(pkg+offset); offset += sizeof(u64);
	offset += sizeof(u64)+sizeof(u64);
	
	u64 unknown08 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown09 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown10 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown11 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown12 = le64(pkg+offset); offset += sizeof(u64);
	u64 unknown13 = le64(pkg+offset); offset += sizeof(u64);
	
	printf("Unknown      0x%08x (%u)\n",unknown01,unknown01);
	printf("Unknown      0x%08x (%u)\n",unknown02,unknown02);

	printf("Unknown      0x%016lx (%lu)\n",unknown03,unknown03);
	printf("Version      0x%016lx (%lu)\n",version,version);
	printf("Unknown      0x%016lx (%lu)\n",unknown05,unknown05);
	printf("Unknown      0x%016lx (%lu)\n",unknown06,unknown06);
	printf("Unknown      0x%016lx (%lu)\n",unknown07,unknown07);
	printf("Unknown      0x%016lx (%lu)\n",unknown08,unknown08);
	printf("Unknown      0x%016lx (%lu)\n",unknown09,unknown09);
	printf("Unknown      0x%016lx (%lu)\n",unknown10,unknown10);
	printf("Unknown      0x%016lx (%lu)\n",unknown11,unknown11);
	printf("Unknown      0x%016lx (%lu)\n",unknown12,unknown12);
	printf("Unknown      0x%016lx (%lu)\n",unknown13,unknown13);
}

int main(int argc, char *argv[]){

	if (argc == 2) {
		pkg = mmap_file(argv[1]);
		readSPKG();

	} else if(argc == 3) {
		if (strcmp(argv[1], "-d") != 0)
			fail("invalid option: %s", argv[1]);
		else
			set_dbg_flag();

		pkg = mmap_file(argv[2]);
		printf("[PKG File]    %s\n",argv[2]);
		readSPKG();

	}else {
		fail("usage: %s vita_system_package.pkg\n"
		     /*"          -d  | show more informations"*/,argv[0]);
	}


	return 0;
}

