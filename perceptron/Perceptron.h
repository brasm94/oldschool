// ----------------------------------------------------------------
// The contents of this file are distributed under the CC0 license.
// See http://creativecommons.org/publicdomain/zero/1.0/
// ----------------------------------------------------------------

#include "learner.h"
#include "rand.h"
#include "error.h"
#include <iostream>
#include <algorithm>
using namespace std;

class PerceptronLearner : public SupervisedLearner
{
  private:
	Rand &m_rand;			   // pseudo-random number generator (not actually used by the baseline learner)
	vector<double> m_labelVec; // The label vector that this learner will always predict
	double rate;
	vector<double> weights;
	vector<double> weight2; // for iris 2
	vector<double> weight3; // for iris 3

  public:
	PerceptronLearner(Rand &r): SupervisedLearner(), m_rand(r)
	{
		rate = .1;
	}

	virtual ~PerceptronLearner()
	{
	}
	void printWeights()
	{
		// cout << "W1 = \n";
		for (unsigned int i = 0; i < weights.size(); i++)
		{
			cout << weights[i] << ",";
		}
		cout << endl;
		// cout << "W2 = \n";
		for (unsigned int i = 0; i < weight2.size(); i++)
		{
			cout << weight2[i] << ",";
		}
		cout << endl;
		// cout << "W3 = \n";
		for (unsigned int i = 0; i < weight3.size(); i++)
		{
			cout << weight3[i] << ",";
		}
		cout << endl;
	}
	double getNet(size_t row, Matrix &features, vector<double> weightTemp)
	{
		double net = 0;
		for (size_t col = 0; col < features.cols(); col++)
		{
			net += features[row][col] * weightTemp[col];
		}
		net += weightTemp.back() * 1; // 1 is for the bais :)
		return net;
	}
	void changeWeight(vector<double> &tempWeight, int row, double dif, Matrix &labels, Matrix &features)
	{
		for (unsigned int i = 0; i < features.cols(); i++)
		{
			double chWeight = rate * (dif)*features[row][i];
			// cout << chWeight << " change in weight " <<endl;
			tempWeight[i] += chWeight;
			// cout << tempWeight[i] << "new tempweight elem" <<endl;
		}
		tempWeight[tempWeight.size() - 1] += rate * (dif)*1; //for bais
	}
	//void updateWeight(vector<double> weight)
	// Train the model to predict the labels
	virtual void train(Matrix &features, Matrix &labels)
	{
		// Check assumptions
		if (features.rows() != labels.rows())
			ThrowError("Expected the features and labels to have the same number of rows");

		// Shuffle the rows. (This won't really affect this learner, but with other
		// learners it is a good idea to shuffle the rows before doing any training.)
		features.shuffleRows(m_rand, &labels);

		// Throw away any previous training
		m_labelVec.clear();
		// int attribute = features.cols();
		// cout <<"num of atr = " << attribute << endl;
		//vector<double> weights;
		for (unsigned int i = 0; i < features.rows(); i++)
		{
			// cout << labels[i][0] << " ,";
		}
		for (unsigned int i = 0; i < features.cols() + 1; i++)
		{
			weights.push_back(1);
			weight2.push_back(1);
			weight3.push_back(1);
		}
		double right = 0;
		for (size_t row = 0; row < features.rows(); row++) // just to print the oth epoch as in before training
		{
			double net = getNet(row, features, weights);
			double outout = 0;
			if (net > 0)
				outout = 1;
			if (outout == labels[row][0])
			{
				right++;
			}
		}
		double acc = 0;
		acc = right / (double)features.rows();
		cout << "the 0th epoch is " << (1 - acc) << endl;
		//printWeights();
		//calculates the net value;
		int epoch = 0;
		//int count = 80;
		double accuracy = 0;
		double accuracy1 = 0;
		double accuracy2 = 0;
		double accuracy3 = 0;
		double accuracyPrev = -1;
		int count2 = 20;

		while ((accuracy > accuracyPrev + .05) || count2 > 0)
		// int bob = 10;
		// while (bob > 0)
		{
			// bob--;
			if (accuracy <= accuracyPrev + .05)
			{
				count2--;
			}
			else
			{
				accuracyPrev = accuracy;
				count2 = 20;
			}
			
			double count = 0;
			double count2 = 0;
			double count3 = 0;
			for (size_t row = 0; row < features.rows(); row++)
			{
				double net = getNet(row, features, weights);
				double net2 = getNet(row, features, weight2);
				double net3 = getNet(row, features, weight3);
				double outout = 0;
				double outout2 = 0;
				double outout3 = 0;
				double setosa = 0;
				double versi = 0;
				double virge = 0;
				if (labels[row][0] == 0)
					setosa = 1;
				if (labels[row][0] == 1)
					versi = 1;
				if (labels[row][0] == 2)
					virge = 1;
				//   cout << "seto = " << setosa << "versi = " << versi << "virge = " << virge << endl;
				if (net > 0)
					outout = 1;
				if (net2 > 0)
					outout2 = 1;
				if (net3 > 0)
					outout3 = 1;
				// cout << "out1 = " << net << " out2 = " << net2 << "out3 = " << net3 << endl;
				// cout << outout << " = outout " << setosa << " = set" <<endl;
				if (outout != setosa)
				{
					changeWeight(weights, row, (setosa - outout), labels, features);
				}
				else
				{
					count++;
				}
				if (outout2 != versi)
				{
					if(outout2 == 0 && versi == 1)
					{
						// cout << "i sohuld change to the positive" <<endl;
					}
					 changeWeight(weight2, row, (versi - outout2), labels, features);
				}
				else
				{
					count2++;
				}
				if (outout3 != virge)
				{
					// printWeights();
					changeWeight(weight3, row, (virge - outout3), labels, features);
					// printWeights();
				}
				else
				{
					count3++;
				}
			}
			accuracy1 = count /  features.rows();
			accuracy2 = count2 / features.rows();
			accuracy3 = count3 /  features.rows();
			accuracy = min(accuracy3, min(accuracy1,accuracy2));
			// cout << " miscalc = " << (1 - accuracy) << endl;
			// cout << accuracy <<endl;
			epoch++;
			// features.shuffleRows(m_rand, &labels);
		}
		printWeights();
		cout << "It took " << epoch << " epochs to finalize" << endl;
	}

	// Evaluate the features and predict the labels
	// int count = 0;
	virtual double predict(const std::vector<double> &features, std::vector<double> &labels)
	{
		double net = 0;
		double net2 = 0;
		double net3 = 0;
		// printWeights();
		for (unsigned int feat = 0; feat < features.size(); feat++)
		{

			net += features[feat] * weights[feat];
			net2 += features[feat] * weight2[feat];
			net3 += features[feat] * weight3[feat];
		}
		net += weights[features.size()] * 1;
		net2 += weight2[features.size()] * 1;
		net3 += weight3[features.size()] * 1;
		// cout << net << " " << net2 << " " << net3 << " the nets "
			//  << "count = " << count++ << endl;
		int outout = 0;
		if (net > 0)
		{
			outout = 0;
			if (net < net2)
				outout = 1;
			if (net < net3)
			{
				if(net3 > net2)
				outout = 2;
			}
			outout = 0;
		}
		else if (net2 > 0)
		{
			outout = 1;
			if(net2 < net3)
				outout = 2;
		}
		else if (net3 > 0)
		{
			outout = 2;
		}
			labels[0] = outout;
			return 0;
	}
};
