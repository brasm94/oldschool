#include <vector>
#include <cmath>  // for e
#include <math.h> // pow
#include "rand.h"
#include <sstream>
#include "matrix.h"
using namespace std;
class DesNode
{
  private:
    int nodeType; // 0 = output , 1 = hidden, 2 = input 3 = bais;
    // vector<double> weightSelfToChildren;
    vector<DesNode *> children;
    Matrix features;
    Matrix labels;
    vector<bool> used;
    vector<bool> rowUsed;
    int splitOn;
    int output;
  public:
    DesNode(Matrix &features,Matrix &labels)
    {
        this->features = features;
        this->labels = labels;
        this->used = vector<bool>(features.cols(),false);
        this->rowUsed = vector<bool>(features.rows(),false);
    };
    DesNode(Matrix &features,Matrix &labels, vector<bool> usedNew,int index,vector<bool> rowKept)
    {
        this->features = features;
        this->labels = labels;
        this->used = usedNew;
        used[index] = true;
        this->rowUsed = rowKept;
        this->output = -1;
    };
    ~DesNode()
    {};
    int getOut(const vector<double>& feat)
    {
        if(children.size() == 0)
        {
         if(output == -1)
         {
            cout<<"this is bad"<<endl;
         }
        return output;
        }
        if(feat[splitOn]> children.size())
            cout << "lol ---------------" <<endl;
        return children[int(feat[splitOn])]->getOut(feat);//children[features]
    }
    double calcInfoLoss(int colFeat , int numOfOuts)
	{
		int colFeatMax = features.columnMax(colFeat) + 1;//plus one for number 0 , 1, ,2 => should be 3 not 2
		vector<vector<double>  > totalCount;
		vector<double> countForType(colFeatMax,0);
		for(int i = 0 ; i < colFeatMax ; i++)
		{
			totalCount.push_back(vector<double>(numOfOuts,0));
		}
		for(unsigned int i = 0 ; i < labels.rows() ; i++)
		{
			totalCount[features[i][colFeat]][labels[i][0]]++; // finds what type and increments the out for that type
			countForType[features[i][colFeat]]++;
		}
		
		double gain = 0;
		for(int i = 0 ; i < colFeatMax;i++)
		{
			double partInfo = 0;
			for(unsigned int j = 0 ; j < totalCount[i].size(); j++)
			{
				double ratio = totalCount[i][j]/countForType[i];
				if(ratio != 0)
					partInfo += ratio * log2(ratio);
			}
			gain += partInfo * countForType[i] / double(labels.rows()) ;
		}
		gain *= -1;

		return gain;
	}
    int getColToSplit()
    {
        // cout << "im here" <<endl;
        // cout << "feature cols = " << features.cols() <<endl;
        vector<double> infoLoss;
        int count = 0;
        for(unsigned int i = 0 ; i < features.cols();i++)
        {
            if(used[i] == false)
            {
                count++;
                infoLoss.push_back(calcInfoLoss(i,labels.columnMax(0) + 1));
                // cout << "info = " << calcInfoLoss(i,labels.columnMax(0) + 1) <<endl;
            }
        }
        // if(count == 1) // if only one comumn left don't split
            // return -1;
        int index = -1; // -1 if no more column to split on
        double smallest = 10000000; // big number
        for(unsigned int i = 0 ; i < features.cols();i++)
        {
            if(used[i] == false)
            {
                if(smallest > infoLoss[i] && used[i] == false)
                {
                    index = i;
                    smallest = infoLoss[i];
                    // cout << infoLoss[i] << endl;
                }
            }
        }
        return index;
    }
    void printColUsed()
    {
        for(unsigned int i = 0 ; i < used.size();i++)
        {
            cout << i << used[i] << " ";
        }
        cout << endl;
        for(unsigned int row = 0 ; row < features.rows();row++)
        {
            cout << row << " " << rowUsed[row] << endl;
        }
    }
    void printChildren()
    {
        for(unsigned int i = 0 ; i < children.size() ; i++)
        {
            children[i]->printColUsed();
        }
    }
    double getGain()
    {
        int howMany = labels.columnMax(0) + 1;
		vector<double> count(howMany,0);
        int rowsLeft = 0;
		for(unsigned int i = 0; i < labels.rows() ; i++)
		{
			if(rowUsed[i] == false)
            {
			    count[labels[i][0]] +=1;
                rowsLeft++;
            }
		}
		double gain = 0;
		for( unsigned int i = 0 ; i < count.size();i++)
		{
			double ratio = count[i]/rowsLeft;
			// cout << "ratio = " << count[i] << " " << rowsLeft <<endl;
            if(ratio != 0)
			    gain += ratio * log2(ratio);
		}
		gain *= -1; //make it positive
		// cout << "gain = " << gain <<endl;
        return gain;
    }
    bool lookOutSame()
    {
        bool same = true;
        int type = -1;
        vector<int> count((labels.columnMax(0)+1),0) ;
        for(unsigned int i = 0 ; i < labels.rows();i++)
        {
            if(rowUsed[i] == false)
            {
                count[labels[i][0]]++; // get the most commonValue
                if(type == -1)
                    type = labels[i][0];
                if(type != labels[i][0])
                    same = false;
            }
        }
        int tempMax = 0;
        int index = -1;
        for(unsigned int i = 0 ; i < count.size();i++)
        {
            if(count[i] > tempMax)
            {
                index = i;
                tempMax = count[i];
            }
        }
        output = type;
        // cout << "i have been called"<<endl;
        return same;
    }
    void makeTree()
    {
        if(lookOutSame() == true) // no more usefull info in table
            return;
        int split = getColToSplit();
        splitOn = split;
        if(split == -1)
            return;
        // cout << "splitting on col = " << split << endl;
        vector<vector<bool> > splitRowCounter; // a vector of vector each vector represents a vector
        //of the used rows. (AKA don't use them again)
        for(unsigned int i = 0 ; i < features.columnMax(split)+1;i++) // +1 because starts a t zero
        {
            splitRowCounter.push_back(rowUsed);
        }
        for(unsigned int row = 0 ; row < features.rows();row++)
        {
            for(unsigned int type = 0 ; type < splitRowCounter.size();type++)
            {
                if (features[row][split] != type)
                    splitRowCounter[type][row] = true;
            }
        }
        for(unsigned int i = 0 ; i < splitRowCounter.size();i++)
        {
            //splitRowCount = the rows used AKA don't use again
            //used cols used so far
            //split what this node split on.
            children.push_back(new DesNode(features,labels,used,split,splitRowCounter[i]));
        }
        // cout << "the number of children = " << children.size() << endl;
        // printChildren();
        for(unsigned int child = 0; child < children.size();child++)
        {
            children[child]->makeTree();
        }
    }
};


