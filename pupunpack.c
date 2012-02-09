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

static u8 *pup = NULL;
static u32 file_count;

static int read_only = 0;

static struct id2name_tbl t_names[] = {
	{0x100, "version.txt"},
	{0x101, "license.xml"},
	{0x200, "psp2swu.self"},
	{0x201, "exec_file1.self"},
	{0x202, "exec_file2.self"},
	{0x203, "exec_file3.self"},
	{0x204, "exec_file4.self"},
	{0x301, "package_data01.pkg"},
	{0x302, "package_data02.pkg"},
	{0x303, "package_data03.pkg"},
	{0x304, "package_data04.pkg"},
	{0x305, "package_data05.pkg"},
	{0x306, "package_data06.pkg"},
	{0x307, "package_data07.pkg"},
	{0x308, "package_data08.pkg"},
	{0x309, "package_data09.pkg"},
	{0x30a, "package_data10.pkg"},
	{0x30b, "package_data11.pkg"},
	{0x30c, "package_data12.pkg"},
	{0x30d, "package_data13.pkg"},
	{0x30e, "package_data14.pkg"},
	{0x30f, "package_data15.pkg"},
	{0x310, "package_data16.pkg"},
	{0x311, "package_data17.pkg"},
	{0x312, "package_data18.pkg"},
	{0x313, "package_data19.pkg"},
	{0x314, "package_data20.pkg"},
	{0x400, "package_scewm.wm"},
	{0x401, "package_sceas.as"},
	{0, NULL}
};



void Unpup(){
	u32 i;
	const char *file_name;
	if(read_only!=0)
		printf("Read Only Mode!\n");
	printf("Reading...\n");
	u64 HDR = be64(pup);
	u32 pkg_version = le32(pup+0x08);
	u32 img_version = le32(pup+0x10);
	file_count = le32(pup+0x18);
	u32 hdr_lenght = le32(pup+0x20);
	u32 pkg_lenght = le32(pup+0x28);
/*
	//this is useless for now..
	u8 security_1[0x50];
	for(i=0;i<0x50;i+=0x1)
		security_1[i] = be64(pup+0x30+i);
*/

	dmsg("[HDR]          0x%08x%08x\n",HDR,HDR>>32);
	dmsg("[PKG][VERSION] 0x%08x\n",pkg_version);
	dmsg("[IMG][VERSION] 0x%08x\n",img_version);
	printf("[N Files]      %u\n",file_count);
	dmsg("[HDR][Lenght]  0x%08x\n",hdr_lenght);
	dmsg("[PKG][Lenght]  0x%08x\n",pkg_lenght);
	dmsg("[TableLenght]  0x%08x\n",0x80+(0x20*file_count));
	u32 entry,offset,size;
	for(i=0;i<file_count;i+=0x1){
		entry  = le32(pup+0x80+0x20*i);
		offset = le32(pup+0x80+0x20*i+0x08);
		size   = le32(pup+0x80+0x20*i+0x10);

		file_name = id2name(entry, t_names, NULL);
		if(file_name==NULL)
			fail("unknown entry id: 0x%08x | Offset: 0x%08x ",entry,offset);
		dmsg("Offset: %08x  ",offset);
		printf("Found: %20s | size: %10u Bytes\n",file_name,size);
		if(read_only!=1)
			memcpy_to_file(file_name, pup + offset, size);
	}

	printf("Done!\n");
}

int main(int argc, char *argv[]){

	if (argc == 2) {
		pup = mmap_file(argv[1]);
		printf( "PUP Unpacker\n");
		Unpup();

	} else if(argc == 3) {
		if (strcmp(argv[1], "-d") != 0)
			if(strcmp(argv[1], "-rd") !=0)
				fail("invalid option: %s", argv[1]);
		
		if(strcmp(argv[1], "-d")==0)
			set_dbg_flag();

		if(strcmp(argv[1], "-rd")==0)
			read_only=1;

		printf( "PUP Unpacker\n");
		pup = mmap_file(argv[2]);
		dmsg("[PUP File] %s\n",argv[2]);
		Unpup();

	}else {
		fail("usage: %s PSP2UPDAT.PUP\n	-d	| show more informations\n	-rd	| read only mode",argv[0]);
	}


	return 0;
}
