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

	u64 time        = le64(pkg+offset); //??is it the time? it's static in all pkgs
	offset += sizeof(u64);

	u32 hdr_size     = le32(pkg+offset);
	offset += sizeof(u64);

	u64 content_size        = le64(pkg+offset);
	offset += sizeof(u64);

	u64 size        = le64(pkg+offset);
	offset += sizeof(u64);

	printf("CreationTime 0x%08x%08x\n",(u32)time,(u32)(time>>32));
	printf("Header  Size 0x%08x (%u Bytes)\n",(u32)hdr_size,(u32)hdr_size);
	printf("Content Size 0x%08x%08x (%u%u Bytes)\n",(u32)content_size,(u32)(content_size>>32),(u32)content_size,(u32)(content_size>>32));
	printf("Package Size 0x%08x%08x (%u%u Bytes)\n",(u32)size,(u32)(size>>32),(u32)size,(u32)(size>>32));

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
		     "          -d  | show more informations",argv[0]);
	}


	return 0;
}

