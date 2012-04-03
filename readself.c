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


static u8 *self = NULL;

const u32 HDR = 0x53434500;

void readself(){
	printf("Reading...\n");

	u32 magic               = be32(self);       /* 53434500 = SCE\0 */
	u32 version             = le32(self+0x4 );  /* header version 3*/
	u16 sdk_type            = le16(self+0x8 );  /* */
	u16 header_type         = le16(self+0xA );  /* 1 self, 2 unknown, 3 pkg */
	u32 metadata_offset     = le32(self+0xC );  /* metadata offset */
	u64 header_len          = le64(self+0x10);  /* self header length */
	u64 elf_filesize        = le64(self+0x18);  /* ELF file length */
	u64 self_filesize       = le64(self+0x20);  /* SELF file length */
	u64 unknown1            = le64(self+0x28);  /* UNKNOWN */
	u64 unknown2            = le64(self+0x30);  /* value is 4 */
	u64 appinfo_offset      = le64(self+0x38);  /* app info offset */
	u64 elf_offset          = le64(self+0x40);  /* ELF #1 offset */
	u64 phdr_offset         = le64(self+0x48);  /* program header offset */
	u64 shdr_offset         = le64(self+0x50);  /* section header offset */
	u64 section_info_offset = le64(self+0x58);  /* section info offset */
	u64 sceversion_offset   = le64(self+0x60);  /* version offset */
	u64 controlinfo_offset  = le64(self+0x68);  /* control info offset */
	u64 controlinfo_size    = le64(self+0x70);  /* control info size */
	u64 padding             = le64(self+0x78);

	if(magic!=HDR)
		fail("\nERROR! Not a PlayStation Vita Self File (Magic: %08x)",magic);

	if(header_type!=1)
		fail("\nERROR! Not a PlayStation Vita Self File (Type : %08x)",header_type);

	printf("Magic                   0x%08x\n",magic);
	printf("Version                 0x%08x\n",version);
	printf("SDK Type                0x%08x\n",sdk_type);
	printf("Header Type             0x%08x\n",header_type);
	printf("Metadata offset         0x%08x\n",metadata_offset);
	printf("Header Length           0x%08x%08x (%u Bytes)\n",header_len,header_len>>32,header_len);
	printf("Elf Size                0x%08x%08x (%u Bytes)\n",elf_filesize,elf_filesize>>32,elf_filesize);
	printf("Self Size               0x%08x%08x (%u Bytes)\n",self_filesize,self_filesize>>32,self_filesize);
	printf("Unknown_1               0x%08x%08x\n",unknown1,unknown1>>32);
	printf("Unknown_2               0x%08x%08x\n",unknown2,unknown2>>32);
	printf("Application Info Offset 0x%08x%08x\n",appinfo_offset,appinfo_offset>>32);
	printf("Elf Offset              0x%08x%08x\n",elf_offset,elf_offset>>32);
	printf("Program hdr  Offset     0x%08x%08x\n",phdr_offset,phdr_offset>>32);
	printf("Section hdr  Offset     0x%08x%08x\n",shdr_offset,shdr_offset>>32);
	printf("Section Info Offset     0x%08x%08x\n",section_info_offset,section_info_offset>>32);
	printf("Version Offset          0x%08x%08x\n",sceversion_offset,sceversion_offset>>32);
	printf("Control Info Offset     0x%08x%08x\n",controlinfo_offset,controlinfo_offset>>32);
	printf("Control Info Size       0x%08x%08x (%u Bytes)\n",controlinfo_size,controlinfo_size>>32,controlinfo_size);
	printf("Padding                 0x%08x%08x\n",padding,padding>>32);
	


	printf("Done\n");
}

int main(int argc, char *argv[]){

	if (argc == 2) {
		self = mmap_file(argv[1]);
		printf( "readself          - v0.1  By deroad\n\n");
		readself();
	}else {
		fail("usage: %s file.self"
			,argv[0]);
	}


	return 0;
}
