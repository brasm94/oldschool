#include "sha1.h"
#include "test.c"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #include <openssl/sha.h>
#include <stdlib.h>
int main()
{
    // test_ch();
    // test_parity();
    // test_maj();
    // test_padding();
    test_ROTL();
    // test_ROTR();
    // test_prepare_sch();
    uint8_t msg[60000] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    uint32_t output[5];
    int length = strlen((char *)msg);
    padd_msg(msg,&length);
    changeOrder(msg,length);
    uint32_t * lol = (uint32_t*)msg;
    algorithim(lol,length / 4,output);
    for(int i = 0 ; i < 5;i++)
      printf("%08x",output[i]);
    printf("\n");  
    return 0;
}
void changeOrder(uint8_t msg[],int length)
{
  for(int i = 0; i < length /4 ;i++)
  {
    uint8_t temp = msg[i*4+0];
    uint8_t temp2 = msg[i*4+1];
    msg[i*4+0] = msg[i*4+3];
    msg[i*4+1] = msg[i*4+2];
    msg[i*4+2] = temp2;
    msg[i*4+3] = temp;
  }
}
void algorithim(uint32_t msg[],int length,uint32_t out[])
{
    uint32_t constants[4] = {0x5a827999,0x6ed9eba1,0x8f1bbcdc,0xca62c1d6};
    uint32_t H[5] = {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
    uint32_t W[80] = {0};
    uint32_t a;uint32_t b; uint32_t c; uint32_t d; uint32_t e;
    uint32_t temp_word;
    // printf("%08x\n",H[0]);
    // printf("Length %i\n",length);
    int blocks = length / 16;
    printf("Blocks %i\n",blocks);
    for(int i = 0; i < blocks ;i ++)
    {
        prepare_sch(W,msg,i);
        a = H[0]; b = H[1] ; c = H[2]; d = H[3]; e = H[4];
        // printf("%02x,%02x,%02x,%02x,%02x\n",a,b,c,d,e);
        // printf("%u\n",ROTL(a,5));
        // printf("%08x\n",func(b,c,d,0));
        for(int t = 0 ; t < 80 ;t++)
        {
            temp_word = ROTL(a,5) + func(b,c,d,t) + e + constants[t/20]+ W[t];//W[0];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = temp_word;
            // printf("%08x,%08x,%08x,%08x,%08x\n",a,b,c,d,e);
            // printf("%02u,%02u,%02u,%02u,%02u\n",a,b,c,d,e);
        }
        H[0] = H[0] + a;
        H[1] = H[1] + b;
        H[2] = H[2] + c;
        H[3] = H[3] + d;
        H[4] = H[4] + e;
        // printf("%02x,%02x,%02x,%02x,%02x\n",H[0],H[1],H[2],H[3],H[4]);
    }
    for(int i = 0 ; i < 5;i++)
        out[i] = H[i];
}
void prepare_sch(uint32_t W[80],uint32_t msg[16],int block)
{

    for(int i = 0 ; i < 80 ;i++)
    {
        if(i < 16){
            W[i] = msg[i + block*16];
        }
        else
        {
            W[i] = ROTL(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16],1); // need to add ROTL
        }
    }
    // for(int i = 0 ; i < 80 ;i++)
    // {
    //   printf("[%02i]%08x\n",i,W[i]);
    // }
}
uint32_t Ch(uint32_t x,uint32_t y,uint32_t z)
{
    uint32_t result;
    uint32_t xy_and;
    uint32_t nxz;
    xy_and = x & y;
    nxz = ~x & z;
    result = xy_and ^ nxz;
    return result;
}
uint32_t func(uint32_t a , uint32_t b, uint32_t c, int t)
{
    if(t < 20){
        return Ch(a,b,c);
      }
    if(t < 40)
        return Parity(a,b,c);
    if(t < 60)
        return Maj(a,b,c);
    return Parity(a,b,c);
}
uint32_t Parity(uint32_t x,uint32_t y,uint32_t z)
{
    uint32_t result;
    result = x ^ y ^ z;
    return result;
}
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z)
{
    uint32_t result;
    uint32_t xy_and;
    uint32_t xz_and;
    uint32_t yz_and;
    xy_and = x & y;
    xz_and = x & z;
    yz_and = y & z;
    result = xy_and ^ xz_and ^ yz_and;
    return result;
}
void padd_msg(uint8_t msg[],int * length)
{

    uint64_t num_bits = *length * 8;
    uint64_t padding_zeros = 448 - (num_bits + 1)%512;
    // printf("number of bits = %li padding %li\n",num_bits,padding_zeros);
    int pad_80 = padding_zeros / 8;
    msg[*length] = 0x80;
    for(int i = 0 ; i < pad_80 ;i++)
        msg[*length+i+1] = 0x00;
    *length = *length + pad_80 + 1;
    for(int i = 0 ; i < 8;i ++)
        msg[*length + i] = num_bits >> (7-i)*8;
    *length += 8;
}
uint32_t ROTL(uint32_t word,int n)
{
    // uint32_t temp = word << n;
    // uint32_t temp2 = word >> (32-n);
    // uint32_t word2 = temp | temp2;
    // printf("%08x %08x %08x\n",temp,temp2,word2);
    // return word2;
    return (word << n) | word >> (32 -n);
}
uint32_t ROTR(uint32_t word,int n)
{
    return (word >> n) | (word << (32 -n));
}
