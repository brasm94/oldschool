#include <stdint.h>

uint32_t Ch(uint32_t x,uint32_t y,uint32_t z);
uint32_t Parity(uint32_t x,uint32_t y,uint32_t z);
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z);
void padd_msg(uint8_t msg[],int * length);
void algorithim(uint32_t msg[],int length,uint32_t out[]);
void prepare_sch(uint32_t W[80],uint32_t msg[],int block);
uint32_t ROTL(uint32_t word,int n);
uint32_t ROTR(uint32_t word,int n);
uint32_t func(uint32_t a , uint32_t b, uint32_t c, int t);
void changeOrder(uint8_t msg[],int length);
