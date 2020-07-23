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
	
	char setup_code[16];
	char setup_payload[32];
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

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void func_hkPayloadExtract(void){
	
	_stt_dHle_type_A dataSort[10000] = {0};

    //打开文件 
    FILE * r=fopen("C:\\Users\\Administrator\\Desktop\\csvDataHandle.txt","r");
    assert(r!=NULL);
    FILE * w=fopen("C:\\Users\\Administrator\\Desktop\\hkPayload.txt","w");
    assert(w!=NULL);
     
    //读写文件 
    char a[64] = {0},
    	 b[64] = {0};
    int num_A = 0,
    	num_B = 0,
    	num_C = 0;
    int i=0;
    int loop = 0;
    
    while(fscanf(r,"%*[^,],%[^,],%*[^,],%*[^,],%s", a, b)!=EOF)
    {
    	sscanf(a, "%d-%d-%d", &dataSort[i].num_A, &dataSort[i].num_B, &dataSort[i].num_C);
		strcpy(dataSort[i].setup_code, a);
		strcpy(dataSort[i].setup_payload, b);
		
		printf("loop %d handle done,\n", i);
		
		i++;
    }  
    
//    printf("data[11] a:%s.\n", dataSort[11].setup_payload);
    
    qsort(dataSort, i, sizeof(_stt_dHle_type_A), cmpUsrApp_A);
    
    for(loop = 0; loop < i; loop ++){
    	
    	printf("%s\n", dataSort[loop].setup_code);
    	fprintf(w, "%s,%s\n", dataSort[loop].setup_code, dataSort[loop].setup_payload);
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