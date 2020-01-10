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
        ##Congestion control Variables
        self.mss_count = 1
        self.MSS = 1000
        self.cwnd = self.mss_count * self.MSS
        self.mode = "slow_start"
        self.threshold = 100000
        self.increment = 0

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
        if packet.ack_number > self.send_buffer.base_seq:
            self.change_cwnd(packet.ack_number - self.send_buffer.base_seq) ### Added for TCP TAHOE <-- question about this....
            self.send_buffer.slide(packet.ack_number)
            self.cancel_timer()
        if self.send_buffer.outstanding() != 0:
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)
        if packet.ack_number == self.send_buffer.last_seq:
            self.cancel_timer()
            return
        if self.retran:
            self.fast_retran(packet.ack_number)
        self.sendMAX()

    def retransmit(self, event):
        """ Retransmit data. """
        sender_logger.warning("%s (%s) retransmission timer fired" % (self.node.hostname, self.source_address))

        ### Reset Window ###
        self.reset_cwnd()


        data,seq_num = self.send_buffer.resend(self.MSS)
        if self.send_buffer.outstanding() == 0:
            return
        self.send_packet(data,seq_num)
        # print(seq_num)
        self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit)        
    
    def fast_retran(self,acknum):
        if self.cAck == acknum:
            self.dupAck += 1
        else:
            self.cAck = acknum
            self.dupAck = 0
            return
        
        if self.dupAck == 3:
            self.reset_cwnd()
            data,seq_num = self.send_buffer.resend(self.MSS)
            self.send_packet(data,seq_num)
            self.cancel_timer()
            self.timer = Sim.scheduler.add(delay=self.timeout, event='retransmit', handler=self.retransmit) 

    def reset_cwnd(self):
        temp = self.threshold
        half = max(self.cwnd / 2,self.mss)
        half -= (half % self.mss) ## make it divisible by MSS
        self.threshold = half # reset threshold
        print("threshold was {} is now {} cwnd was {}".format(temp,self.threshold,self.cwnd))
        self.cwnd = self.mss
        self.increment = 0
    def change_cwnd(self,b): # b is for byte
        # print("BYTES === ",b)
        if self.mode == "slow_start":
        
            self.cwnd += self.MSS
            if self.cwnd >= self.threshold:
                self.mode = "congestion_avoidance"
        
        elif self.mode == "congestion_avoidance":
        
            self.increment += (self.mss * b) / self.cwnd
            if self.increment >= self.mss:
                self.increment -= self.mss
                self.cwnd += self.mss

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
        # self.fd.write("%f,%f\n" % (Sim.scheduler.current_time(),packet.queueing_delay))
        # print(self.fd)


if __name__ == '__main__':
    main()