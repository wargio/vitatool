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
#include "sfo_stuff.h"

static u8 *pkg = NULL;

u32 sc_header = 0x7f504b47; //already big endian!
static u32 info;
static u32 sfo_off;
static u32 sfo_size;


//PSF 00 50 53 46
void readPKG(void){
	int i;
	u32 hdr;
	u64 pkg_size,content_size;
	char pkg_name[0x2f];

	hdr	 = be32(pkg);
	info	 = be32(pkg+0x08);
	pkg_size = be64(pkg+0x18);
	content_size = be64(pkg+0x28);
	sfo_off = be32(pkg+info+0x98);
	sfo_size = be32(pkg+info+0x9c);

	for(i=0x0;i<0x2f;i++)
		pkg_name[i] = be8(pkg+(0x30+i));

	if(hdr!=sc_header){
		printf("[UNKNOWN HDR] 0x%08x \n", hdr);
		fail("Not a PS VITA Game PKG");
	}
	if(dbg()==1){
		printf("[PKG HDR ] 0x%08x \n", hdr);
		printf("[Info offset] 0x%08x \n", info);
	}
	printf("[PKG Size] %u Bytes\n",(unsigned int)pkg_size);
	printf("[PKG Name] %24s\n",pkg_name);
	printf("[Content  Size ] %u Bytes\n",(unsigned int)content_size);
	if(dbg()==1){
		printf("[Content Offset] 0x%x\n",(unsigned int)be64(pkg+0x20));
	}
	
	analizeSFO(pkg,sfo_off,sfo_size);
	printf("[Content]\n");
}

int main(int argc, char *argv[]){

	if (argc == 2) {
		pkg = mmap_file(argv[1]);
		printf("[PKG File] %s\n",argv[1]);
		readPKG();

	} else if(argc == 3) {
		if (strcmp(argv[1], "-d") != 0)
			fail("invalid option: %s", argv[1]);
		else
			set_dbg_flag();

		pkg = mmap_file(argv[2]);
		printf("[PKG File] %s\n",argv[2]);
		readPKG();

	}else {
		fail("usage: %s vita_game.pkg\n	-d  | show more informations",argv[0]);
	}


	return 0;
}

