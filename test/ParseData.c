#ifndef __PARSEDATA_C__
#define __PARSEDATA_C__ 

#include "ParseData.h"

extern FILE * fileptr;

uint32_t getNextCluster (uint32_t cluster) {
	uint8_t str[5]; //storage string from FAT region
	uint8_t str_hex[5]; // storage string after converting str->hex
	uint8_t i = 0; //count variable
	uint32_t result=0;
	uint8_t a = cluster%2;
	fseek(fileptr,NextIndexCluster_FAT(cluster),SEEK_SET);
	//Get 2 chars from floppy disk
	for(i=0;i<2;i++)
	{
		str[i]=getc(fileptr);
	}
	//Handle 4 bit in each char
	str_hex[0]=((str[0]>>4)&0x0F);
	str_hex[1]=(str[0]&0x0F);
	str_hex[2]=((str[1] >>4) &0x0F);
	str_hex[3]=(str[1]&0x0F);
	// 
	if(a==1)
	{
		result=(str_hex[2]<<8)+(str_hex[3]<<4)+str_hex[0];
	}
	else
	{
		result=(str_hex[3]<<8)+(str_hex[0]<<4)+str_hex[1];
	}
	return result;
}

uint32_t NextIndexCluster_FAT (uint32_t cluster) {
	return (cluster * 3 / 2) + 0x200;
}

uint32_t NextIndexDataofCluster (uint32_t cluster) {
	
	return (cluster + 0x1F) * 0x200;     // 1F = 33 block - 2 block = 31
}

#endif   // __PARSEDATA_C__ 