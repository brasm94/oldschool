from __future__ import print_function

import sys

sys.path.append('..')

from bene.network import Network
from bene.sim import Sim
from bene.packet import Packet
class PacketHandler(object):
    @staticmethod
    def receive_packet(packet, **kwargs):
        print(packet.ident,
        packet.created,
        Sim.scheduler.current_time(),
        Sim.scheduler.current_time() - packet.created,
        packet.transmission_delay,
        packet.propagation_delay,
        packet.queueing_delay)

def main():
    print("\n***Sceneraio 1***\n")
    run1("networks/fast-fast.txt")
    # print("\n***Sceneraio 2***\n")
    # run1("networks/faster-faster.txt")
    # print("\n***Sceneraio 3***\n")
    # run1("networks/fast-slow.txt")


def run1(filename):
    Sim.scheduler.reset()
    net = Network(filename)
    # setup routes
    a = net.get_node('a')
    b = net.get_node('b')
    c = net.get_node('c')
    a.add_forwarding_entry(c.get_address('b'), a.links[0])
    b.add_forwarding_entry(c.get_address('b'), b.links[1])
    # setup app
    PH = PacketHandler()
    net.nodes['c'].add_protocol(protocol="delay",handler=PH)
    amount_data = 8*1000000
    packet_size = 8*1000
    packet2send = amount_data / packet_size
    delay_amount = packet_size / a.links[0].bandwidth
    count = 0
    for i in range(int(packet2send)):
        count += 1
        p = Packet(destination_address=c.get_address('b'),protocol='delay',ident=i+1,length=1000)
        Sim.scheduler.add(delay=(i * delay_amount), event=p, handler=a.send_packet)
    Sim.scheduler.run()
if __name__ == '__main__':
    main()