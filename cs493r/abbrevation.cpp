#include <vector>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>


using namespace std;
void printString(vector<vector<int>> edit,string a , string b)
{
    cout << "- - ";
    for(int i = 0 ; i < b.size();i++)
        cout << b[i] << " ";
    cout << endl;
    for(int i = 0 ; i < edit.size();i++)
    {
        if(i==0)
            cout << "- ";
        else
            cout << a[i-1] << " ";
        for(int j = 0 ; j < edit[0].size();j++)
        {
            cout << edit[i][j] << " ";
        }
        cout << endl;
    }
}
string abbreviation(string a, string b)
{
vector<vector<int>> edit;
    for(int i = 0 ; i < a.size()+1;i++)
        edit.push_back(vector<int>(b.size()+1,0));
    // bool deleted
    edit[0][0] = 1;
    for(int row = 0 ; row < a.size();row++)
    {
        if(islower(a[row]))
            edit[row+1][0] = 1;
        else
            break;
    }
    for(int row = 1 ; row <= a.size();row++)
    {
        int al = row-1;
        for(int col = 1 ; col <= b.size();col++)
        {
            int bl = col-1;
            if(a[al] == b[bl])
                edit[row][col] = edit[row-1][col-1];
            else if(isupper(a[al])) // Cant fix to be changed :(
                edit[row][col] = 0;
            else if(toupper(a[al]) == b[bl]) // see if making upper case makes it match
                edit[row][col] = edit[row-1][col-1] || edit[row-1][col];
            else
                edit[row][col] = edit[row-1][col];
        }
    }
    printString(edit,a,b);
    if(edit[a.size()][b.size()] == 1)
    {
        printf("%i",edit[a.size()][b.size()]);
        return "YES";
    }
    return "NO";
    // return "bob";
}
int main(int argc, char *argv[])
{
    // AbCdE
    // AFE
	string output = abbreviation("abCdd","ABC");
	printf("\nworking on this %s\n",output.c_str());
	return 0;
}


//Theres is first