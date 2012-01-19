#include "sfo_stuff.h"
u32 sfo_header = 0x46535000; //Big endian


void analizeSFO(u8 *pkg, u32 sfo_offset, u32 sfo_size){
	u32 sfo_vals	 = le32(pkg+sfo_offset+0x08);
	u32 sfo_param	 = le32(pkg+sfo_offset+0x0c);
	u32 sfo_type;
	u32 sfo_n_params = le32(pkg+sfo_offset+0x10);
	u32 sfo_val_ptr,sfo_val_size,sfo_param_ptr,sfo_param_size;
	u64 param32;
	unsigned int i,k;
	printf("\n");

	if(dbg()==1){
		printf("[SFO HDR]     0x%08x\n", (unsigned int)le32(pkg+sfo_offset));
		printf("[SFO Offset]  0x%08x\n", sfo_offset);
		printf("[SFO Size  ]  0x%08x\n", sfo_size);
		printf("[SFO Version] 0x%08x\n", (unsigned int)le32(pkg+sfo_offset+0x4));
		printf("[SFO N]	      %u Value(s)\n", sfo_n_params-0x1);
		printf("[SFO Values]  0x%08x\n", sfo_vals);
		printf("[SFO Params]  0x%08x\n", sfo_param);
	}
	printf("[ SFO ]\n");
	for(i=0x0;i<sfo_n_params-0x1;i+=0x1){
		char value[0x20];
		char param[0x200];
		sfo_val_size		= le8 (pkg+(sfo_offset+0x24+(i*0x10)))-le8(pkg+(sfo_offset+0x14+(i*0x10)));
		sfo_val_ptr		= sfo_offset+sfo_vals+le16(pkg+(sfo_offset+0x14+(i*0x10)));
		sfo_param_size		= le32(pkg+(sfo_offset+0x1c+(i*0x10)));
		sfo_param_ptr		= le32(pkg+(sfo_offset+0x20+(i*0x10)))+sfo_offset+sfo_param;
		sfo_type 		= le16(pkg+(sfo_offset+0x16+(i*0x10)));

		for(k=0;k<(unsigned int)(u32)sfo_val_size;k++)
			value[k] = be8(pkg+(sfo_val_ptr+k));

		printf("[ %3i ] %20s",i+1, value);
		if(sfo_param_size==0x4){
			if(sfo_type!=UTF8){
				param32 = le32(pkg+sfo_param_ptr);
				printf("	| Param: 0x%x\n",(unsigned int)param32);
			}else{
				for(k=0x0;k<sfo_param_size;k+=0x1)
					param[k] = le8(pkg+sfo_param_ptr+k);
				printf("	| Param: %s\n", param);
			}
		}
		else if(sfo_param_size>0x4&&sfo_param_size<=0x8){
			if(sfo_type!=UTF8){
				param32 = be32(pkg+sfo_param_ptr);
				printf("	| Param: 0x%x\n",(unsigned int)param32);
			}else{
				for(k=0x0;k<sfo_param_size;k+=0x1)
					param[k] = le8(pkg+sfo_param_ptr+k);
				printf("	| Param: %s\n", param);
			}
		}
		else if(sfo_param_size>0x8&&sfo_param_size<=0x16){
			if(sfo_type!=UTF8){
				param32 = be64(pkg+sfo_param_ptr);
				printf("	| Param: 0x%x\n",(unsigned int)param32);
			}else{
				for(k=0x0;k<sfo_param_size;k+=0x1)
					param[k] = le8(pkg+sfo_param_ptr+k);
				printf("	| Param: %s\n", param);
			}
		}
		else if(sfo_param_size>0x16&&sfo_param_size<=SFO_0x30){
			if(sfo_type!=UTF8){
				param32 = be64(pkg+sfo_param_ptr);
				printf("	| Param: 0x%x\n",(unsigned int)param32);
			}else{
				for(k=0x0;k<0x30;k+=0x1)
					param[k] = le8(pkg+sfo_param_ptr+k);
				printf("	| Param: %s\n", param);
			}
		}
		else if(sfo_param_size>=SFO_0x32){
			if(sfo_type!=UTF8){
				param32 = be64(pkg+sfo_param_ptr);
				printf("	| Param: 0x%x\n",(unsigned int)param32);
			}else{
				for(k=0x0;k<sfo_param_size;k++)
				param[k] = le8(pkg+sfo_param_ptr+k);
				printf("	| Param: %s\n", param);
			}
		}else{}
	}
}
