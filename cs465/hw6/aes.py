from Crypto.Cipher import AES

file = open("file.txt")

enc_obj = AES.new("this is a key123",AES.MODE_CBC,"This is an IV456")
msg = file.read()
if len(msg) % 16 != 0:
    pad = len(msg) % 16
pad_num = 16 - len(msg) % 16
msg += "_" * pad_num
cipher_text = enc_obj.encrypt(msg)
print(cipher_text)
dec = enc_obj.decrypt(cipher_text)
print(dec)
