VITA TOOLS
==========

###Based on fail0verflow tools and modified to be used with the vita.

Authors
-------

	Fail0verflow team
	deroad

CHANGELOG
--------


* 1.07

	Fixed little_endian.h.
	Added pupunpack entries and fixed some wrong things
	Fixed readself.
	
* 1.06

	Added ungpkg to allow to unpack game packages.
	Fixed tools.c to allow to search ps vita keys in the right folder.
	Removed pkg_reader. useless
	
* 1.05b

	Updated readself. again more infos about selfs.
	
* 1.05

	Updated readself. now shows more infos about selfs.
	Updated pupunpack. corrected some ID stuff (a self 0x200 is sprx, same for 0x204. it is a self).
	
* 1.04

	added readself. it show information about the Playstation Vita Signed Executable and Linkable Format.
	
* 1.03

	added security_1 to pupunpack
	
* 1.02

	initial version of unpkg. for now it is NOT able to decrypt pkg. it just extract the main parts.
	deleted some id on the pupunpack
	pkg_reader is still unfinished
	
* 1.01

	pupunpack has been updated to unpack the PUP inside a folder called: NAME_UPDATE.PUP_0
	
* 1.00

	pkg_reader fixed and fully working pupunpack!!
	
* Initial release

	pkg_reader (unfinished and bugged)

