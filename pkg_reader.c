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
#include "types.h"
#include "sfo_stuff.h"

static u8 *pkg = NULL;

u32 sc_header = 0x7f504b47;
u32 hdr;
u64 pkg_size;
u32 pkg_type;
u32 pkg_info_offset;
u32 pkg_info_size;
u32 header_size;
u32 item_count;
u64 data_offset;
u64 data_size;
char contentid[0x30];
u8 digest[0x10];
u8 k_licensee[0x10];

/*

filename_offset 0x0 	u32 	
filename_size 	0x4 	u32 	
data_offset 	0x8 	u64 	
data_size 	0x10 	u64 	
flags 		0x18 	u32 	The file type
padding 	0x1C 	u32 	zero

*/

void analizeDATA(void){
	printf("\n[ DATA ]\n");

	int i;
	u32 filename_offset;
	u32 filename_size;
	u64 data_offset;
	u64 data_size;
	u32 flags;
	u32 padding;
	
	for(i=0;i<item_count;i++){



	}
}

void readPKG(void){
	int i;
	hdr		 = be32(pkg);

	if(hdr!=sc_header)
		fail("[Unknown HDR] 0x%08X \nNot a PS VITA Game PKG", hdr);
	
	pkg_type	= be32(pkg+0x04);
	
	if(pkg_type-0x80000000!=0x2)
		fail("[Unknown Type] 0x%X \nNot a PS VITA Game PKG", pkg_type-0x80000000);
	
	pkg_info_offset	= be32(pkg+0x08);
	pkg_info_size	= be32(pkg+0x0C);
	header_size	= be32(pkg+0x10);
	item_count	= be32(pkg+0x14);
	pkg_size	= be64(pkg+0x18);
	data_offset	= be64(pkg+0x20);
	data_size	= be64(pkg+0x28);
	
	for(i=0x0;i<0x30;i++)
		contentid[i]	 = be8(pkg+0x30+i);
	
	for(i=0x0;i<0x10;i++){
		digest[i]	 = be8(pkg+0x60+i);
		k_licensee[i]	 = be8(pkg+0x70+i);
	}

	u32 sfo_off	= be32(pkg+pkg_info_offset+0x30+(pkg_info_size*8));
	u32 sfo_size	= be32(pkg+pkg_info_offset+0x34+(pkg_info_size*8));

	dmsg("[PKG HDR ]    0x%08x \n", hdr);
	dmsg("[Info offset] 0x%08x \n", pkg_info_offset);

	if(pkg_type>=0x80000000)
		printf("[Retail]      0x%x\n",(unsigned int)pkg_type);
	else
		printf("[Not Retail]  0x%x\n",(unsigned int)pkg_type);


	printf("[PKG  Size]   %u Bytes\n",(unsigned int)pkg_size);
	printf("[Content ID]  0x%s\n",contentid);
	printf("[Data Size]   %u Bytes\n",(unsigned int)data_size);
	dmsg("[Data Offset] 0x%x\n",(unsigned int)data_offset);
	
	printf("[Digest]      0x");
	for(i=0x0;i<0x10;i++)
		printf("%x",digest[i]);	
	printf("\n[K Licensee]  0x");
	for(i=0x0;i<0x10;i++)
		printf("%x",k_licensee[i]);	
	printf("\n");
	analizeSFO(pkg,sfo_off,sfo_size);
	analizeDATA();
}

int main(int argc, char *argv[]){

	if (argc == 2) {
		pkg = mmap_file(argv[1]);
		printf("[PKG File]    %s\n",argv[1]);
		readPKG();

	} else if(argc == 3) {
		if (strcmp(argv[1], "-d") != 0)
			fail("invalid option: %s", argv[1]);
		else
			set_dbg_flag();

		pkg = mmap_file(argv[2]);
		printf("[PKG File]    %s\n",argv[2]);
		readPKG();

	}else {
		fail("usage: %s vita_game.pkg\n	-d  | show more informations",argv[0]);
	}


	return 0;
}


