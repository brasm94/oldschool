#include "sha1.h"
#include <stdio.h>
#include <string.h>
void test_ch()
{
    uint32_t x =    0x010102002;
    uint32_t y =    0x010202011;
    uint32_t z =    0x01030f0ff;
    uint32_t r =    0x000000000;
    uint32_t expe = 0x01020f0fd;
    int pass = 1;
    r = Ch(x,y,z);
    // printf("0x%x 0x%x",r,expe);
    if(r != expe)pass = 0;
    if(pass) printf("%s","\033[32mTest_CH Passed\033[0m\n");else printf("%s","\033[33mTest_CH Failed\033[0m\n");
}
void test_parity()
{
    uint32_t x =    0x010102002;
    uint32_t y =    0x010202011;
    uint32_t z =    0x01030f0ff;
    uint32_t r =    0x000000000;
    uint32_t expe = 0x01000f0ec;
    int pass = 1;
    r = Parity(x,y,z);
    // printf("0x%x 0x%x",r,expe);
    if(r != expe)pass = 0;
    if(pass) printf("%s","\033[32mTest_Parity Passed\033[0m\n");else printf("%s","\033[33mTest_Parity Failed\033[0m\n");
}
void test_maj()
{
    uint32_t x =    0x010102002;
    uint32_t y =    0x010202011;
    uint32_t z =    0x01030f0ff;
    uint32_t r =    0x000000000;
    uint32_t expe = 0x010302013;
    int pass = 1;
    r = Maj(x,y,z);
    // printf("0x%x 0x%x",r,expe);
    if(r != expe)pass = 0;
    if(pass) printf("%s","\033[32mTest_Maj Passed\033[0m\n");else printf("%s","\033[33mTest_Maj Failed\033[0m\n");
}
void test_padding()
{
    uint8_t msg[64] = "";
    uint8_t expe[64] = {0x80};
    int length = strlen((char *)msg);
    padd_msg(msg,&length);
    int pass = 1;
    for(int i = 0 ; i < 64;i++){
        if(msg[i] != expe[i])
        pass = 0;
    }
    if(pass) printf("%s","\033[32mTest_Padding Passed\033[0m\n");else printf("%s","\033[33mTest_Padding Failed\033[0m\n");
    uint8_t msg2[64] = "abc";
    uint8_t expe2[64] = {0x61,0x62,0x63,0x80};
    expe2[63] = 0x18;
    int length2 = strlen((char *)msg2);
    padd_msg(msg2,&length2);
    int pass2 = 1;
    for(int i = 0 ; i < 64;i++){
        if(msg2[i] != expe2[i])
            pass2 = 0;
    }
    if(pass2) printf("%s","\033[32mTest_Padding2 Passed\033[0m\n");else printf("%s","\033[33mTest_Padding2 Failed\033[0m\n");
}
void test_ROTL()
{
    uint32_t word =  0x10000200;
    uint32_t expe =  0x20000400;
    uint32_t word2=  0x80000800;
    uint32_t expe2 = 0x00001001;
    uint32_t res =  ROTL(word,1);
    uint32_t res2 = ROTL(word2,1);
    if(expe == res)
        printf("%s","\033[32mTest_ROTL Passed\033[0m\n");else printf("%s","\033[33mTest_ROTL Failed\033[0m\n");
    if(expe2 == res2)
        printf("%s","\033[32mTest_ROTL2 Passed\033[0m\n");else printf("%s","\033[33mTest_ROTL2 Failed\033[0m\n");
    // printf("%02x %02x",expe,res);
    uint32_t lame = 0x67452301;
    uint32_t result = ROTL(lame,5);
    uint32_t want = 0xe8a4602c;
    if(result == want)
        printf("%s","\033[32mTest_ROTL3 Passed\033[0m\n");else printf("%s","\033[33mTest_ROTL3 Failed\033[0m\n");

    // printf("%02x\n",result);
}
void test_ROTR()
{
    uint32_t word =  0x20000400;
    uint32_t expe =  0x10000200;
    uint32_t res =  ROTR(word,1);
    if(expe == res)
        printf("%s","\033[32mTest_ROTR Passed\033[0m\n");else printf("%s","\033[33mTest_ROTR Failed\033[0m\n");
}
void test_prepare_sch()
{
    uint32_t W[80] = {0};
    uint32_t W2[32] = {
      0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
      0x00000001,0x00000000,0x00000000,0x00000002,0x00000000,0x00000000,0x00000004,0x00000000,0x00000002,0x00000008,0x00000000,0x00000000,0x00000010,0x00000000,0x0000000a,0x00000020
    };
    uint32_t expe2[16] = {0};
    expe2[0] =  0x80000000;
    expe2[15] = 0x00000000;
    prepare_sch(W,expe2,0);
    int pass =1;
    for(int i = 0 ; i < 32 ;i++)
    {
      if(W2[i] != W[i])
        pass =0;
    }
    if(pass)printf("%s","\033[32mTest_Prepare_sch Passed\033[0m\n");else printf("%s","\033[33mTest_Prepare_sch Failed\033[0m\n");
  }































// lol
