#include "sfo_stuff.h"

u32 sfo_header = 0x46535000; //Big endian


void analizeSFO(u8 *pkg, u32 sfo_offset, u32 sfo_size){
	u32 sfo_vals  = le32(pkg+sfo_offset+0x08);
	u32 sfo_param = le32(pkg+sfo_offset+0x0c);

	unsigned int sfo_val_ptr,sfo_val_size,sfo_param_ptr,sfo_param_size;
	char value[0x20];
	char param[0x200];
	u64 param32;
	unsigned int i,k;


	if(dbg()==1){
		printf("[SFO HDR]     0x%08x\n", (unsigned int)le32(pkg+sfo_offset));
		printf("[SFO Offset]  0x%08x\n", sfo_offset);
		printf("[SFO Size  ]  0x%08x\n", sfo_size);
		printf("[SFO Version] 0x%08x\n", (unsigned int)le32(pkg+sfo_offset+0x4));
	}
	printf("[SFO]\n");
	for(i=0x0;i<sfo_vals-0x40;i+=0x10){
		sfo_val_ptr	= le32(pkg+(sfo_offset+0x20+i))+sfo_offset+sfo_vals;
		sfo_val_size	= be8 (pkg+(sfo_offset+0x24+i))-be8(pkg+(sfo_offset+0x14+i));
		sfo_param_size	= le32(pkg+(sfo_offset+0x1c+i));
		sfo_param_ptr	= le32(pkg+(sfo_offset+0x20+i))+sfo_offset+sfo_param;
		for(k=0;k<(unsigned int)sfo_val_size;k++)
			value[k] = be8(pkg+(sfo_val_ptr+k));

		printf("[ + ] %12s", value);
		if(sfo_param_size==SFO_0x4){
			param32 = le32(pkg+sfo_param_ptr);
			printf("	| Param: 0x%x\n",(unsigned int)param32);
		}
		else if(sfo_param_size==SFO_0x8){
			for(k=0x0;k<0x8;k+=0x1)
				param[k] = be8(pkg+sfo_param_ptr+k);
			printf("	| Param: %s\n", param);
		}
		else if(sfo_param_size==SFO_0x12){
			for(k=0x0;k<0x12;k+=0x1)
				param[k] = be8(pkg+sfo_param_ptr+k);
			printf("	| Param: %s\n", param);
		}
		else if(sfo_param_size==SFO_0x16){
			for(k=0x0;k<0x16;k+=0x1)
				param[k] = be8(pkg+sfo_param_ptr+k);
			printf("	| Param: %s\n", param);
		}
		else if(sfo_param_size==SFO_0x30){
			for(k=0x0;k<0x30;k+=0x1)
				param[k] = be8(pkg+sfo_param_ptr+k);
			printf("	| Param: %s\n", param);
		}
		else if(sfo_param_size>=SFO_0x32){
			for(k=0x0;k<sfo_param_size;k++)
				param[k] = be8(pkg+sfo_param_ptr+k);
			printf(" 	| Param: %s\n", param);
		}else{}

		for(k=0x0;k<0x20;k++)
			value[k] = '\0';
	}
}
