#include <vector>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>


using namespace std;
void printVec(vector<int> vec)
{
    for(int i = 0 ; i < vec.size();i++)
        cout << vec[i] << " ";
    cout << endl;
}
long candies(vector<int> ratings)
{
    vector<int> amount(ratings.size(),1);
    for(int i = 1;i<ratings.size();i++)
    {
        if(ratings[i - 1] < ratings[i])
            amount[i] = amount[i-1] + 1;
    }
    printVec(ratings);
    printVec(amount);
    long sum = 0;
    for(int i = ratings.size()-1; i >= 1;i--)
    {
        if(ratings[i] < ratings[i - 1] && amount[i-1] <= amount[i])
            amount[i-1] = amount[i] + 1;
        sum += amount[i];
    }
    printVec(amount);
    return sum + amount[0];
}
int main(int argc, char *argv[])
{
    // AbCdE
    // AFE
    vector<int> ratings = {2,4,2,6,1,7,8,9,2,1};
    
	long sum = candies(ratings);
	printf("\nworking on this %li\n",sum);
	return 0;
}


//Theres is first