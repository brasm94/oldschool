#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define byte 8
void sha_1(unsigned char msg[],unsigned char hash[],int n);
void test_pre_image();
int compare(unsigned char msg1[],unsigned char msg2[]);
void incr_str(unsigned char * msg);
void randstring(size_t length,unsigned char * randomString);
void print_bits(unsigned int num,int bits)
{
    // unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1 << (bits);
    for(int i = 0;i<bits;++i){
            printf("%u ",num&maxPow ? 1 : 0);
            num = num<<1;
    }
    printf("\n");
}
int main(){
    printf("Running the program now\n\n");
    int size = 2;
    unsigned char num[20] =  "off";
    unsigned char num2[20] =  "ba";
    unsigned char num3[20]; //malloc(4*sizeof(unsigned char));
    unsigned char num4[20]; //malloc(4*sizeof(unsigned char));
    sha_1(num,num3,size);
    sha_1(num2,num4,size);
    printf("1%s\n2%s\n",num3,num4);
    while(compare(num3,num4) == -1)
    {
        randstring(5,num2);
        sha_1(num2,num4,size);
        // printf("1%s=2%s\n",num3,num4);
    }
    printf("found solution[%s]\n[%s]\n",num,num2);
    print_bits((unsigned int)num3[0],size*4);
    print_bits((unsigned int)num4[0],size*4);
    return 0;
}
void sha_1(unsigned char * msg,unsigned char * hash,int n)
{
    unsigned char obuf[20];
    SHA1(msg,strlen((char*)msg),  obuf);
    for(int i = 0 ; i < n; i++)
        hash[i] = obuf[i];
}
void incr_str(unsigned char * msg){
    for(int i = 0; msg[i] != 0; i++){
        if(msg[i] == 255){
            msg[i] = 'a';
            if(msg[i+1] == 0){
                msg[i+1] += 1;
                msg[i+2] = 0;
            }
            else
                msg[i+1]++;
        }
        
        // printf("%s","got to msg[i] = 0\n");
        msg[i]++;
        msg[i+1] = 'a';
    }
}
int compare(unsigned char msg1[],unsigned char msg2[])
{
    for(int i = 0 ; msg1[i] != 0;i++)
    {
        if(msg1[i] != msg2[i])
            return -1;
    }
    return 0;
}
void randstring(size_t length,unsigned char * randomString) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    // char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    // return randomString;
}