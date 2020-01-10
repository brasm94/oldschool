from __future__ import print_function

import logging
import optparse
import os
import subprocess
import sys

sys.path.append('..')

from bene.network import Network
from bene.sim import Sim, TERM_COLOR_GREEN
from bene.transport import Transport

from tcp import TCP


logger = logging.getLogger('app')
# uncomment the lines below to make app output green
#from bene.sim import TERM_COLOR_GREEN
#Sim.add_console_logging('app', TERM_COLOR_GREEN)

class AppHandler(object):
    def __init__(self, filename):
        self.filename = filename
        self.directory = 'received'
        if not os.path.exists(self.directory):
            os.makedirs(self.directory)
        self.f = open(os.path.join(self.directory, os.path.basename(self.filename)), 'wb')
        self.count = 0

    def receive_data(self, data):
        self.count += 1

        logger.info("%d application got %d bytes" % (self.count,len(data)))
        self.f.write(data)
        self.f.flush()

    def packet_stats(self, packet):
        pass

class Main(object):
    def __init__(self):
        self.directory = 'received'
        self.parse_options()
        self.run_all()
        # self.throPut()
        self.diff()
        self.filename = None
        self.loss = None
        self.wsize = None

    def parse_options(self):
        parser = optparse.OptionParser(usage="%prog [options]",
                                       version="%prog 0.1")

        parser.add_option("-f", "--filename", type="str", dest="filename",
                          default='../examples/internet-architecture.pdf',
                          help="filename to send")

        parser.add_option("-l", "--loss", type="float", dest="loss",
                          default=0.0,
                          help="random loss rate")
        parser.add_option("-w", "--window", dest="wsize",type='int',
                            default=3000,
                          help="Window size")

        parser.add_option("-d", "--debug", dest="debug",
                          action="store_true", default=False,
                          help="debug logging")
        parser.add_option("-r", "--retran", dest="retran",
                          action="store_true", default=False,
                          help="Fast Retransmit logging")

        (options, args) = parser.parse_args()
        self.filename = options.filename
        self.loss = options.loss
        self.debug = options.debug
        self.wsize = options.wsize
        self.retran = options.retran

    def diff(self):
        args = ['diff','-u', self.filename, os.path.join(self.directory, os.path.basename(self.filename))]
        try:
            result = subprocess.check_call(args)
        except OSError as e:
            print("There was a problem running diff: %s" % e)
        except subprocess.CalledProcessError as e:
            print("File transfer failed.  %s" % e)
            sys.exit()
        else:
            print("File transfer correct!")

    def run_all(self):
        sizes = [1000,2000,5000,10000,15000,20000]
        fd = open("data/throughput.csv","w")
        fd.write("Window_size,Time\n")
        for sz in sizes:
            dtime,dsize = self.run(sz)
            fd.write("%i,%f\n" % (sz,(dsize*8)/dtime))
            self.diff()
            

    def run(self,size):
        # parameters
        Sim.scheduler.reset()
        logging.getLogger('app').setLevel(logging.INFO)
        logging.getLogger('bene.link.queue').setLevel(logging.DEBUG)
        logging.getLogger('bene.tcp.sequence').setLevel(logging.DEBUG)
        logging.getLogger('bene.tcp.cwnd').setLevel(logging.DEBUG)
        if self.debug:
            logging.getLogger('bene.tcp').setLevel(logging.DEBUG)
            logging.getLogger('bene.tcp.sender').setLevel(logging.DEBUG)
            logging.getLogger('bene.tcp.receiver').setLevel(logging.DEBUG)

        # setup network
        net = Network('networks/one-hop-q10.txt')
        net.loss(self.loss)

        # setup routes
        n1 = net.get_node('n1')
        n2 = net.get_node('n2')
        n1.add_forwarding_entry(n2.get_address('n1'), n1.links[0])
        n2.add_forwarding_entry(n1.get_address('n2'), n2.links[0])

        # setup transport
        t1 = Transport(n1)
        t2 = Transport(n2)

        # setup application
        a = AppHandler(self.filename)

        window = size

        # setup connection
        drop = []
        filen = ("data/queue-%i.csv" % size)
        fd = open(filen,"w")
        fd.write("Time_stamp,qDelay\n")
        c1 = TCP(t1, n1.get_address('n2'), 1, n2.get_address('n1'), 1, a, window=window, drop=drop,retran=self.retran)
        c2 = TCP(t2, n2.get_address('n1'), 1, n1.get_address('n2'), 1, a, window=window,fd=fd)
        # send a file
        time_before = Sim.scheduler.current_time()
        count = 0
        with open(self.filename, 'rb') as f:
            while True:
                data = f.read(1000)
                count += len(data)
                # print("0",count)
                if not data:
                    break
                Sim.scheduler.add(delay=0, event=data, handler=c1.send)
            Sim.scheduler.run()
        time_after = Sim.scheduler.current_time()
        Sim.scheduler.reset()
        fd.close()
        return (time_after - time_before),count

    def throPut(self):
        sizes = [1000,2000,5000,10000,15000,20000]
        print(sizes)
        for sz in (sizes):
            print(sz)
            with open ("data/queue-%i.csv" % sz,"r") as f:
                data = f.read()
                data = data.replace("\n",",")
                data = data[:-1]
                val = data.split(",")
                diff = float(val[0]) - float(val[-1])
                print("dTime",diff)
            

if __name__ == '__main__':
    m = Main()
