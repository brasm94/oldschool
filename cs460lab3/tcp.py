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
                 destination_address, destination_port, app=None, window=1000,drop=[],retran=False,fd=None,reno=False):
        super(TCP, self).__init__(transport, source_address, source_port,
                            destination_address, destination_port, app, window, drop)
        ##Congestion control Variables
        self.mss_count = 1
        self.MSS = 1000
        self.cwnd -= self.cwnd
        self.cwnd += self.MSS

        self.mode = "slow_start"
        self.threshold = 100000
        self.increment = 0
        self.reno = reno

        self.retran = True
        self.dupAck = 0
        self.cAck = -1
        # self.filename = filename
        self.fd = fd
        # self.timeout = 100
    ''' Sender '''

    def sendMAX(self):
        while (self.send_buffer.next_seq - self.send_buffer.base_seq) + min(self.MSS,self.send_buffer.available()) <= self.cwnd and self.send_buffer.available() != 0:
            if self.timer is None:
                self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)        
            data,seq_num = self.send_buffer.get(self.MSS)
            self.send_packet(data,seq_num)

    def send(self, data):

        self.send_buffer.put(data)
        self.sendMAX()

### I think this is Done ?
    def handle_ack(self, packet):
        """ Handle an incoming ACK. """
        self.plot_sequence(packet.ack_number - packet.length,'ack')
        sender_logger.debug("%s (%s) received ACK from %s for %d" % (
            self.node.hostname, packet.destination_address, packet.source_address, packet.ack_number))
        # print("current packet number",self.send_buffer.base_seq,"received packet number",packet.ack_number)
        # print("RECIEVED ",packet.ack_number)
        if packet.ack_number < self.send_buffer.base_seq:
            return
        if packet.ack_number == self.send_buffer.last_seq:
            self.cancel_timer()
            return
        if packet.ack_number > self.send_buffer.base_seq:
            self.change_cwnd(packet.ack_number - self.send_buffer.base_seq) ### Added for TCP TAHOE <-- question about this....
            self.send_buffer.slide(packet.ack_number)
            self.cancel_timer()
            # self.dupAck = 0 ## added
        dupped = False
        if self.retran:
            dupped = self.fast_retran(packet.ack_number)
        if not dupped:
            self.sendMAX()
        # if dupped:
            # return
        if self.send_buffer.outstanding() != 0:
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)

    def retransmit(self, event):
        """ Retransmit data. """
        sender_logger.warning("%s (%s) retransmission timer fired" % (self.node.hostname, self.source_address))

        ### Reset Window ###
        self.reset_cwnd(False)


        data,seq_num = self.send_buffer.resend(self.MSS)
        if self.send_buffer.outstanding() == 0:
            return
        self.send_packet(data,seq_num)
        # print(seq_num)
        self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)        
    
    def fast_retran(self,acknum):
        # print("Acn #s",self.cAck," ",acknum, " ",self.cwnd)
        if self.cAck == acknum:
            self.dupAck += 1
            # print("AIASMM  ASD")
        else:
            # self.sequence = acknum
            self.cAck = acknum
            self.dupAck = 0
        
        if self.dupAck == 3:
            # self.dupAck += 1
            data,seq_num = self.send_buffer.resend(self.MSS)
            temp = self.cwnd
            self.reset_cwnd(True)
            print("Triple dupped called",self.cAck," ",acknum," CWND ",self.cwnd,temp)
            self.send_packet(data,seq_num)
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit) 
            return True
        if self.dupAck > 3:
            return True
        return False
    def reset_cwnd(self,triple):
        temp = self.threshold
        half = max(self.cwnd / 2,self.mss)
        half -= (half % self.mss) ## make it divisible by MSS
        self.threshold = half # reset threshold
        if self.reno and triple:
            # print("using reno threshold")
            self.cwnd = self.threshold + self.mss
        else:    
            self.cwnd = self.mss
        self.increment = 0
        self.plot_cwnd()

    def change_cwnd(self,b): # b is for byte
        # print("BYTES === ",b)
        # print(self.cwnd)
        if self.cwnd < self.threshold:
            self.cwnd += self.MSS
        else:
            self.increment += (self.mss * b) / self.cwnd
            if self.increment >= self.mss:
                self.increment -= self.mss
                self.cwnd += self.mss
        # if b == 0:
        #     print("THIS soidnfksjnfkljasndfkjn")
        # print("cwnd",self.cwnd)
        self.plot_cwnd()

    ''' Receiver '''

    def handle_data(self, packet):
        """ Handle incoming data."""
        # print("this was called")
        sender_logger.debug("%s (%s) received TCP segment from %s for %d" % (self.node.hostname, packet.destination_address, packet.source_address, packet.sequence))
        #FIXME
        self.receive_buffer.put(packet.body,packet.sequence)
        data,start = self.receive_buffer.get()
        # print(start)
        if len(data) != 0:
            self.app.receive_data(data)
        self.ack = start + len(data)
        self.send_ack()
        # self.fd.write("%f,%f\n" % (Sim.scheduler.current_time(),packet.queueing_delay))
        # print(self.fd)


if __name__ == '__main__':
    main()
