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

#ifdef WIN32
#define MKDIR(x,y) mkdir(x)
#else
#define MKDIR(x,y) mkdir(x,y)
#endif

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


static void decrypt_retail_pkg(void){
	u8 key[0x10];
	u8 iv[0x10];

	if (be16(pkg + 0x06) != 2)
		fail("invalid pkg type: %x", be16(pkg + 0x06));

	if (key_get_simple("gpkg-key", key, 0x10) < 0)
		fail("failed to load the package key.");

	memcpy(iv, pkg + 0x70, 0x10);
	aes128ctr(key, iv, pkg + data_offset, data_size, pkg + data_offset);
}

static void decrypt_debug_pkg(void){
	u8 key[0x40];
	u8 bfr[0x1c];
	u64 i;

	memset(key, 0, sizeof key);
	memcpy(key, pkg + 0x60, 8);
	memcpy(key + 0x08, pkg + 0x60, 8);
	memcpy(key + 0x10, pkg + 0x60 + 0x08, 8);
	memcpy(key + 0x18, pkg + 0x60 + 0x08, 8);

	sha1(key, sizeof key, bfr);

	for (i = 0; i < data_size; i++) {
		if (i != 0 && (i % 16) == 0) {
			wle64(key + 0x38, le64(key + 0x38) + 1);	
			sha1(key, sizeof key, bfr);
		}
		pkg[data_offset + i] ^= bfr[i & 0xf];
	}
}



void analizeDATA(void){
	printf("\n[ DATA ]\n");

	u64 i;
	u64 n_files;
	u32 fname_len;
	u32 fname_off;
	u64 file_offset;
	u32 flags;
	char fname[256];
	u8 *tmp;
	u64 size;
	for(i=0;i<item_count;i++){

		tmp = pkg + data_offset + i*0x20;

		fname_off = le32(tmp) + data_offset;
		fname_len = le32(tmp + 0x04);
		file_offset = le64(tmp + 0x08) + data_offset;
		size = le64(tmp + 0x10);
		flags = le32(tmp + 0x18);

		if (fname_len >= sizeof fname)
			fail("filename too long: %s", pkg + fname_off);

		memset(fname, 0, sizeof fname);
		strncpy(fname, (char *)(pkg + fname_off), fname_len);

		flags &= 0xff;
		if (flags == 4)
			MKDIR(fname, 0777);
		else if (flags == 1 || flags == 3)
			memcpy_to_file(fname, pkg + file_offset, size);
		else
			fail("unknown flags: %08x", flags);

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


	if (pkg_type>=0x80000000)
		decrypt_retail_pkg();
	else
		decrypt_debug_pkg();

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


