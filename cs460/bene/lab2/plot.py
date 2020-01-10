import pandas
import matplotlib.pyplot as plt
import numpy as np
from os import listdir

def make_out_graph():
    thro_file = "data/throughput.csv"
    data = pandas.read_csv(thro_file).values
    print(data[:,1]/1000000)
    plt.plot(data[:,0],data[:,1]/1000000,label="Throughput")
    plt.ylabel("Mbps")
    plt.xlabel("Window Size (Bytes)")
    plt.legend()
    plt.savefig("graphs/throughput.eps")
    plt.clf()
    # pass

def make_queue_graph():
    sizes = [1000,2000,5000,10000,15000,20000]
    qd = []
    for sz in sizes:
        data = pandas.read_csv("data/queue-%i.csv" % sz).values
        average = np.mean(data,axis=0)[1]
        qd += [average * 1000] # times 1000 -> Mill sec
    print(qd)
    plt.plot(sizes,qd,label="Queueing Delay")
    plt.xlabel("Window Size (Bytes)")
    plt.ylabel("AVG Queueing Delay (MS)")
    plt.legend()
    plt.savefig("graphs/delay.eps")
    plt.clf()

def main():
    make_out_graph()
    make_queue_graph()

    
if __name__ == '__main__':
    main()