import logging
import sys
sys.path.append('..')
from bene.sim import Sim
from bene.tcp import TCP as TCPStub
from bene.tcp import logger, sender_logger, receiver_logger

# def main():

class TCP(TCPStub):
    """ A TCP connection between two hosts."""

    def __init__(self, transport, source_address, source_port,
                 destination_address, destination_port, app=None, window=1000,drop=[],retran=False,fd=None):
        self.MSS = 1000
        self.retran = retran
        self.dupAck = 0
        self.cAck = -1
        # self.filename = filename
        self.fd = fd
        super(TCP, self).__init__(transport, source_address, source_port,
                            destination_address, destination_port, app, window, drop)
        # self.timeout = 100
    ''' Sender '''

    def sendMAX(self):
        # if self.send_buffer.available() == 0 and :
        #     print("this is here")
        #     return       
        while (self.send_buffer.next_seq - self.send_buffer.base_seq) + min(self.MSS,self.send_buffer.available()) <= self.cwnd and self.send_buffer.available() != 0:
            if self.timer is None:
                self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)        
            data,seq_num = self.send_buffer.get(self.MSS)
            self.send_packet(data,seq_num)

    def send(self, data):
        # print(self.timeout)
        # sys.exit()

        self.send_buffer.put(data)
        self.sendMAX()

### I think this is Done ?
    def handle_ack(self, packet):
        """ Handle an incoming ACK. """
        # print("GOT ACEK ",packet.ack_number)
        self.plot_sequence(packet.ack_number - packet.length,'ack')
        sender_logger.debug("%s (%s) received ACK from %s for %d" % (
            self.node.hostname, packet.destination_address, packet.source_address, packet.ack_number))
        ### got the  ACK for the end so finish
        # print("ACKKKK NUMBER",packet.ack_number)
        if packet.ack_number == self.send_buffer.last_seq:
            # print("this canceles 1")
            self.cancel_timer()
        if packet.ack_number > self.send_buffer.base_seq:
            self.send_buffer.slide(packet.ack_number)
            self.cancel_timer()
        if self.send_buffer.outstanding() != 0:
            # print("this canceles 2")
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)
        if packet.ack_number == self.send_buffer.last_seq:
            # print("stupid")
            return
        if self.retran:
            self.fast_retran(packet.ack_number)
        self.sendMAX()

    def retransmit(self, event):
        # print("this was called to retransmit")
        """ Retransmit data. """
        sender_logger.warning("%s (%s) retransmission timer fired" % (self.node.hostname, self.source_address))
        #FIXME
        data,seq_num = self.send_buffer.resend(self.MSS)
        if self.send_buffer.outstanding() == 0:
            return
        # print("RETRANSMIT",len(data)," SEQ NUM= ",seq_num)
        # print(seq_num)
        self.send_packet(data,seq_num)
        print(seq_num)
        self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)        
        # print("this was called again")
    
    def fast_retran(self,acknum):
        # print("FASTER THEN LIGHTING MCQEUEEN",acknum,self.cAck,self.dupAck)
        if self.cAck == acknum:
            self.dupAck += 1
        else:
            self.cAck = acknum
            self.dupAck = 0
            return
        # if self.dupAck == 2:
        #     print("1---------------\n---------------\n---------------\n---------------\n---------------\n---------------\n")
        #     sys.exit()
        if self.dupAck == 3:
            # print("---------------\n---------------\n---------------\n---------------\n---------------\n---------------\n")
            # sys.exit()
            data,seq_num = self.send_buffer.resend(self.MSS)
            # if self.send_buffer.outstanding() == 0:
            #     return
            self.send_packet(data,seq_num)
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit) 




    ''' Receiver '''

    def handle_data(self, packet):
        """ Handle incoming data."""
        # print("this was called")
        sender_logger.debug("%s (%s) received TCP segment from %s for %d" % (self.node.hostname, packet.destination_address, packet.source_address, packet.sequence))
        #FIXME
        # print(packet.length)
        self.receive_buffer.put(packet.body,packet.sequence)
        data,start = self.receive_buffer.get()
        if len(data) != 0:
            self.app.receive_data(data)
        self.ack = start + len(data)
        self.send_ack()
        self.fd.write("%f,%f\n" % (Sim.scheduler.current_time(),packet.queueing_delay))
        # print(self.fd)


if __name__ == '__main__':
    main()