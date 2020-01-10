from __future__ import print_function

import sys

sys.path.append('..')

from bene.network import Network
from bene.sim import Sim
from bene.packet import Packet
import numpy as np
import random

class Generator(object):
    def __init__(self, node, destination, load, duration):
        self.node = node
        self.load = load
        self.destination = destination
        self.duration = duration
        self.start = 0
        self.ident = 1

    def handle(self, event):
        # quit if done
        now = Sim.scheduler.current_time()
        if (now - self.start) > self.duration:
            return
        # generate a packet
        self.ident += 1
        p = Packet(destination_address=self.destination, ident=self.ident, protocol='delay', length=1000)
        Sim.scheduler.add(delay=0, event=p, handler=self.node.send_packet)
        # schedule the next time we should generate a packet
        Sim.scheduler.add(delay=random.expovariate(self.load), event='generate', handler=self.handle)


class DelayHandler(object):
    def __init__(self,file):
        self.file = file
    # @staticmethod
    def receive_packet(self,packet,link):
        temp = "{},{}\n".format(Sim.scheduler.current_time(),packet.queueing_delay)
        self.file.write(temp)
        # print("Stupid")
            # (Sim.scheduler.current_time(),
        #        packet.ident,
        #        packet.created,
        #        Sim.scheduler.current_time() - packet.created,
        #        packet.transmission_delay,
        #        packet.propagation_delay,
        #        packet.queueing_delay))

def main():
    for p in [.1,.2,.3,.4,.5,.6,.7,.8,.9,.95,.98]:
        print("data/queue-{:01.2f}.csv".format(p))
        file = open("data/queue-{:01.2f}.csv".format(p),'w')
        run1("networks/fast-fast.txt",1000,p,file)
        file.close()
def run1(filename,packetsize,p,file):
    Sim.scheduler.reset()
    net = Network(filename)
    a = net.get_node('a')
    b = net.get_node('b')
    c = net.get_node('c')
    a.add_forwarding_entry(c.get_address('b'), a.links[0])
    b.add_forwarding_entry(c.get_address('b'), b.links[1])
    d = DelayHandler(file)
    net.nodes['c'].add_protocol(protocol="delay", handler=d)

    bandwidth = a.links[0].bandwidth
    service_rate = bandwidth/ (packetsize * 8)
    arrival_rate = p * service_rate

    destination = c.get_address('b')
    g = Generator(node=a, destination=destination, load=arrival_rate, duration=600)
    Sim.scheduler.add(delay=0, event='generate', handler=g.handle)
    Sim.scheduler.run()
if __name__ == '__main__':
    main()
