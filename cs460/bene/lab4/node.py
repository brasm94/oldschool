import copy
import logging

from bene.node import Node
from bene.packet import Packet
from bene.ip import *
from bene.sim import Sim

# logger = logging.getLogger('bene.node')
logger = logging.getLogger(__name__)


# logger.set
print(logger)

class DVRouter(Node):
    def __init__(self, hostname, default_gateway=None):
        # print(self.__dict__)
        super(DVRouter, self).__init__(hostname, default_gateway)

        self.cost2neighbors = {}
        self.DV = {}
        self.DVneighbors = {}
        self.neighborsTimer = {}


    def updateTable(self,data):
        ####

        oldDV = self.DV
        self.DV = {}

        ### Re init DV ####
        for link in self.links:
            # print(self.hostname,link)
            self.DV[link.address] = 0 

        ### go throught DVs of neighbors and upadte our DV
        for hn,nDV in self.DVneighbors.items():
            for n,dis in nDV.items():
                if n in self.DV:
                    if self.DV[n] > dis + self.cost2neighbors[hn]:
                        self.DV[n] = dis + self.cost2neighbors[hn]
                        self.add_forwarding_entry(n,self.get_link(hn))
                else:
                    self.DV[n] = dis + self.cost2neighbors[hn]
                    self.add_forwarding_entry(n,self.get_link(hn))
        # print(self.forwarding_table.entries)        

        p = Packet(
            source_address= None, ## this might need to be changed :D 
            destination_address=BROADCAST_IP_ADDRESS,
            ident=self.hostname, ttl=1, protocol='dvrouting',body=self.DV,length=100)
        
        if oldDV != self.DV:
            # print("this was called")
            # Sim.scheduler.add(delay=1,event=p,handler=self.send_packet)
            self.sendDV()
        # Sim.scheduler.add(delay=1,event=None,handler=self.updateTable)
    def checkLinks(self,data):
        for hn,count in self.neighborsTimer.items():
            if count == 1: ## one because I update after so 3-2 (-1) = 1 (-1) 
                del self.DVneighbors[hn]
                # print("removed this guy " , hn)
            self.neighborsTimer[hn] -= 1        


        p = Packet(
            source_address= None, ## this might need to be changed :D 
            destination_address=BROADCAST_IP_ADDRESS,
            ident=self.hostname, ttl=1, protocol='dvrouting',body=self.DV,length=100)

        Sim.scheduler.add(delay=1,event=None,handler=self.checkLinks)

    def sendDV(self):
        p = Packet(
            source_address= None, ## this might need to be changed :D 
            destination_address=BROADCAST_IP_ADDRESS,
            ident=self.hostname, ttl=1, protocol='dvrouting',body=self.DV,length=100)
        # print("stupid")
        logger.debug(str(self.hostname) + " is sending its DV")
        Sim.scheduler.add(delay=1,event=p,handler=self.send_packet)



