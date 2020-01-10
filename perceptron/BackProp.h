// ----------------------------------------------------------------
// The contents of this file are distributed under the CC0 license.
// See http://creativecommons.org/publicdomain/zero/1.0/
// ----------------------------------------------------------------

#include "learner.h"
// #include "rand.h"
#include "error.h"
#include "Node.h"
#include <fstream>

using namespace std;

// This is a very simple supervised learning algorithm that we provide as an example
// to get you started. It finds the most common class in the training set and
// always predicts that class. (Or, if you are doing regression, it always predicts
// the centroid label.)
int debug = 1;

class BackPropLearner : public SupervisedLearner
{
  private:
	Rand &m_rand;			   // pseudo-random number generator (not actually used by the baseline learner)
	vector<double> m_labelVec; // The label vector that this learner will always predict
	vector<Node *> inputNodes;
	vector<vector<Node *> > hiddenLayer;
	vector<Node *> outNodes;

  public:
	BackPropLearner(Rand &r) : SupervisedLearner(), m_rand(r)
	{
		m_rand = r;
	}

	virtual ~BackPropLearner()
	{
	}
	void init(Matrix &features, Matrix &labels)
	{
		int hidden = features.cols() * 2;
		int outSize = labels.columnMax(0) + 1;
		int hidLayer = 5;
		for (unsigned int i = 0; i < features.cols(); i++)
		{
			inputNodes.push_back(new Node(INPUT));
		}
		inputNodes.push_back(new Node(BAIS));
		vector<Node *> tempHid = inputNodes;
		vector<Node *> hiddenNodes;
		for (int hid = 0; hid < hidLayer; hid++)
		{
			hiddenNodes = vector<Node *>();
			for (int i = 0; i < hidden; i++)
			{
				hiddenNodes.push_back(new Node(tempHid, HIDDEN, m_rand));
			}
			hiddenNodes.push_back(new Node(tempHid, BAIS, m_rand));
			hiddenLayer.push_back(hiddenNodes);
			tempHid = hiddenNodes;
		}
		for (int i = 0; i < outSize; i++) // i hardcoded 3.
		{
			outNodes.push_back(new Node(tempHid, OUTPUT, m_rand));
		}
	}
	// void printLayers()
	// {
	// 	// if(debug)cout << "output = " ;
	// 	for (unsigned int i = 0; i < outNodes.size(); i++)
	// 	{
	// 		cout << outNodes[i]->getWeightChange() << " , ";
	// 	}
	// 	cout << endl;
	// 	for (unsigned int i = 0; i < hiddenNodes.size(); i++)
	// 	{
	// 		cout << hiddenNodes[i]->getWeightChange() << " ";
	// 	}
	// 	cout << endl;
	// 	for (unsigned int i = 0; i < inputNodes.size(); i++)
	// 	{
	// 		cout << inputNodes[i]->getWeightChange() << " ";
	// 	}
	// }
	void getRandVector()
	{
		vector<double> rands;
		for (int i = 0; i < 10; i++)
		{
			double randTemp = m_rand.normal();
			cout << randTemp << " r ";
		}
		cout << endl;
	}
	// Train the model to predict the labels
	virtual void train(Matrix &features, Matrix &labels)
	{
		// getRandVector();
		if (debug)
			cout << "the number of labels " << labels.cols() << endl;
		// Check assumptions
		if (features.rows() != labels.rows())
			ThrowError("Expected the features and labels to have the same number of rows");

		// Shuffle the rows. (This won't really affect this learner, but with other
		// learners it is a good idea to shuffle the rows before doing any training.)
		features.shuffleRows(m_rand, &labels);
		//split for validation sets
		Matrix fTrain;
		Matrix VS;
		fTrain.copyPart(features, 0, 0, features.rows() * .75, features.cols());
		cout << "to here" << endl;
		VS.copyPart(features, int(features.rows() * .75), 0, features.rows() - features.rows() * .75, features.cols());
		Matrix lTrain;
		Matrix VSLabel;
		lTrain.copyPart(labels, 0, 0, labels.rows() * .75, labels.cols());
		VSLabel.copyPart(labels, labels.rows() * .75, 0, labels.rows() - labels.rows() * .75, labels.cols());
		// Throw away any previous training
		cout << "to here" << endl;

		m_labelVec.clear();
		init(fTrain, lTrain); // setup net
		double bssf = 1000;
		double correctBest = 0;
		// for (int run = 0; run < 500; run++)
		int changed = 5;
		int iterations = 0;
		ofstream myMSE;
		myMSE.open("mse.txt", ofstream::app);
		double errorTrain = 0;
		double error = 0;
		while (changed > 0)
		{
			errorTrain = 0;
			iterations++;
			for (unsigned int row = 0; row < fTrain.rows(); row++) //features.rows();row++)
			{
				for (unsigned int i = 0; i < fTrain.cols(); i++)
				{
					inputNodes[i]->setValue(fTrain[row][i]);
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					// outNodes[i]->setTarget(lTrain[row][0]);
					if (i == lTrain[row][0])
						outNodes[i]->setTarget(1);
					else
						outNodes[i]->setTarget(0);
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					double out = outNodes[i]->getOutput();
					double pError = (outNodes[i]->getTarget() - out) * (outNodes[i]->getTarget() - out);
					errorTrain += pError;
					// if(out > .5 && i == VSLabel[rowVS][0])
					// correct++;
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					outNodes[i]->calcError();
				}
				for (unsigned int hid = 0; hid < hiddenLayer.size(); hid++)
				{
					for (unsigned int i = 0; i < hiddenLayer[hid].size(); i++)
					{
						hiddenLayer[hid][i]->calcError();
					}
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					outNodes[i]->changeWeight();
				}
				for (unsigned int hid = 0; hid < hiddenLayer.size(); hid++)
				{
					for (unsigned int i = 0; i < hiddenLayer[hid].size(); i++)
					{
						hiddenLayer[hid][i]->changeWeight();
					}
				}
			}

			fTrain.shuffleRows(m_rand, &lTrain);
			error = 0;
			double correct = 0;
			for (unsigned int rowVS = 0; rowVS < VS.rows(); rowVS++)
			{
				for (unsigned int i = 0; i < VS.cols(); i++)
				{
					inputNodes[i]->setValue(VS[rowVS][i]);
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					// outNodes[i]->setTarget(lTrain[row][0]);
					if (i == VSLabel[rowVS][0])
						outNodes[i]->setTarget(1);
					else
						outNodes[i]->setTarget(0);
				}
				for (unsigned int i = 0; i < outNodes.size(); i++)
				{
					double out = outNodes[i]->getOutput();
					double pError = (outNodes[i]->getTarget() - out) * (outNodes[i]->getTarget() - out);
					error += pError;
					if (out > .5 && i == VSLabel[rowVS][0])
						correct++;
				}
			}
			// cout << "error on this bach = " << errorTrain / lTrain.rows() <<endl;
			// cout << "the percent  error = " << error / VSLabel.rows() << endl;
			// myMSE << iterations << " , " << errorTrain / lTrain.rows() << " , " << error / VSLabel.rows() << " , " << correct / VSLabel.rows() << " \r\n";
			// cout << "the number correct = " <<correct << " out of " << VSLabel.rows() <<endl;
			changed--;
			if (correctBest < correct)
			{
				correctBest = correct;
				changed = 5;
			}
			if (bssf > error / VSLabel.rows())
			{
				bssf = error / VSLabel.rows();
				changed = 5;
			}
		}
		 myMSE << iterations << " , " << error/VSLabel.rows() <<" , " << errorTrain / lTrain.rows();// << "\r\n";
		myMSE.close();
		cout << "VSlabel row = " << VSLabel.rows() << endl;
		cout << "this is the error" << error << endl;
		cout << "VS row = " << VS.rows() << endl;
		cout << "the tot iter = " << iterations << endl;
	}

	// Evaluate the features and predict the labels

	virtual double predict(const std::vector<double> &features, std::vector<double> &labels)
	{
		// if(labels.size() != m_labelVec.size())
		// 	ThrowError("Unexpected number of label dims");
		// for(size_t i = 0; i < labels.size(); i++)
		// 	labels[i] = m_labelVec[i];
		for (unsigned int i = 0; i < features.size(); i++)
		{
			inputNodes[i]->setValue(features[i]);
		}
		//  cout <<"the outnode Size = " << outNodes.size() << endl;
		// for (unsigned int i = 0; i < outNodes.size(); i++)
		// {
		// 	double out = outNodes[i]->getOutput();
		// 	if (out > .5)
		// 		labels[0] = i;
		// }
		// return 0;
		double err = 0;
		for (unsigned int i = 0; i < outNodes.size(); i++)
		{
			double out = outNodes[i]->getOutput();
			if(labels[0] == i)
				err += (1 - out) * (1 - out);
			else
				err += out * out;
		}
		return err;
	}
};
