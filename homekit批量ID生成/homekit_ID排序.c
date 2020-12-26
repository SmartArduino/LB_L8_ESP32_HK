#include <iostream>
#include "string.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include  <assert.h>

typedef struct _stt_dataHandle_typeA{
	
	int num_A;
	int num_B;
	int num_C;
	
	char *setup_id;
	char *setup_code;
	char *setup_salt;
	char *setup_verifier;
	char *setup_payload;
}_stt_dHle_type_A;


int cmpUsrApp_A(const void *a,const void *b)
{
	int res = 0;
	
	_stt_dHle_type_A *aa = NULL,
					 *bb = NULL;
					 
	aa = (_stt_dHle_type_A *)a;
	bb = (_stt_dHle_type_A *)b;
	
	res = aa->num_A - bb->num_A;
	
	if(0 == res)res = aa->num_B - bb->num_B;
	if(0 == res)res = aa->num_C - bb->num_C;
	
//	printf("res cmp:%d.\n", res);
	return res;
}

void func_hkPayloadExtract(void){
	
	_stt_dHle_type_A dataSort[10000] = {0};

    //打开文件 
    FILE * r=fopen("C:\\Users\\Nepenthes\\Desktop\\csvDataHandle.txt","r");
    assert(r!=NULL);
    FILE * w=fopen("C:\\Users\\Nepenthes\\Desktop\\hkPayload.txt","w");
    assert(w!=NULL);
     
    //读写文件 
    char a[32] = {0},
    	 b[32] = {0},
    	 c[64] = {0},
    	 d[936] = {0},
    	 e[64] = {0};
    int num_A = 0,
    	num_B = 0,
    	num_C = 0;
    int i=0;
    int loop = 0;
    
    while(fscanf(r,"\n%[^,],%[^,],%[^,],%[^,],%s", a, b, c, d, e)!=EOF)
    {
    	dataSort[i].setup_id = (char *)calloc(1, sizeof(char) * 32);
    	dataSort[i].setup_code = (char *)calloc(1, sizeof(char) * 32);
    	dataSort[i].setup_salt = (char *)calloc(1, sizeof(char) * 64);
    	dataSort[i].setup_verifier = (char *)calloc(1, sizeof(char) * 936);
    	dataSort[i].setup_payload = (char *)calloc(1, sizeof(char) * 64);
    	
    	sscanf(b, "%d-%d-%d", &dataSort[i].num_A, &dataSort[i].num_B, &dataSort[i].num_C);
		strcpy(dataSort[i].setup_id, a);
		strcpy(dataSort[i].setup_code, b);
		strcpy(dataSort[i].setup_salt, c);
		strcpy(dataSort[i].setup_verifier, d);
		strcpy(dataSort[i].setup_payload, e);
		
		printf("loop %d handle done,\n", i);
		
		i++;
    }  
    
//    printf("data[11] a:%s.\n", dataSort[11].setup_payload);
    
    qsort(dataSort, i, sizeof(_stt_dHle_type_A), cmpUsrApp_A);
    
    for(loop = 0; loop < i; loop ++){
    	
    	printf("%s\n", dataSort[loop].setup_code);
    	fprintf(w, "%s,%s,%s,%s,%s\n", dataSort[loop].setup_id,
									   dataSort[loop].setup_code, 
									   dataSort[loop].setup_salt,
									   dataSort[loop].setup_verifier,
									   dataSort[loop].setup_payload);
    	
    	free(dataSort[i].setup_id);
    	free(dataSort[i].setup_code);
    	free(dataSort[i].setup_salt);
    	free(dataSort[i].setup_verifier);
    	free(dataSort[i].setup_payload);
	}
     
    //关闭文件 
    fclose(r);
    fclose(w);
    system("pause");
}

int main(int argc, char** argv) {
	
//	func_hkPayloadExtract();

	unsigned char a = 4,
				  b = 8,
				  c = 0;
				  
	c = a - b;
				  
	printf("res:%d", c);

    return 0;
}