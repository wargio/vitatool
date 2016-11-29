// Copyright 2010-2011 Sven Peter <svenpeter@gmail.com>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

// Thanks to Mathieulh for his C# retail unpacker
//  (http://twitter.com/#!/Mathieulh/status/23070344881381376)
// Thanks to Matt_P for his python debug unpacker
//  (https://github.com/HACKERCHANNEL/PS3Py/blob/master/pkg.py)
// Thanks to deroad for his modification to port this on the vita


#include "tools.h"
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef WIN32
#define MKDIR(x,y) mkdir(x)
#else
#define MKDIR(x,y) mkdir(x,y)
#endif


static u8 *pkg = NULL;
static u64 size;
static u64 offset;



static void decrypt_retail_pkg(void)
{
	u8 key[0x10];
	u8 iv[0x10];

	if (be16(pkg + 0x06) != 2)
		fail("invalid pkg type: %x", be16(pkg + 0x06));

	u32 type = be32(pkg + 0xE4) & 0xFF;
	if(type == 1) {
		if (key_get_simple("gpkg-key-psp", key, 0x10) < 0)
			fail("failed to load the psp package key.");
	} else if(type == 2) {
		if (key_get_simple("gpkg-key-2", key, 0x10) < 0)
			fail("failed to load the package key.");
	} else if(type == 3) {
		if (key_get_simple("gpkg-key-3", key, 0x10) < 0)
			fail("failed to load the package key.");
	} else if(type == 4) {
		if (key_get_simple("gpkg-key-4", key, 0x10) < 0)
			fail("failed to load the package key.");
	} else {
		printf("Key Type: %d\n", type);
		fail("failed to find the package key.");
	}

	memcpy(iv, pkg + 0x70, 0x10);
	aes128ctr(key, iv, pkg + offset, size, pkg + offset);
}

static void decrypt_debug_pkg(void)
{
	u8 key[0x40];
	u8 bfr[0x1c];
	u64 i;

	memset(key, 0, sizeof key);
	memcpy(key, pkg + 0x60, 8);
	memcpy(key + 0x08, pkg + 0x60, 8);
	memcpy(key + 0x10, pkg + 0x60 + 0x08, 8);
	memcpy(key + 0x18, pkg + 0x60 + 0x08, 8);

	sha1(key, sizeof key, bfr);

	for (i = 0; i < size; i++) {
		if (i != 0 && (i % 16) == 0) {
			wbe64(key + 0x38, be64(key + 0x38) + 1);	
			sha1(key, sizeof key, bfr);
		}
		pkg[offset + i] ^= bfr[i & 0xf];
	}
}

static void unpack_pkg(void)
{

	char contentid[0x30];
	u8 k_licensee[0x10];

	u32 j;
	for(j=0x0;j<0x30;j++)
		contentid[j]  = be8(pkg+0x30+j);
	for(j=0x0;j<0x10;j++)
		k_licensee[j] = be8(pkg+0x70+j);

	printf("Content ID  %s\n",contentid);
	printf("K Licensee  ");
	for(j=0x0;j<0x10;j++)
		printf("%x",k_licensee[j]);	
	printf("\n");

	u64 i;
	u64 n_files;
	u32 fname_len;
	u32 fname_off;
	u64 file_offset;
	u32 flags;
	char fname[256];
	u8 *tmp;

	n_files = be32(pkg + 0x14);
	for (i = 0; i < n_files; i++) {
		tmp = pkg + offset + i*0x20;

		fname_off = be32(tmp) + offset;
		fname_len = be32(tmp + 0x04);
		file_offset = be64(tmp + 0x08) + offset;
		size = be64(tmp + 0x10);
		flags = be32(tmp + 0x18);

		if (fname_len >= sizeof fname)
			fail("filename too long: %s", pkg + fname_off);

		memset(fname, 0, sizeof fname);
		strncpy(fname, (char *)(pkg + fname_off), fname_len);

		flags &= 0xff;
		if (flags == 4 || flags == 18) {
			printf(" %02x : %s\n", flags, fname);
			MKDIR(fname, 0777);
		} else if (flags == 0 || flags == 1 || flags == 3 || flags == 14 ||
				   flags == 15 || flags == 16 || flags == 17 || flags == 19 ||
				   flags == 21 || flags == 22) {
			printf(" %02x : %s (%u bytes)\n", flags, fname, size);
			memcpy_to_file(fname, pkg + file_offset, size);
		} else
			fail("unknown flag %d for %s", flags, pkg + fname_off);
	}
}

int main(int argc, char *argv[])
{
	char *dir;

	int beta=0;

	if (argc != 2 && argc != 3)
		fail("usage: ungpkg filename.pkg\n"
		     " This is just for PS Vita Game Packages. not PS Vita System Packages.\n");

	if (argc == 2) {
		pkg = mmap_file(argv[1]);
		dir = malloc(0x31);
		memset(dir, 0, 0x31);
		memset(dir, 0, 0x30);
		memcpy(dir, pkg + 0x30, 0x30);
	} else {
		pkg = mmap_file(argv[2]);
		dir = malloc(0x31);
		memset(dir, 0, 0x31);
		memset(dir, 0, 0x30);
		memcpy(dir, pkg + 0x30, 0x30);
	}

	MKDIR(dir, 0777);

	if (chdir(dir) != 0)
		fail("chdir(%s)", dir);

	offset = be64(pkg + 0x20);
	size = be64(pkg + 0x28);

	if(be16(pkg + 0x04) & 0x8000)
		decrypt_retail_pkg();
	else
		decrypt_debug_pkg();

	unpack_pkg();

	return 0;
}
