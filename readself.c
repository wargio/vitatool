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

void elf(u32 offset){
	uint16_t e_type       = le16(self+offset+0x10);			/* object file type */
	uint16_t e_machine    = le16(self+offset+0x12);			/* machine type */
	uint32_t e_version    = le32(self+offset+0x14);			/* object file version */
	uint64_t e_entry      = le64(self+offset+0x18);			/* entry point address */
	uint32_t e_phoff      = le32(self+offset+0x24);			/* program header offset */
	uint32_t e_shoff      = le32(self+offset+0x28);			/* section header offset */
	uint16_t e_flags      = le16(self+offset+0x2c);			/* processor-specific flags */
	uint32_t e_ehsize     = le16(self+offset+0x2e);			/* ELF header size */
	uint16_t e_phentsize  = le16(self+offset+0x30);			/* size of program header entry */
	uint16_t e_phnum      = le16(self+offset+0x34);			/* number of program header entries */
	uint16_t e_shentsize  = le16(self+offset+0x38);			/* size of section header entry */
	uint16_t e_shnum      = le16(self+offset+0x2c);			/* number of section header entries */
	uint16_t e_shstrndx   = le16(self+offset+0x2e);			/* section name string table index */

	printf("--- ELF INFO ---\n");
	printf("Type                    0x%04x [%s]\n",e_type,(e_type==2) ? "EXEC" : "SPRX");
	printf("Machine Type            0x%04x\n",e_machine);
	printf("Version                 0x%08x\n",e_version);
	printf("Entry Point Address     0x%08x%08x\n",(u32)e_entry,(u32)(e_entry>>32));
	printf("Program Header Offset   0x%08x\n",e_phoff);
	printf("Section Header Offset   0x%08x\n",e_shoff);
	printf("Flag                    0x%04x\n",e_flags);
	printf("ELF Header Size         0x%08x (%u Bytes)\n",e_ehsize,e_ehsize);
	printf("Size Progr Hdr Entry    0x%04x (%u Bytes)\n",e_phentsize,e_phentsize);
	printf("Number Hdr Entries      0x%04x (%u)\n",e_phnum,e_phnum);
	printf("Size Sect  Hdr Entry    0x%04x (%u Bytes)\n",e_shentsize,e_shentsize);
	printf("Number Hdr Entries      0x%04x (%u)\n",e_shnum,e_shnum);
	printf("Section String Table    0x%04x\n",e_shstrndx);


}

void app_info(u32 offset){
	u64 authid    = le64(self+offset);       /* auth id     */
	u32 vendor_id = le32(self+offset+0x8);   /* vendor id   */
	u32 self_type = le32(self+offset+0xC);   /* app type    */
	u64 version   = le64(self+offset+0x10);  /* app version */
	u64 padding   = le64(self+offset+0x18);  /* UNKNOWN     */

	printf("--- APP INFO ---\n");	
	printf("Authotiry ID            0x%08x%08x\n",(u32)authid,(u32)(authid>>32));
	printf("Vendor ID               0x%08x\n",vendor_id);
	printf("SELF Type               0x%08x\n",self_type);
	printf("Version                 0x%08x%08x\n",(u32)version,(u32)(version>>32));
	printf("Padding                 0x%08x%08x\n",(u32)padding,(u32)(padding>>32));
	

}

void readself(){
	printf("\nReading...\n");

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
	u32 appinfo_offset      = le32(self+0x38);  /* app info offset */
	u32 elf_offset          = le32(self+0x40);  /* ELF #1 offset */
	u32 phdr_offset         = le32(self+0x48);  /* program header offset */
	u32 shdr_offset         = le32(self+0x50);  /* section header offset */
	u32 section_info_offset = le32(self+0x58);  /* section info offset */
	u32 sceversion_offset   = le32(self+0x60);  /* version offset */
	u32 controlinfo_offset  = le32(self+0x68);  /* control info offset */
	u64 controlinfo_size    = le64(self+0x70);  /* control info size */
	u64 padding             = le64(self+0x78);

	if(magic!=HDR)
		fail("\nERROR! Not a PlayStation Vita Self File (Magic: %08x)",magic);

	if(header_type!=1){
		char* HType;
		if(header_type==2)
			HType = "rvk";
		else if(header_type==3)
			HType = "pkg";
		else if(header_type==4)
			HType = "spp";
		else
			HType = "Unknown";

		fail("\nERROR! Not a PlayStation Vita Self File (Type : %08x)\n"
		       "                                               [%s]",header_type,HType);
	}

	printf("Magic                   0x%08x\n",magic);
	printf("Version                 0x%08x\n",version);
	printf("SDK Type                0x%08x\n",sdk_type);
	printf("Header Type             0x%08x\n",header_type);
	printf("Metadata offset         0x%08x\n",metadata_offset);
	printf("Header Length           0x%08x%08x (%u Bytes)\n",(u32)(header_len),(u32)(header_len>>32),(u32)(header_len>>32));
	printf("Elf Size                0x%08x%08x (%u Bytes)\n",(u32)elf_filesize,(u32)(elf_filesize>>32),(u32)(elf_filesize>>32));
	printf("Self Size               0x%08x%08x (%u Bytes)\n",(u32)self_filesize,(u32)(self_filesize>>32),(u32)(self_filesize>>32));
	printf("Unknown_1               0x%08x%08x\n",(u32)unknown1,(u32)(unknown1>>32));
	printf("Unknown_2               0x%08x%08x\n",(u32)unknown2,(u32)(unknown2>>32));
	printf("Application Info Offset 0x%08x\n",appinfo_offset);
	printf("Elf Offset              0x%08x\n",elf_offset);
	printf("Program hdr  Offset     0x%08x\n",phdr_offset);
	printf("Section hdr  Offset     0x%08x\n",shdr_offset);
	printf("Section Info Offset     0x%08x\n",section_info_offset);
	printf("Version Offset          0x%08x\n",sceversion_offset);
	printf("Control Info Offset     0x%08x\n",controlinfo_offset);
	printf("Control Info Size       0x%08x%08x (%u Bytes)\n",(u32)controlinfo_size,(u32)(controlinfo_size>>32),(u32)(controlinfo_size>>32));
	printf("Padding                 0x%08x%08x\n",(u32)padding,(u32)(padding>>32));
	
	app_info(appinfo_offset);
	elf(elf_offset);

	printf("\nDone\n");
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
