import random
import hashlib
import os

class hash_attack:
    def __init__(self,length,num_test):
        self.length=length
        self.num_test=num_test
    def get_digest(self):
        test_byte = os.urandom(random.randint(100,1000))
        temp_dg = hashlib.sha1(test_byte).hexdigest()[0:self.length//4]
        return temp_dg
    def pre_image_gen(self):
        this_dg = self.get_digest()
        test_dg = self.get_digest()
        count = 0
        while this_dg != test_dg:
            test_dg = self.get_digest()
            count += 1
            # if count % 1000000 == 0:
                # print(count)
        return count
        # print(count,this_dg,test_dg)
    def test_pre_image(self):
        test = []
        for i in range(self.num_test):
            test.append(self.pre_image_gen())
        print("average ={}".format(sum(test)/self.num_test))
        print(test)
    def test_collision(self):
        test = []
        for i in range(self.num_test):
            test.append(self.collision())
        print("average ={}".format(sum(test)/self.num_test))
        print(test)
    def collision(self):
        tried = dict()
        dg = self.get_digest()
        count = 0
        while not dg in tried:
            tried[dg] = dg
            count += 1
            dg = self.get_digest()
        return count
def main():
    test_sizes = [8,12,16,20,24]
    for x in test_sizes:
        ha = hash_attack(x,50)
        print("PRE IMAGE SIZE[{}]\n************Result*********".format(x))
        ha.test_pre_image()
        print("Collision SIZE[{}]\n************Result*********".format(x))
        ha.test_collision()

if __name__ == '__main__':
    main()
