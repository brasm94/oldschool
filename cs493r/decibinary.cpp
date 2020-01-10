#include <vector>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>


using namespace std;
long getval(int val)
{
    // printf("Start %i\n",val);
    long sum = 0;
    int base = 1;
    while(val > 0)
    {
        int end = val % 10;
        val = val / 10;
        sum += base * end;
        base *= 2;
    }
    return sum;
}
void decibinary(long x)
{
    long count = 1;
    long number = 0;
    long total = 1;
    bool change = false;
    long nbase = 4;
    while(x > total)
    {
        printf("Count == %i %i %i\n",count,number,total);
        total += count;
        number++;
        if(change)
        {
            change = false;
            if(nbase == number)
            {
                count += 2;
                nbase *= 2;
            }
            else
                count += 1;
        }
        else
            change = true;
    }
    printf("Number %li\n",number-1);
}
int main(int argc, char *argv[])
{
	decibinary(20);
	return 0;
}


//Theres is first