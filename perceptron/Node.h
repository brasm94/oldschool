#include <vector>
#include <cmath>  // for e
#include <math.h> // pow
#include "rand.h"
#include <sstream>
using namespace std;
const int INPUT = 2;
const int HIDDEN = 1;
const int OUTPUT = 0;
const int BAIS = 3;
const double learningRate = .25;
const double momCon = .9;
class Node
{
  private:
    int nodeType; // 0 = output , 1 = hidden, 2 = input 3 = bais;
    // vector<double> weightSelfToChildren;
    vector<Node *> parents;
    vector<double> weightFromParents;
    double value;
    double net;
    double output;
    double target;
    double error;
    double errorCom;
    vector<double> lastWeight;

  public:
    Node()
    {
        error = 0;
    };
    Node(int nodeType)
    {
        this->nodeType = nodeType;
        error = 0;
    };
    Node(vector<Node *> myParents, int nodeType, Rand &r)
    {
        error = 0;
        this->nodeType = nodeType;
        this->parents = myParents;
        for (unsigned int i = 0; i < myParents.size(); i++)
        {
            // cout << r.normal() << " = rand" << endl;
            weightFromParents.push_back(r.normal()); //1);//
            lastWeight.push_back(0);
        }
    };
    ~Node(){};
    double calcSig(double net)
    {
        // cout << "i am the calcsig = " << (1 / ( 1+ pow(M_E,(net * -1)))) << endl;
        if(nodeType == BAIS)
            cout << "done goofed"<<endl;
        return (1 / (1 + pow(M_E, (net * -1))));
    }
    void setValue(double value)
    {
        error = 0;
        this->value = value;
    }
    double addParents(vector<Node *> parents)
    {
        this->parents = parents;
        return 0;
    }
    int sizeOfParent()
    {
        return parents.size();
    }
    int getType()
    {
        return nodeType;
    }
    double getOutput()
    {
        error = 0;
        if (nodeType == BAIS)
        {
            output = 1;
            net = 1; // do i need this i should check
            return 1;
        }
        if (nodeType == INPUT)
        {
            return value;
        }
        net = getNetTotal();
        output = calcSig(net);
        return output;
    }
    double getSavedOut()
    {
        if(nodeType == BAIS)
            return 1;
        if(nodeType == INPUT)
            return value;
        return output;
    }
    double getNetTotal()
    {
        double net = 0;
        for (unsigned int i = 0; i < weightFromParents.size(); i++)
        {
            net += weightFromParents[i] * parents[i]->getOutput();
        }
        return net;
    }
    double getNetDer()
    {
        return output * (1 - output);
    }
    void setTarget(double target)
    {
        this->target = target;
    }
    double getTarget()
    {
        return target;
    }
    void calcError()
    {
        if(nodeType == INPUT)
            return;
        if (nodeType == OUTPUT)
        {
            errorCom = (target - output) * getNetDer();
            // cout << "target = " <<target << " out = " << output <<endl;
            // cout <<"error com= " << errorCom <<endl;
        }
        if(nodeType == HIDDEN)
            errorCom = error * getNetDer();
        for (unsigned int i = 0; i < parents.size(); i++)
        {
            parents[i]->addPartError(errorCom * weightFromParents[i]);
        } 
        for(unsigned int i = 0 ; i < parents.size();i++)
        {
            //parents[i]->calcError();
        }  
    }
    void addPartError(double error)
    {
        this->error += error;
    }
    double getErrorCom()
    {
        return errorCom;
    }
    string getWeightChange()
    {
        stringstream ss;
        for (unsigned int i = 0 ; i < parents.size();i++)
        {
            ss << parents[i]->getOutput() * errorCom * learningRate << " ";
        }
        return ss.str();;
    }
    void changeWeight()
    {
        if(nodeType == INPUT)
            return;
        for(unsigned int i = 0 ; i < parents.size();i++)
        {
            double cw = parents[i]->getSavedOut() * errorCom * learningRate;
            weightFromParents[i] += cw + lastWeight[i] * momCon;
            lastWeight[i] = cw + lastWeight[i] * momCon;
            //  cout << "im in here";
        }
        for(unsigned int i = 0 ; i < parents.size();i++)
        {
           // parents[i]->changeWeight();
        }
    }
    string getWeightPar()
    {
        if(nodeType == BAIS)
            return "";
        stringstream ss;
        for(unsigned int i = 0 ; i < weightFromParents.size();i++)
        {
            ss << weightFromParents[i] << " ";
        }
        return ss.str();
    }
};
