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
#include "readself.h"

static u8 *self = NULL;

const u32 HDR = 0x53434500;

//64bit


void program_hdr(u64 offset){
	uint32_t p_type    = le32(self+offset);		/* type of segment           */
	offset+=sizeof(uint32_t);
	uint32_t p_flags   = le32(self+offset);		/* segment attributes        */
	offset+=sizeof(uint32_t);
	uint32_t p_offset  = le32(self+offset);		/* offset in file            */
	offset+=sizeof(uint32_t);
	uint32_t p_vaddr   = le32(self+offset);		/* virtual address in memory */
	offset+=sizeof(uint32_t);
	uint32_t p_paddr   = le32(self+offset);		/* reserved                  */
	offset+=sizeof(uint32_t);
	uint32_t p_filesz  = le32(self+offset);		/* size of segment in file   */
	offset+=sizeof(uint32_t);
	uint32_t p_memsz   = le32(self+offset);		/* size of segment in memory */
	offset+=sizeof(uint32_t);
	uint32_t p_align   = le32(self+offset);		/* alignment of segment      */
	offset+=sizeof(uint32_t);

	const char* p_type_char = prg_type(p_type);
	const char* p_flags_char = prg_flags(p_flags);

	printf("--- SELF PROGRAM INFO ---\n");
	printf("Type                    0x%08x [%s]\n",p_type,p_type_char);
	printf("Flags                   0x%08x [%s]\n",p_flags,p_flags_char);
	printf("Offset                  0x%08x\n",p_offset);
	printf("Virtual Address         0x%08x\n",p_vaddr);
	printf("Reserved Adderess       0x%08x\n",p_paddr);
	printf("Real Size               0x%08x (%u Bytes)\n",p_filesz,p_filesz);
	printf("Size in Memory          0x%08x (%u Bytes)\n",p_memsz,p_memsz);
	printf("Alignment               0x%08x (%u)\n",p_align,p_align);
}

void section_hdr(u64 offset){
	uint32_t sh_name      = le32(self+offset);	/* section name */
	offset+=sizeof(uint32_t);
	uint32_t sh_type      = le32(self+offset);	/* section type */
	offset+=sizeof(uint32_t);
	uint32_t sh_flags     = le32(self+offset);	/* section attributes */
	offset+=sizeof(uint32_t);
	uint32_t sh_addr      = le32(self+offset);	/* virtual address in memory */
	offset+=sizeof(uint32_t);
	uint32_t sh_offset    = le32(self+offset);	/* offset in file */
	offset+=sizeof(uint32_t);
	uint32_t sh_size      = le32(self+offset);	/* size of section */
	offset+=sizeof(uint32_t);
	uint32_t sh_link      = le32(self+offset);	/* link to other section */
	offset+=sizeof(uint32_t);
	uint32_t sh_info      = le32(self+offset);	/* miscellaneous information */
	offset+=sizeof(uint32_t);
	uint32_t sh_addralign = le32(self+offset);	/* address alignment boundary */
	offset+=sizeof(uint32_t);
	uint32_t sh_entsize   = le32(self+offset);	/* size of entries, if section has table */
	offset+=sizeof(uint32_t);

	printf("--- SELF PROGRAM INFO ---\n");
	printf("Name                    0x%08x\n",sh_name);
	printf("Type                    0x%08x\n",sh_type);
	printf("Flags                   0x%08x\n",sh_flags);
	printf("Virtual Address         0x%08x\n",sh_addr);
	printf("Offset                  0x%08x (%u Bytes)\n",sh_offset,sh_offset);
	printf("Size                    0x%08x\n",sh_size);
	printf("Link to other section   0x%08x\n",sh_link);
	printf("Info                    0x%08x\n",sh_info);
	printf("Address Alignment       0x%08x\n",sh_addralign);
	printf("Size Entries            0x%08x (%u Bytes)\n",sh_entsize,sh_entsize);
}

void elf(u64 offset){
	uint8_t e_ident[16];
	int i=0;
	for(i=0;i<0x10;i++)
		e_ident[i] = be8(self+offset+i);

	uint16_t e_type       = le16(self+offset+0x10);			/* object file type */
	offset+=0x10+sizeof(uint16_t);

	uint16_t e_machine    = le16(self+offset);			/* machine type */
	offset+=sizeof(uint16_t);

	uint32_t e_version    = le32(self+offset);			/* object file version */
	offset+=sizeof(uint32_t);

	uint32_t e_entry      = le32(self+offset);			/* entry point address */
	offset+=sizeof(uint32_t);

	uint32_t e_phoff      = le32(self+offset);			/* program header offset */
	offset+=sizeof(uint32_t);

	uint32_t e_shoff      = le32(self+offset);			/* section header offset */
	offset+=sizeof(uint32_t);

	uint16_t e_flags      = le16(self+offset);			/* processor-specific flags */
	offset+=sizeof(uint16_t);

	uint32_t e_ehsize     = le16(self+offset);			/* ELF header size */
	offset+=sizeof(uint16_t);

	uint16_t e_phentsize  = le16(self+offset);			/* size of program header entry */
	offset+=sizeof(uint16_t);

	uint16_t e_phnum      = le16(self+offset);			/* number of program header entries */
	offset+=sizeof(uint16_t);

	uint16_t e_shentsize  = le16(self+offset);			/* size of section header entry */
	offset+=sizeof(uint16_t);

	uint16_t e_shnum      = le16(self+offset);			/* number of section header entries */
	offset+=sizeof(uint16_t);

	uint16_t e_shstrndx   = le16(self+offset);			/* section name string table index */
	offset+=sizeof(uint16_t);

	printf("--- ELF INFO ---\n");

	printf("File Class              0x%02x [%s]\n",e_ident[4],(e_ident[4]==1) ? "ELFCLASS64" : "ELFCLASS32");
	printf("File Data               0x%02x [%s]\n",e_ident[5],(e_ident[5]==1) ? "ELFDATA2MSB][BIG ENDIAN" : "ELFDATA2LSB][LITTLE ENDIAN");
	printf("File Version            0x%02x\n",e_ident[6]);

	printf("Type                    0x%04x [%s]\n",e_type,(e_type==2) ? "EXEC" : "PRX");
	printf("Machine Type            0x%04x\n",e_machine);
	printf("Version                 0x%08x\n",e_version);
	printf("Entry Point Address     0x%08x\n",e_entry);
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

void app_info(u64 offset){
	u64 authid    = le64(self+offset);   /* auth id     */
	offset += sizeof(u64);
	u32 vendor_id = le32(self+offset);   /* vendor id   */
	offset += sizeof(u32);
	u32 self_type = le32(self+offset);   /* app type    */
	offset += sizeof(u32);
	u64 version   = le64(self+offset);   /* app version */
	offset += sizeof(u64);
	u64 padding   = le64(self+offset);   /* UNKNOWN     */
	offset += sizeof(u64);

	printf("--- APP INFO ---\n");	
	printf("Authotiry ID            0x%016lx\n",authid);
	printf("Vendor ID               0x%08x\n",vendor_id);
	printf("SELF Type               0x%08x\n",self_type);
	printf("Version                 0x%016lx\n",version);
	printf("Padding                 0x%016lx\n",padding);
	

}

void sce_version(u64 offset){
	uint32_t unknown1 = le32(self+offset);
	offset+=sizeof(uint32_t);
	uint32_t unknown2 = le32(self+offset);
	offset+=sizeof(uint32_t);
	uint32_t unknown3 = le32(self+offset);
	offset+=sizeof(uint32_t);
	uint32_t unknown4 = le32(self+offset);
	offset+=sizeof(uint32_t);

	printf("--- SELF SCE VERSION ---\n");
	printf("Unknown_1               0x%08x\n",unknown1);
	printf("Unknown_2               0x%08x\n",unknown2);
	printf("Unknown_3               0x%08x\n",unknown3);
	printf("Unknown_4               0x%08x\n",unknown4);

}

void control_information(u64 offset){
	uint32_t type    = le32(self+offset);    // 4== ; 6==;7==
	offset+=sizeof(uint32_t);
	uint32_t size    = le32(self+offset);
	offset+=sizeof(uint32_t);
	uint64_t unknown = le64(self+offset); // 0;1
	offset+=sizeof(uint64_t);

	printf("--- CONTROL INFORMATION ---\n");
	printf("Type                    0x%08x\n",type);
	printf("Size                    0x%08x\n",size);
	printf("Unknown                 0x%016lx\n",unknown);

}


void readself(){
	printf("\nReading...\n");
	u32 offset=0;
	u32 magic               = be32(self);         /* 53434500 = SCE\0 */
	offset += sizeof(u32);
	u32 version             = le32(self+offset);  /* header version 3*/
	offset += sizeof(u32);
	u16 sdk_type            = le16(self+offset);  /* SDK type */
	offset += sizeof(u16);
	u16 header_type         = le16(self+offset);  /* 1 self, 2 unknown, 3 pkg */
	offset += sizeof(u16);
	u32 metadata_offset     = le32(self+offset);  /* metadata offset */
	offset += sizeof(u32);
	u64 header_len          = le64(self+offset);  /* self header length */
	offset += sizeof(u64);
	u64 elf_filesize        = le64(self+offset);  /* ELF file length */
	offset += sizeof(u64);
	u64 self_filesize       = le64(self+offset);  /* SELF file length */
	offset += sizeof(u64);
	u64 unknown1            = le64(self+offset);  /* UNKNOWN */
	offset += sizeof(u64);
	u64 self_offset         = le64(self+offset);  /* Self Offset */
	offset += sizeof(u64);
	u64 appinfo_offset      = le64(self+offset);  /* app info offset */
	offset += sizeof(u64);
	u64 elf_offset          = le64(self+offset);  /* ELF #1 offset */
	offset += sizeof(u64);
	u64 phdr_offset         = le64(self+offset);  /* program header offset */
	offset += sizeof(u64);
	u64 shdr_offset         = le64(self+offset);  /* section header offset */
	offset += sizeof(u64);
	u64 section_info_offset = le64(self+offset);  /* section info offset */
	offset += sizeof(u64);
	u64 sceversion_offset   = le64(self+offset);  /* version offset */
	offset += sizeof(u64);
	u64 controlinfo_offset  = le64(self+offset);  /* control info offset */
	offset += sizeof(u64);
	u64 controlinfo_size    = le64(self+offset);  /* control info size */
	offset += sizeof(u64);


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
	printf("Header Length           0x%016lx (%lu Bytes)\n",header_len,header_len);
	printf("Elf Size                0x%016lx (%lu Bytes)\n",elf_filesize,elf_filesize);
	printf("Self Size               0x%016lx (%lu Bytes)\n",self_filesize,self_filesize);
	printf("Unknown_1               0x%016lx\n",unknown1);
	printf("Self Offset             0x%016lx\n",self_offset);
	printf("Application Info Offset 0x%016lx\n",appinfo_offset);
	printf("Elf Offset              0x%016lx\n",elf_offset);
	printf("Program hdr  Offset     0x%016lx\n",phdr_offset);
	printf("Section hdr  Offset     0x%016lx\n",shdr_offset);
	printf("Section Info Offset     0x%016lx\n",section_info_offset);
	printf("Version Offset          0x%016lx\n",sceversion_offset);
	printf("Control Info Offset     0x%016lx\n",controlinfo_offset);
	printf("Control Info Size       0x%016lx (%lu Bytes)\n",controlinfo_size,controlinfo_size);
	
	app_info(appinfo_offset);
	elf(elf_offset);
	program_hdr(phdr_offset);
	section_hdr(shdr_offset);
	sce_version(sceversion_offset);
	control_information(controlinfo_offset);

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
