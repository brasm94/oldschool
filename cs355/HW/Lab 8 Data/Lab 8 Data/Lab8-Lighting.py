""" Modified code from Peter Colling Ridge 
	Original found at http://www.petercollingridge.co.uk/pygame-3d-graphics-tutorial
"""

import pygame, math
import numpy as np
import wireframe as wf
import basicShapes as shape

class WireframeViewer(wf.WireframeGroup):
    """ A group of wireframes which can be displayed on a Pygame screen """
    
    def __init__(self, width, height, name="Wireframe Viewer"):
        self.width = width
        self.height = height
        
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption(name)
        
        self.wireframes = {}
        self.wireframe_colours = {}
        self.object_to_update = []
        
        self.displayNodes = False
        self.displayEdges = True
        self.displayFaces = True
        
        self.perspective = False
        self.eyeX = self.width/2
        self.eyeY = 100
        self.light_color = np.array([1,1,1])*1
        self.view_vector = np.array([0, 0, -1])        
        self.light_vector = np.array([0.0, 0.0, -1.0])  

        self.background = (10,10,50)
        self.nodeColour = (250,250,250)
        self.nodeRadius = 4
        
        self.control = 0
    
    def addWireframe(self, name, wireframe):
        self.wireframes[name] = wireframe
        #   If colour is set to None, then wireframe is not displayed
        self.wireframe_colours[name] = (250,250,250)
    
    def addWireframeGroup(self, wireframe_group):
        # Potential danger of overwriting names
        for name, wireframe in wireframe_group.wireframes.items():
            self.addWireframe(name, wireframe)
    
    def display(self):
        self.screen.fill(self.background)

        for name, wireframe in self.wireframes.items():
            nodes = wireframe.nodes
            
            if self.displayFaces:
                for (face, colour) in wireframe.sortedFaces():
                    v1 = (nodes[face[1]] - nodes[face[0]])[:3]
                    v2 = (nodes[face[2]] - nodes[face[0]])[:3]

                    normal = np.cross(v1, v2)
                    normal /= np.linalg.norm(normal)
                    towards_us = np.dot(normal, self.view_vector)
                    lv = self.light_vector
                    print (lv)
                    # lv = self.light_vector / np.linalg.norm(self.light_vector)
                    #my variables
                    mgloss = 5
                    mdiff = np.multiply([1,1,1],.5)
                    mspec = np.multiply([1,1,1],.4)
                    diff = 0
                    spec = 0
                    # Only draw faces that face us
                    if towards_us > 0:
                        m_ambient = 0.1
                        ambient = self.light_color * (m_ambient * colour)

                        #Your lighting code here
                        #Make note of the self.view_vector and self.light_vector 
                        #Use the Phong model
                        if np.dot(normal,lv) <= 0:
                            diff = spec = 0
                        else:
                            diff = (self.light_color * mdiff * colour) * np.dot(normal,lv)
                            # print diff
                            r = 2 * np.dot(normal,lv)*normal - lv
                            if np.dot(r,self.view_vector) <= 0:
                                # print "im really here"
                                spec = 0
                            else:
                            #     # print "im here"
                                spec = (self.light_color * mspec * colour) * (np.dot(r,self.view_vector) ** mgloss)

						#Once you have implemented diffuse and specular lighting, you will want to include them here
                        light_total = ambient + diff + spec
                        light_total = np.clip(light_total,0,255)
                        # print light_total
                        # print diff

                        pygame.draw.polygon(self.screen, light_total, [(nodes[node][0], nodes[node][1]) for node in face], 0)

                if self.displayEdges:
                    for (n1, n2) in wireframe.edges:
                        if self.perspective:
                            if wireframe.nodes[n1][2] > -self.perspective and nodes[n2][2] > -self.perspective:
                                z1 = self.perspective/ (self.perspective + nodes[n1][2])
                                x1 = self.width/2  + z1*(nodes[n1][0] - self.width/2)
                                y1 = self.height/2 + z1*(nodes[n1][1] - self.height/2)
                    
                                z2 = self.perspective/ (self.perspective + nodes[n2][2])
                                x2 = self.width/2  + z2*(nodes[n2][0] - self.width/2)
                                y2 = self.height/2 + z2*(nodes[n2][1] - self.height/2)
                                
                                pygame.draw.aaline(self.screen, colour, (x1, y1), (x2, y2), 1)
                        else:
                            pygame.draw.aaline(self.screen, colour, (nodes[n1][0], nodes[n1][1]), (nodes[n2][0], nodes[n2][1]), 1)

            if self.displayNodes:
                for node in nodes:
                    pygame.draw.circle(self.screen, colour, (int(node[0]), int(node[1])), self.nodeRadius, 0)
        
        pygame.display.flip()

    def keyEvent(self, key):
        global angleX ,angleY,angleZ
        #Your code here
        change = 1
        # if key == pygame.K_w:
            # print("w is pressed")
        if key == pygame.K_a:
            angleX = -.5
        if key == pygame.K_d:
            angleX = .5
        if key == pygame.K_w:
            angleY = -.5
        if key == pygame.K_s:
            angleY = .5
        if key == pygame.K_q:
            angleZ = -.5
        if key == pygame.K_e:
            angleZ = .5
        yMat = np.matrix([[math.cos(angleX),0,-math.sin(angleX)],
                        [0,1,0],
                        [math.sin(angleX),0,math.cos(angleX)]
                        ])
        xMat = np.matrix([
                        [1,0,0],
                        [0,math.cos(angleY),-math.sin(angleY)],
                        [0,math.sin(angleY),math.cos(angleY)]
                        ])
        zMat = np.matrix([
                        [math.cos(angleZ),-math.sin(angleZ),0],
                        [math.sin(angleZ),math.cos(angleZ),0],
                        [0,0,1]
                        ])
        if key == pygame.K_a:
            self.light_vector = yMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()
        if key == pygame.K_d:
            self.light_vector = yMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()
        if key == pygame.K_w:
            self.light_vector = xMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()
        if key == pygame.K_s:
            self.light_vector = xMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()
        if key == pygame.K_q:
            self.light_vector = zMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()
        if key == pygame.K_e:
            self.light_vector = zMat * np.transpose([self.light_vector])
            self.light_vector = (np.ravel(self.light_vector)).tolist()

        return

    def run(self):
        """ Display wireframe on screen and respond to keydown events """
        global running
        running = True
        key_down = False
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    key_down = event.key
                elif event.type == pygame.KEYUP:
                    key_down = None
            if key_down:
                if key_down == pygame.K_ESCAPE:
                    running = False
                self.keyEvent(key_down)
            
            self.display()
            self.update()
            
        pygame.quit()

		
resolution = 52
viewer = WireframeViewer(600, 400)
viewer.addWireframe('sphere', shape.Spheroid((300,200, 20), (160,160,160), resolution=resolution))
#angle changes?????
global angleX , angleY ,angleZ
angleX = angleY = angleZ = 0
# Colour ball
faces = viewer.wireframes['sphere'].faces
for i in range(int(resolution/4)):
	for j in range(resolution*2-4):
		f = i*(resolution*4-8) +j
		faces[f][1][1] = 0
		faces[f][1][2] = 0

viewer.displayEdges = False
viewer.run()
