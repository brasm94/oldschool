import pandas
import matplotlib.pyplot as plt
import numpy as np
from os import listdir



def w(p,service_rate):
	return p / ( 2 * service_rate * (1 - p))

def main():

	# for file in listdir("data"):
	all_means = []
	ps = np.asarray([.1,.2,.3,.4,.5,.6,.7,.8,.9,.95,.98])
	for file in listdir("data"):
		data = pandas.read_csv("data/" + file)
		all_means += [data.mean(axis=0)[1]]
	service_rate = 1000000 / (8 * 1000)
	p_queue = w(ps,service_rate)
	plt.plot(ps,all_means,label="Actual")
	plt.plot(ps,p_queue,label="Theoretical")
	plt.ylabel("AVG Queueing Delay")
	plt.xlabel("Utilization")
	plt.xticks(np.linspace(0,1,11))
	plt.ylim(0)
	plt.legend()
	# plt.show()
	plt.savefig("graphs/delay.eps")
	print(p_queue)

if __name__ == '__main__':
    main()
