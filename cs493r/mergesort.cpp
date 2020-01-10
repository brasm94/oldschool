#include <vector>
#include <stdio.h>


using namespace std;

vector<int> merge(vector<int> la,vector<int> lb)
{
	int i = 0;
	int j = 0;
	vector<int> c;
	while(i < la.size() && j < lb.size())
	{
		printf("%i %i \n",la[i],lb[j]);
		if(la[i] < lb[j])
		{
			c.push_back(la[i]);
			i++;
		}
		else if(la[i] > lb[j])
		{
			c.push_back(lb[j]);
			j++;
		}
		else
		{
			c.push_back(la[i]);
			c.push_back(la[i]);
			i++;
			j++;
		}
	}
	while(i < la.size()){c.push_back(la[i]);i++;}
	while(j < lb.size()){c.push_back(lb[j]);j++;}
	return c;
}
vector<int> mergesort(vector<int> list)
{
	if (list.size() <= 1)
		return list;

	vector<int> left;
	vector<int> right;
	for(int i = 0 ; i < list.size();i++)
	{
		if(i < (list.size()/2))
			left.push_back(list[i]);
		else
			right.push_back(list[i]);
	}
	left = mergesort(left);
	right = mergesort(right);
	return merge(left,right);
}

int main(int argc, char *argv[])
{
	vector<int> lame = {2,1,3,5,3,100,99,87};
	vector<int> ordered = mergesort(lame);
	for(auto i:ordered)
		printf("%i,",i);
	return 0;
}
