#include <stdio.h>
#include <openssl/bn.h>
#include <string.h>
int mod_exp(int a, int b, int c);
int mod_2(int base,int exp,int mod);
void mod_big(BIGNUM *base,BIGNUM *exp,BIGNUM *mod,BIGNUM * result);
void run_BN_mod_exp();
void func();
int main()
{
  // run_BN_mod_exp();
    func();
      // printf("1=%i\n",mod_exp(17,45,19));
    return 0;
}
void func()
{
    FILE *f = fopen("/dev/urandom","r");
    unsigned char rand[1000];
    fread(&rand,sizeof(rand),1,f);
    fclose(f);
    BIGNUM * exp = BN_new();
    // BN_rand(exp,502,0,0);
    char * e = "11987468116921369759089427269924567051228555588768215200388884063394963208018362529909408294591283014480463819985388851061128894506610158538018479665477";
    BN_dec2bn(&exp,e);
//   BN_set_word(exp,10);
    char *out = BN_bn2dec(exp);
    BIGNUM * prime = BN_new();
    // BN_generate_prime_ex(prime,506,1,NULL,NULL,NULL);
//BN_generate_prime_ex(prime,8,1,NULL,NULL,NULL);
    char prime1[] = "157711492810705644346696979427237291242607285984688871961810848362774974594227258870895200593862153768045476657423076360112168733063256502176554043971787";
    // char * p = BN_bn2dec(prime1);
    BN_dec2bn(&prime,prime1);
    BIGNUM * base = BN_new();
    int a = 5;
    BN_set_word(base,a);
    BIGNUM * result =BN_new();
    BN_set_word(result,1);
    char *ag = BN_bn2dec(base);
    printf("exp= %s\n",out);
    printf("Prime= %s\n",prime1);
    printf("gen= %s\n",ag);
    mod_big(base,exp,prime,result);
    char * res = BN_bn2dec(result);
    printf("result= %s\n",res);
    char t[] = "1998286638065473057944506344030256054916203227381748916180906390214373930105605405985818224246280726328877245115163209963634633681313092395058312190549";
    BIGNUM * T = BN_new();
    BN_dec2bn(&T,t);
    BIGNUM *at = BN_new();
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BN_mul(at,exp,T,ctx);
    BIGNUM * res2 = BN_new();
    BN_set_word(res2,1);
    ag = BN_bn2dec(base);
    char * out2 = BN_bn2dec(T);
    printf("exp= %s\n",out2);
    printf("gen= %s\n",ag);
    mod_big(base,at,prime,res2);
    BIGNUM * try2 = BN_new();
    BN_set_word(try2,1);
    mod_big(result,T,prime,try2);

    // char * ciph = "U2FsdGVkX19KH9fwH2ibwt8Dn4FN/o05WVAGtddxxEknEjvifZGn3YDk+nZbKgMqyVS5EIWnF9ZjUXxkhkjAxzzHWFhPbXMliw1L2BKpCv7ja3ojdPU1DMGhk8qpwQ8hmr0idS/Ba4Hkc+arScVYtjHEI8TbN3XGQ/SDkGHTWaP9e7ItRB6h2L7tUGLBrxXWKcqHsRFJlJKbXAammZopqIYS8PBcQDr2EMLLF7iW48//iXPA6IQPQD2sqf1jUbQ+CJRyNTlZq6LjvbnU2X0uQ1/rnFkzTfO7iRaakuB4zvHCNnpeg9Sv7LXlmtVUEo29sUbWK+QwQvUgtvJpHjniLZUbnKTkz4TOX2pvJrwOAso8Nc3JC/rc6lfKivNbXk81KWFxP2Prvh0i1X/zFkj+Rxk45N0tj4Dm/ytgBW6ZPr2PCQvqZ5Er9VkwUdCgrUZ8vmhquFZk2m/XZ+Ne53Hc4AdV8hhp1zraPZUQZbd12bDp9KTFRxSvytaSk6XqfNReAsvEVZuk1DboT+ekHdOvxdB6UL8XQ3+oxQ+ForsWuXMJMlNrvw2XySsziJ191rslFB4PB5SK2VDSLIks198Pfw==";
    char * gtp = "36348644293237313633528841988920846087844810492838058592182472463476958947594080443853762648697391670077569103627655329421464693042879024413233199138517";
    BIGNUM *gg = BN_new();
    BIGNUM *res3 = BN_new();
    BN_set_word(res3,1);
    BN_dec2bn(&gg,gtp);
    ag = BN_bn2dec(base);
    char * out3 = BN_bn2dec(exp);
    char * exx = BN_bn2dec(gg);
    char * wprime = BN_bn2dec(prime);
    printf("exp= %s\n",out3);
    printf("gen= %s\n",exx);
    printf("prime= %s\n",wprime);
    mod_big(gg,exp,prime,res3);

    }
void run_BN_mod_exp()
{
    BIGNUM * base = BN_new();
    // char * b_str = "123455678";
    BIGNUM * exp = BN_new();
    BIGNUM * mod = BN_new();
    int a = 5;
    int b = 23;
    int c = 23;
    BN_set_word(base,a);
    BN_set_word(exp,b);
    BN_set_word(mod,c);
    BIGNUM * result =BN_new();
    BN_set_word(result,1);
    mod_big(base,exp,mod,result);
    printf("2=%i\n",mod_2(a,b,c));
}
void mod_big(BIGNUM *base,BIGNUM *exp,BIGNUM *mod,BIGNUM * result)
{
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BIGNUM * zero = BN_new();
    BIGNUM * r_exp = BN_new();
    BIGNUM * r_base = BN_new();
    BN_copy(r_exp,exp);
    BN_copy(r_base,base);

    BN_set_word(zero,0);
    while(BN_cmp(zero,r_exp) != 0)
    {
        if(BN_is_bit_set(r_exp,0))
        {
            BN_mod_mul(result,result,r_base,mod,ctx);
        }
    BN_rshift1(r_exp,r_exp);
    BN_mod_mul(r_base,r_base,r_base,mod,ctx);
    }
    // BN_print_fp(stdout, result);
    char * res = BN_bn2dec(result);
    printf("res= %s\n",res);
}
int mod_exp(int a, int b, int c)
{
    if(b == 0)
        return 1;
    long z = mod_exp(a,b/2,c);
    if(b % 2 ==0)
        return (z * z) % c;
    else
        return a * ((z * z) % c) % c;
}
int mod_2(int base,int exp,int mod)
{
    int tot = 1;
    int num = base;
    while(exp != 0)
    {
        if(exp & 1){
            tot *= num;
            tot = tot % mod;
        }
        // printf("%i\n",tot);
        exp = exp >> 1;
        num = (num * num) % mod;
        // printf("num = %i\n",num);
    }
    return tot;
}
