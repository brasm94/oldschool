from __future__ import print_function

import sys

sys.path.append('..')

from bene.network import Network
from bene.sim import Sim
from bene.packet import Packet
class PacketHandler(object):
    @staticmethod
    def receive_packet(packet, **kwargs):
        print(packet.ident,packet.created,Sim.scheduler.current_time())

def main():
    print("\n***Sceneraio 1***\n")
    run1()
    print("\n***Sceneraio 2***\n")
    run2()
    print("\n***Sceneraio 3***\n")
    run3()
#### LOL I copied and pasted code for different networks lol.
def run1():
    Sim.scheduler.reset()
    net = Network("networks/fast-long.txt")
    # setup routes
    n1 = net.get_node('n1')
    n2 = net.get_node('n2')
    n1.add_forwarding_entry(n2.get_address('n1'), n1.links[0])
    n2.add_forwarding_entry(n1.get_address('n2'), n2.links[0])

    # setup app
    PH = PacketHandler()
    net.nodes['n2'].add_protocol(protocol="delay",handler=PH)
    p = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=1,length=1000)
    Sim.scheduler.add(delay=0, event=p, handler=n1.send_packet)
    Sim.scheduler.run()
def run2():
    Sim.scheduler.reset()
    net = Network("networks/slow-short.txt")
    # setup routes
    n1 = net.get_node('n1')
    n2 = net.get_node('n2')
    n1.add_forwarding_entry(n2.get_address('n1'), n1.links[0])
    n2.add_forwarding_entry(n1.get_address('n2'), n2.links[0])
    # setup app
    PH = PacketHandler()
    net.nodes['n2'].add_protocol(protocol="delay",handler=PH)
    p = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=1,length=1000)
    Sim.scheduler.add(delay=0, event=p, handler=n1.send_packet)
    Sim.scheduler.run()
def run3():
    Sim.scheduler.reset()
    net = Network("networks/fast-short.txt")
    # setup routes
    n1 = net.get_node('n1')
    n2 = net.get_node('n2')
    n1.add_forwarding_entry(n2.get_address('n1'), n1.links[0])
    n2.add_forwarding_entry(n1.get_address('n2'), n2.links[0])
    # setup app
    PH = PacketHandler()
    net.nodes['n2'].add_protocol(protocol="delay",handler=PH)
    p1 = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=1,length=1000)
    p2 = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=2,length=1000)
    p3 = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=3,length=1000)
    p4 = Packet(destination_address=n2.get_address('n1'),protocol='delay',ident=4,length=1000)
    Sim.scheduler.add(delay=0, event=p1, handler=n1.send_packet)
    Sim.scheduler.add(delay=0, event=p2, handler=n1.send_packet)
    Sim.scheduler.add(delay=0, event=p3, handler=n1.send_packet)
    Sim.scheduler.add(delay=2, event=p4, handler=n1.send_packet)
    Sim.scheduler.run()


if __name__ == '__main__':
    main()