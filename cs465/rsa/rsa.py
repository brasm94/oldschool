from Crypto.Util import number as num
import os
def exd_euc(a,b):
    mod = b
    old_s = 1
    s = 0
    old_t = 0
    t = 1
    rem = 1
    while rem != 0:
        q = int(b/a)
        rem = b - a * q
        (old_s,s) = (s,old_s - q * s)
        (old_t,t) = (t,old_t - q *  t)
        print "b=",b,"a=",a,"rem=",rem,"q=",q,"s=",s,"t=",t
        b = a
        a = rem
        if rem == 1:
            if t < 0:
                print "USING THIS"
                return t + mod
            return t
    return -1
def gcd(a,b):
        if a == 0:
            return b
        if b == 0:
            return a
        if a > b:
            return gcd(a%b,b)
        else:
            return gcd(b%a,a)
def gcd_rem(a,b):
    assert a > b
    return a % b
def mod_exp(base,exp,m):
    tot = 1
    b2 = base
    while exp != 0:
        if exp & 1 == 1:
            tot *= b2
            tot = tot % m
        b2 = b2 * b2
        b2 = b2 % m
        exp = exp >> 1
    return tot
def get_p_q():
    if os.stat("p.txt").st_size == 0:
        p = num.getPrime(512)
        q = num.getPrime(512)
        fp = open("p.txt","w")
        fq = open("q.txt","w")
        fp.write(str(p))
        fq.write(str(q))
    else:
        fp = open("p.txt","r")
        fq = open("q.txt","r")
        p = int(fp.read())
        q = int(fq.read())
    fp.close()
    fq.close()
    return p,q
def decrypt(d,n):
    if os.stat("dec.txt").st_size != 0:
        fdec = open("dec.txt","r")
        dec = int(fdec.read())
        fdec.close()
        print "to Dec= ",dec
        tot = mod_exp(dec,d,n)
        print "decrypted=",tot
        return
    print "nothing to decrypt"
def encrypt(d,n):
    if os.stat("enc.txt").st_size != 0:
        fenc = open("enc.txt","r")
        enc = int(fenc.read())
        fenc.close()
        print "to enc= ",enc
        tot = mod_exp(enc,d,n)
        print "encypted =",tot
        return
    print "nothing to encypted"

if __name__ == "__main__":
    p,q = get_p_q()
    phi = (p-1) * (q-1)
    e = 65537
    d = exd_euc(e,phi)
    print "P= " , p
    print "Q= " , q
    print "PHI= " , phi
    print "n= " , p*q
    print "E= ",e
    print "D= ",d
    if d == -1:
        print "exd_eu Failed\n"
    print "MOD CHECK = ",(e*d)%phi
    encrypt(e,p*q)
    decrypt(d,p*q)
